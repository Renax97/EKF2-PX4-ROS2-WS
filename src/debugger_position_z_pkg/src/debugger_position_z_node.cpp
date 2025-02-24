#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/wrench_stamped.hpp"
#include "px4_msgs/msg/vehicle_odometry.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include <limits>

class ForceMonitorNode : public rclcpp::Node {
public:
    ForceMonitorNode() : Node("force_monitor_node"), z_position_(std::numeric_limits<double>::quiet_NaN()) {
        
        // Subscriber to force/torque sensor topic
        force_sub_ = this->create_subscription<geometry_msgs::msg::WrenchStamped>(
            "/world/default/model/x500_0/joint/link2_link3_joint/sensor/force_torque_sensor/forcetorque",
            10, std::bind(&ForceMonitorNode::force_callback, this, std::placeholders::_1));
        
        // Subscriber to vehicle odometry topic
        odom_sub_ = this->create_subscription<px4_msgs::msg::VehicleOdometry>(
            "/fmu/out/vehicle_odometry", 10,
            std::bind(&ForceMonitorNode::odom_callback, this, std::placeholders::_1));
        
        // Publisher for contact position z data
        contact_pub_ = this->create_publisher<std_msgs::msg::Float64MultiArray>("/contact/position_z", 10);
    }

private:
    void force_callback(const geometry_msgs::msg::WrenchStamped::SharedPtr msg) {
        double force_y = msg->wrench.force.y;
        if (std::abs(force_y) >= 1.0 && std::abs(force_y) <= 10.0) {
            publish_contact_position(z_position_);
        } else {
            publish_contact_position(1000.0);
        }
    }
    
    void odom_callback(const px4_msgs::msg::VehicleOdometry::SharedPtr msg) {
        z_position_ = msg->position[2];
    }
    
    void publish_contact_position(double z_value) {
        auto msg = std_msgs::msg::Float64MultiArray();
        msg.data.push_back(z_value);  // z_position_contact
        contact_pub_->publish(msg);

        RCLCPP_INFO(this->get_logger(), "Pubblicato: z_position_contact=%f", z_value);
    }

    rclcpp::Subscription<geometry_msgs::msg::WrenchStamped>::SharedPtr force_sub_;
    rclcpp::Subscription<px4_msgs::msg::VehicleOdometry>::SharedPtr odom_sub_;
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr contact_pub_;
    
    double z_position_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ForceMonitorNode>());
    rclcpp::shutdown();
    return 0;
}

