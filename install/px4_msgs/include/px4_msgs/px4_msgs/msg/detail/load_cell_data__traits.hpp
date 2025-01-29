// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from px4_msgs:msg/LoadCellData.idl
// generated code does not contain a copyright notice

#ifndef PX4_MSGS__MSG__DETAIL__LOAD_CELL_DATA__TRAITS_HPP_
#define PX4_MSGS__MSG__DETAIL__LOAD_CELL_DATA__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "px4_msgs/msg/detail/load_cell_data__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace px4_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const LoadCellData & msg,
  std::ostream & out)
{
  out << "{";
  // member: timestamp
  {
    out << "timestamp: ";
    rosidl_generator_traits::value_to_yaml(msg.timestamp, out);
    out << ", ";
  }

  // member: force_x
  {
    out << "force_x: ";
    rosidl_generator_traits::value_to_yaml(msg.force_x, out);
    out << ", ";
  }

  // member: force_y
  {
    out << "force_y: ";
    rosidl_generator_traits::value_to_yaml(msg.force_y, out);
    out << ", ";
  }

  // member: force_z
  {
    out << "force_z: ";
    rosidl_generator_traits::value_to_yaml(msg.force_z, out);
    out << ", ";
  }

  // member: torque_x
  {
    out << "torque_x: ";
    rosidl_generator_traits::value_to_yaml(msg.torque_x, out);
    out << ", ";
  }

  // member: torque_y
  {
    out << "torque_y: ";
    rosidl_generator_traits::value_to_yaml(msg.torque_y, out);
    out << ", ";
  }

  // member: torque_z
  {
    out << "torque_z: ";
    rosidl_generator_traits::value_to_yaml(msg.torque_z, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const LoadCellData & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: timestamp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "timestamp: ";
    rosidl_generator_traits::value_to_yaml(msg.timestamp, out);
    out << "\n";
  }

  // member: force_x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "force_x: ";
    rosidl_generator_traits::value_to_yaml(msg.force_x, out);
    out << "\n";
  }

  // member: force_y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "force_y: ";
    rosidl_generator_traits::value_to_yaml(msg.force_y, out);
    out << "\n";
  }

  // member: force_z
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "force_z: ";
    rosidl_generator_traits::value_to_yaml(msg.force_z, out);
    out << "\n";
  }

  // member: torque_x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "torque_x: ";
    rosidl_generator_traits::value_to_yaml(msg.torque_x, out);
    out << "\n";
  }

  // member: torque_y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "torque_y: ";
    rosidl_generator_traits::value_to_yaml(msg.torque_y, out);
    out << "\n";
  }

  // member: torque_z
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "torque_z: ";
    rosidl_generator_traits::value_to_yaml(msg.torque_z, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const LoadCellData & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace px4_msgs

namespace rosidl_generator_traits
{

[[deprecated("use px4_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const px4_msgs::msg::LoadCellData & msg,
  std::ostream & out, size_t indentation = 0)
{
  px4_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use px4_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const px4_msgs::msg::LoadCellData & msg)
{
  return px4_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<px4_msgs::msg::LoadCellData>()
{
  return "px4_msgs::msg::LoadCellData";
}

template<>
inline const char * name<px4_msgs::msg::LoadCellData>()
{
  return "px4_msgs/msg/LoadCellData";
}

template<>
struct has_fixed_size<px4_msgs::msg::LoadCellData>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<px4_msgs::msg::LoadCellData>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<px4_msgs::msg::LoadCellData>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // PX4_MSGS__MSG__DETAIL__LOAD_CELL_DATA__TRAITS_HPP_
