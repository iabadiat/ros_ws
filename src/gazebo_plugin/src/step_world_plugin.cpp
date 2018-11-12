#include <gazebo/common/Plugin.hh>
#include <ros/ros.h>
#include <sdf/sdf.hh>
#include "gazebo/gazebo.hh"
#include "gazebo/msgs/msgs.hh"
#include "gazebo/physics/physics.hh"
#include "gazebo/transport/transport.hh"
#include "ros/subscribe_options.h"
#include "std_msgs/Bool.h"
#include <thread>
#include "ros/callback_queue.h"
#include "rosgraph_msgs/Clock.h"
#include <baxter_core_msgs/JointCommand.h>

namespace gazebo
{

class StepWorldPlugin : public WorldPlugin
{


private:
  physics::WorldPtr world;
  std::unique_ptr<ros::NodeHandle> rosNode;
  ros::Subscriber rosSub;
  ros::Subscriber rosSub_clock;
  ros::Subscriber unpause;
  ros::Publisher rosPub;
  ros::Publisher position_publisher;
  ros::CallbackQueue rosQueue;
  std::thread rosQueueThread;

  ros::Time clock_init;
  rosgraph_msgs::Clock clock_gazebo;
  ros::Time time;
  ros::Duration duration;
  ros::Duration step = ros::Duration (0.002);

  bool synchronizer_started = false;

  int count = 0;

public:
  StepWorldPlugin() : WorldPlugin(), time(0.0){}

  void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
  {
    // Make sure the ROS node for Gazebo has already been initialized
    if (!ros::isInitialized())
    {
      ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
        << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
      int argc = 0;
      char **argv = NULL;
      ros::init(argc, argv, "gazebo_client",
      ros::init_options::NoSigintHandler);
      ROS_INFO("LOAD IF");
      if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Info) ) {
          ros::console::notifyLoggerLevelsChanged();
      }
    }
    ROS_INFO("LOAD START");
    this->world = _world;
    // Create our ROS node. This acts in a similar manner to the Gazebo node
    this->rosNode.reset(new ros::NodeHandle("gazebo_client"));

    this->rosSub = this->rosNode->subscribe("/clock_sync", 100, &StepWorldPlugin::OnMsg_clocksync, this);
    this->rosSub_clock = this->rosNode->subscribe("/clock", 100, &StepWorldPlugin::OnMsg_clock, this);
    this->unpause = this->rosNode->subscribe("/unpause_gazebo", 100, &StepWorldPlugin::OnMsg_unpause, this);
    this->rosPub = this->rosNode->advertise<rosgraph_msgs::Clock>("/clock_gazebo", 100);
    this->position_publisher = this->rosNode->advertise<baxter_core_msgs::JointCommand>("/robot/limb/left/joint_command", 1);

    // Spin up the queue helper thread.
    this->rosQueueThread = std::thread(std::bind(&StepWorldPlugin::QueueThread, this));
    ROS_INFO("LOAD END");
  }

  //Function for stepping the simulation every time the synchronizer node publishes a new time
  public: void OnMsg_clocksync(const rosgraph_msgs::Clock& _msg)
  {
    //First message received from the synchronizer with time 0.0:
    //Pause the simulation, get the time at which the simulation has stopped as a reference,
    //Publish time 0.0 as gazebo clock so the synchronizer knows gazebo is ready
    if (_msg.clock.toSec() == 0.0){
      //ROS_INFO("MSG CLOCK_SYNC 0.0");
      this->world->SetPaused(true);
      this->clock_init = ros::Time::now();
      //ROS_INFO("CLOCK INIT %f", this->clock_init.toSec());
      this->duration = ros::Time::now() - _msg.clock;
      this->clock_gazebo.clock = ros::Time::now() - this->duration;
      this->rosPub.publish(clock_gazebo); //publish on new topic (i.e. clock_gazebo)
      //ROS_INFO("PUB GZCLOCK %f", clock_gazebo.clock.toSec());
      this->synchronizer_started = true;
      this->time = ros::Time (0.0);
    }
    //When a different time than 0.0 is received from the synchronizer STEP the simulation and update time
    else if (_msg.clock > this->time){
      this->time = _msg.clock;
      this->count = this->count + 1;
      //ROS_INFO("STEP GZ %i", count);
      this->world->Step(1);
    }
  }

  //Function for publishing Gazebo's time every time a step is done.
  //Time 0 = Initialization time
  public: void OnMsg_clock(const rosgraph_msgs::Clock& _msg)
  {
      //When Gazebo publishes on clock means that a step has been finished. Therefore publish to
      //the synchronizer node so it knows that Gazebo has finished a step
      if(this->synchronizer_started){
        this->clock_gazebo.clock = this->clock_gazebo.clock + this->step;
        this->rosPub.publish(clock_gazebo);
        //ROS_INFO("PUB GZCLOCK %f", clock_gazebo.clock.toSec());
        //ROS_INFO("T - D %f", ros::Time::now().toSec() - this->duration.toSec());
      }
  }

  //Send baxter arm to position 0.0.0.0.0.0.0 when a message is received. When the cerebellum
  //simulation is finished, it publishes on topic /unpause_gazebo. Gazebo will keep running and send the
  //arm to zero position
  public: void OnMsg_unpause(const std_msgs::Bool& _msg)
  {
    this->world->SetPaused(false);

  	baxter_core_msgs::JointCommand newMsg;
  	std::vector<double> StartingPosition(7);
  	for (int i = 0; i< StartingPosition.size(); i++){
  		StartingPosition[i] = 0;
  	}
    std::vector<std::string> joint_list;

    joint_list.push_back("left_s0");
    joint_list.push_back("left_s1");
    joint_list.push_back("left_e0");
    joint_list.push_back("left_e1");
    joint_list.push_back("left_w0");
    joint_list.push_back("left_w1");
    joint_list.push_back("left_w2");

  	newMsg.mode = 1; // Position control mode
  	newMsg.names = joint_list;
  	newMsg.command = StartingPosition;

    this->position_publisher.publish(newMsg);
    this->synchronizer_started = false;
  }


  /// \brief ROS helper function that processes messages
  private: void QueueThread()
  {
    static const double timeout = 0.0001;
    while (this->rosNode->ok())
    {
      this->rosQueue.callAvailable(ros::WallDuration(timeout));
      //ROS_INFO("QUEUE THREAD");
    }
  }

};
GZ_REGISTER_WORLD_PLUGIN(StepWorldPlugin)
}
