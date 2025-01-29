// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from px4_msgs:msg/ExternalWrenchEstimation.idl
// generated code does not contain a copyright notice

#ifndef PX4_MSGS__MSG__DETAIL__EXTERNAL_WRENCH_ESTIMATION__STRUCT_H_
#define PX4_MSGS__MSG__DETAIL__EXTERNAL_WRENCH_ESTIMATION__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Constant 'ORB_QUEUE_LENGTH'.
/**
  * Lunghezza massima della coda del topic
 */
enum
{
  px4_msgs__msg__ExternalWrenchEstimation__ORB_QUEUE_LENGTH = 8
};

/// Struct defined in msg/ExternalWrenchEstimation in the package px4_msgs.
typedef struct px4_msgs__msg__ExternalWrenchEstimation
{
  /// Tempo dal boot del sistema, in microsecondi
  uint64_t timestamp;
  /// Componente X della forza (es. in Newton)
  float force_x;
  /// Componente Y della forza (es. in Newton)
  float force_y;
  /// Componente Z della forza (es. in Newton)
  float force_z;
  /// Componente X del momento torcente (es. in Newton-metro)
  float torque_x;
  /// Componente Y del momento torcente (es. in Newton-metro)
  float torque_y;
  /// Componente Z del momento torcente (es. in Newton-metro)
  float torque_z;
} px4_msgs__msg__ExternalWrenchEstimation;

// Struct for a sequence of px4_msgs__msg__ExternalWrenchEstimation.
typedef struct px4_msgs__msg__ExternalWrenchEstimation__Sequence
{
  px4_msgs__msg__ExternalWrenchEstimation * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} px4_msgs__msg__ExternalWrenchEstimation__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // PX4_MSGS__MSG__DETAIL__EXTERNAL_WRENCH_ESTIMATION__STRUCT_H_
