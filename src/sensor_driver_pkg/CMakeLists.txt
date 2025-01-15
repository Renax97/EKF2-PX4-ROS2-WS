cmake_minimum_required(VERSION 3.8)
project(sensor_driver_pkg)

# Trova le dipendenze richieste
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(px4_msgs REQUIRED)

# Aggiungi gli eseguibili del nodo
add_executable(sensor_driver_node src/sensor_driver_node.cpp)
add_executable(sensor_driver_flamingo_node src/sensor_driver_flamingo_node.cpp)
add_executable(sensor_driver_gz_node src/sensor_driver_gz_node.cpp)

# Specifica le dipendenze per gli eseguibili
ament_target_dependencies(sensor_driver_node rclcpp geometry_msgs)
ament_target_dependencies(sensor_driver_flamingo_node rclcpp geometry_msgs)
ament_target_dependencies(sensor_driver_gz_node rclcpp geometry_msgs px4_msgs)

# Installa gli eseguibili
install(TARGETS
  sensor_driver_node
  sensor_driver_flamingo_node
  sensor_driver_gz_node
  DESTINATION lib/${PROJECT_NAME})

# Richiesta di ament_package
ament_package()

