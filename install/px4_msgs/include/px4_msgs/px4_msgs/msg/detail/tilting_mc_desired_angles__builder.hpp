// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from px4_msgs:msg/TiltingMcDesiredAngles.idl
// generated code does not contain a copyright notice

#ifndef PX4_MSGS__MSG__DETAIL__TILTING_MC_DESIRED_ANGLES__BUILDER_HPP_
#define PX4_MSGS__MSG__DETAIL__TILTING_MC_DESIRED_ANGLES__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "px4_msgs/msg/detail/tilting_mc_desired_angles__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace px4_msgs
{

namespace msg
{

namespace builder
{

class Init_TiltingMcDesiredAngles_pitch_body
{
public:
  explicit Init_TiltingMcDesiredAngles_pitch_body(::px4_msgs::msg::TiltingMcDesiredAngles & msg)
  : msg_(msg)
  {}
  ::px4_msgs::msg::TiltingMcDesiredAngles pitch_body(::px4_msgs::msg::TiltingMcDesiredAngles::_pitch_body_type arg)
  {
    msg_.pitch_body = std::move(arg);
    return std::move(msg_);
  }

private:
  ::px4_msgs::msg::TiltingMcDesiredAngles msg_;
};

class Init_TiltingMcDesiredAngles_roll_body
{
public:
  explicit Init_TiltingMcDesiredAngles_roll_body(::px4_msgs::msg::TiltingMcDesiredAngles & msg)
  : msg_(msg)
  {}
  Init_TiltingMcDesiredAngles_pitch_body roll_body(::px4_msgs::msg::TiltingMcDesiredAngles::_roll_body_type arg)
  {
    msg_.roll_body = std::move(arg);
    return Init_TiltingMcDesiredAngles_pitch_body(msg_);
  }

private:
  ::px4_msgs::msg::TiltingMcDesiredAngles msg_;
};

class Init_TiltingMcDesiredAngles_timestamp
{
public:
  Init_TiltingMcDesiredAngles_timestamp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TiltingMcDesiredAngles_roll_body timestamp(::px4_msgs::msg::TiltingMcDesiredAngles::_timestamp_type arg)
  {
    msg_.timestamp = std::move(arg);
    return Init_TiltingMcDesiredAngles_roll_body(msg_);
  }

private:
  ::px4_msgs::msg::TiltingMcDesiredAngles msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::px4_msgs::msg::TiltingMcDesiredAngles>()
{
  return px4_msgs::msg::builder::Init_TiltingMcDesiredAngles_timestamp();
}

}  // namespace px4_msgs

#endif  // PX4_MSGS__MSG__DETAIL__TILTING_MC_DESIRED_ANGLES__BUILDER_HPP_
