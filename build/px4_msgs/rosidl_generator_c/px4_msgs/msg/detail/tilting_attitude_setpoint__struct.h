// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from px4_msgs:msg/TiltingAttitudeSetpoint.idl
// generated code does not contain a copyright notice

#ifndef PX4_MSGS__MSG__DETAIL__TILTING_ATTITUDE_SETPOINT__STRUCT_H_
#define PX4_MSGS__MSG__DETAIL__TILTING_ATTITUDE_SETPOINT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/TiltingAttitudeSetpoint in the package px4_msgs.
typedef struct px4_msgs__msg__TiltingAttitudeSetpoint
{
  /// time since system start (microseconds)
  uint64_t timestamp;
  /// For quaternion-based attitude control
  /// Desired quaternion for quaternion control
  float q_d[4];
} px4_msgs__msg__TiltingAttitudeSetpoint;

// Struct for a sequence of px4_msgs__msg__TiltingAttitudeSetpoint.
typedef struct px4_msgs__msg__TiltingAttitudeSetpoint__Sequence
{
  px4_msgs__msg__TiltingAttitudeSetpoint * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} px4_msgs__msg__TiltingAttitudeSetpoint__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // PX4_MSGS__MSG__DETAIL__TILTING_ATTITUDE_SETPOINT__STRUCT_H_
