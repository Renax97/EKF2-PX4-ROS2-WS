cmake_minimum_required(VERSION 3.8)
project(apply_link_wrench_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(ros_gz_interfaces REQUIRED)
find_package(px4_msgs REQUIRED)

# Define the executable
add_executable(apply_link_wrench src/apply_link_wrench.cpp)

# Link dependencies to the executable
ament_target_dependencies(apply_link_wrench rclcpp ros_gz_interfaces px4_msgs)

# Install the executable
install(TARGETS
  apply_link_wrench
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

