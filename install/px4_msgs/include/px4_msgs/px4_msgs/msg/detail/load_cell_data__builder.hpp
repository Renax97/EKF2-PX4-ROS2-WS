// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from px4_msgs:msg/LoadCellData.idl
// generated code does not contain a copyright notice

#ifndef PX4_MSGS__MSG__DETAIL__LOAD_CELL_DATA__BUILDER_HPP_
#define PX4_MSGS__MSG__DETAIL__LOAD_CELL_DATA__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "px4_msgs/msg/detail/load_cell_data__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace px4_msgs
{

namespace msg
{

namespace builder
{

class Init_LoadCellData_torque_z
{
public:
  explicit Init_LoadCellData_torque_z(::px4_msgs::msg::LoadCellData & msg)
  : msg_(msg)
  {}
  ::px4_msgs::msg::LoadCellData torque_z(::px4_msgs::msg::LoadCellData::_torque_z_type arg)
  {
    msg_.torque_z = std::move(arg);
    return std::move(msg_);
  }

private:
  ::px4_msgs::msg::LoadCellData msg_;
};

class Init_LoadCellData_torque_y
{
public:
  explicit Init_LoadCellData_torque_y(::px4_msgs::msg::LoadCellData & msg)
  : msg_(msg)
  {}
  Init_LoadCellData_torque_z torque_y(::px4_msgs::msg::LoadCellData::_torque_y_type arg)
  {
    msg_.torque_y = std::move(arg);
    return Init_LoadCellData_torque_z(msg_);
  }

private:
  ::px4_msgs::msg::LoadCellData msg_;
};

class Init_LoadCellData_torque_x
{
public:
  explicit Init_LoadCellData_torque_x(::px4_msgs::msg::LoadCellData & msg)
  : msg_(msg)
  {}
  Init_LoadCellData_torque_y torque_x(::px4_msgs::msg::LoadCellData::_torque_x_type arg)
  {
    msg_.torque_x = std::move(arg);
    return Init_LoadCellData_torque_y(msg_);
  }

private:
  ::px4_msgs::msg::LoadCellData msg_;
};

class Init_LoadCellData_force_z
{
public:
  explicit Init_LoadCellData_force_z(::px4_msgs::msg::LoadCellData & msg)
  : msg_(msg)
  {}
  Init_LoadCellData_torque_x force_z(::px4_msgs::msg::LoadCellData::_force_z_type arg)
  {
    msg_.force_z = std::move(arg);
    return Init_LoadCellData_torque_x(msg_);
  }

private:
  ::px4_msgs::msg::LoadCellData msg_;
};

class Init_LoadCellData_force_y
{
public:
  explicit Init_LoadCellData_force_y(::px4_msgs::msg::LoadCellData & msg)
  : msg_(msg)
  {}
  Init_LoadCellData_force_z force_y(::px4_msgs::msg::LoadCellData::_force_y_type arg)
  {
    msg_.force_y = std::move(arg);
    return Init_LoadCellData_force_z(msg_);
  }

private:
  ::px4_msgs::msg::LoadCellData msg_;
};

class Init_LoadCellData_force_x
{
public:
  explicit Init_LoadCellData_force_x(::px4_msgs::msg::LoadCellData & msg)
  : msg_(msg)
  {}
  Init_LoadCellData_force_y force_x(::px4_msgs::msg::LoadCellData::_force_x_type arg)
  {
    msg_.force_x = std::move(arg);
    return Init_LoadCellData_force_y(msg_);
  }

private:
  ::px4_msgs::msg::LoadCellData msg_;
};

class Init_LoadCellData_timestamp
{
public:
  Init_LoadCellData_timestamp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LoadCellData_force_x timestamp(::px4_msgs::msg::LoadCellData::_timestamp_type arg)
  {
    msg_.timestamp = std::move(arg);
    return Init_LoadCellData_force_x(msg_);
  }

private:
  ::px4_msgs::msg::LoadCellData msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::px4_msgs::msg::LoadCellData>()
{
  return px4_msgs::msg::builder::Init_LoadCellData_timestamp();
}

}  // namespace px4_msgs

#endif  // PX4_MSGS__MSG__DETAIL__LOAD_CELL_DATA__BUILDER_HPP_
