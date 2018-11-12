
"use strict";

let NavigatorState = require('./NavigatorState.js');
let AnalogIOState = require('./AnalogIOState.js');
let HeadState = require('./HeadState.js');
let DigitalIOStates = require('./DigitalIOStates.js');
let EndEffectorProperties = require('./EndEffectorProperties.js');
let EndEffectorCommand = require('./EndEffectorCommand.js');
let CameraSettings = require('./CameraSettings.js');
let DigitalIOState = require('./DigitalIOState.js');
let SEAJointState = require('./SEAJointState.js');
let CollisionAvoidanceState = require('./CollisionAvoidanceState.js');
let EndpointStates = require('./EndpointStates.js');
let EndpointState = require('./EndpointState.js');
let HeadPanCommand = require('./HeadPanCommand.js');
let CollisionDetectionState = require('./CollisionDetectionState.js');
let AssemblyState = require('./AssemblyState.js');
let URDFConfiguration = require('./URDFConfiguration.js');
let AnalogOutputCommand = require('./AnalogOutputCommand.js');
let NavigatorStates = require('./NavigatorStates.js');
let CameraControl = require('./CameraControl.js');
let RobustControllerStatus = require('./RobustControllerStatus.js');
let AssemblyStates = require('./AssemblyStates.js');
let JointCommand = require('./JointCommand.js');
let EndEffectorState = require('./EndEffectorState.js');
let AnalogIOStates = require('./AnalogIOStates.js');
let DigitalOutputCommand = require('./DigitalOutputCommand.js');

module.exports = {
  NavigatorState: NavigatorState,
  AnalogIOState: AnalogIOState,
  HeadState: HeadState,
  DigitalIOStates: DigitalIOStates,
  EndEffectorProperties: EndEffectorProperties,
  EndEffectorCommand: EndEffectorCommand,
  CameraSettings: CameraSettings,
  DigitalIOState: DigitalIOState,
  SEAJointState: SEAJointState,
  CollisionAvoidanceState: CollisionAvoidanceState,
  EndpointStates: EndpointStates,
  EndpointState: EndpointState,
  HeadPanCommand: HeadPanCommand,
  CollisionDetectionState: CollisionDetectionState,
  AssemblyState: AssemblyState,
  URDFConfiguration: URDFConfiguration,
  AnalogOutputCommand: AnalogOutputCommand,
  NavigatorStates: NavigatorStates,
  CameraControl: CameraControl,
  RobustControllerStatus: RobustControllerStatus,
  AssemblyStates: AssemblyStates,
  JointCommand: JointCommand,
  EndEffectorState: EndEffectorState,
  AnalogIOStates: AnalogIOStates,
  DigitalOutputCommand: DigitalOutputCommand,
};
