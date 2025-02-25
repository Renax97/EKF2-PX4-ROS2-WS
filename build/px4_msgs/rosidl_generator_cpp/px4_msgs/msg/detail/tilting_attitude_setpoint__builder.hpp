// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from px4_msgs:msg/TiltingAttitudeSetpoint.idl
// generated code does not contain a copyright notice

#ifndef PX4_MSGS__MSG__DETAIL__TILTING_ATTITUDE_SETPOINT__BUILDER_HPP_
#define PX4_MSGS__MSG__DETAIL__TILTING_ATTITUDE_SETPOINT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "px4_msgs/msg/detail/tilting_attitude_setpoint__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace px4_msgs
{

namespace msg
{

namespace builder
{

class Init_TiltingAttitudeSetpoint_q_d
{
public:
  explicit Init_TiltingAttitudeSetpoint_q_d(::px4_msgs::msg::TiltingAttitudeSetpoint & msg)
  : msg_(msg)
  {}
  ::px4_msgs::msg::TiltingAttitudeSetpoint q_d(::px4_msgs::msg::TiltingAttitudeSetpoint::_q_d_type arg)
  {
    msg_.q_d = std::move(arg);
    return std::move(msg_);
  }

private:
  ::px4_msgs::msg::TiltingAttitudeSetpoint msg_;
};

class Init_TiltingAttitudeSetpoint_timestamp
{
public:
  Init_TiltingAttitudeSetpoint_timestamp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_TiltingAttitudeSetpoint_q_d timestamp(::px4_msgs::msg::TiltingAttitudeSetpoint::_timestamp_type arg)
  {
    msg_.timestamp = std::move(arg);
    return Init_TiltingAttitudeSetpoint_q_d(msg_);
  }

private:
  ::px4_msgs::msg::TiltingAttitudeSetpoint msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::px4_msgs::msg::TiltingAttitudeSetpoint>()
{
  return px4_msgs::msg::builder::Init_TiltingAttitudeSetpoint_timestamp();
}

}  // namespace px4_msgs

#endif  // PX4_MSGS__MSG__DETAIL__TILTING_ATTITUDE_SETPOINT__BUILDER_HPP_
