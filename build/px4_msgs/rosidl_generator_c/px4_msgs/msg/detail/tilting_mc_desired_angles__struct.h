// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from px4_msgs:msg/TiltingMcDesiredAngles.idl
// generated code does not contain a copyright notice

#ifndef PX4_MSGS__MSG__DETAIL__TILTING_MC_DESIRED_ANGLES__STRUCT_H_
#define PX4_MSGS__MSG__DETAIL__TILTING_MC_DESIRED_ANGLES__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/TiltingMcDesiredAngles in the package px4_msgs.
typedef struct px4_msgs__msg__TiltingMcDesiredAngles
{
  /// time since system start (microseconds)
  uint64_t timestamp;
  /// body angle in NED frame
  float roll_body;
  /// body angle in NED flag_control_manual_enabled
  float pitch_body;
} px4_msgs__msg__TiltingMcDesiredAngles;

// Struct for a sequence of px4_msgs__msg__TiltingMcDesiredAngles.
typedef struct px4_msgs__msg__TiltingMcDesiredAngles__Sequence
{
  px4_msgs__msg__TiltingMcDesiredAngles * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} px4_msgs__msg__TiltingMcDesiredAngles__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // PX4_MSGS__MSG__DETAIL__TILTING_MC_DESIRED_ANGLES__STRUCT_H_
