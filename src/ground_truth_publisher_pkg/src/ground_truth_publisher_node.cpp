#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"

class OdometryConverter : public rclcpp::Node {
public:
    OdometryConverter() : Node("odometry_converter") {
        // Sottoscrizione al topic di odometria
        odometry_subscriber_ = this->create_subscription<nav_msgs::msg::Odometry>(
            "/model/x500_0/odometry", 1,
            std::bind(&OdometryConverter::odometry_callback, this, std::placeholders::_1));

        // Publisher per il topic convertito
        ground_truth_publisher_ = this->create_publisher<nav_msgs::msg::Odometry>(
            "/model/x500_vision_0/ground_truth", 1);
    }

private:
    void odometry_callback(const nav_msgs::msg::Odometry::SharedPtr msg) {
        // Creiamo un nuovo messaggio per il frame NED
        auto ned_msg = std::make_shared<nav_msgs::msg::Odometry>();

        // Copiamo l'header e il frame ID
        ned_msg->header = msg->header;
        ned_msg->child_frame_id = msg->child_frame_id;

        // **Convertiamo la posizione da ENU a NED**
        ned_msg->pose.pose.position.x = msg->pose.pose.position.y;   // ENU y -> NED x
        ned_msg->pose.pose.position.y = msg->pose.pose.position.x;   // ENU x -> NED y
        ned_msg->pose.pose.position.z = -msg->pose.pose.position.z;  // ENU z -> NED -z

        // **Convertiamo la velocità lineare da ENU a NED**
        ned_msg->twist.twist.linear.x = msg->twist.twist.linear.y;   // ENU y -> NED x
        ned_msg->twist.twist.linear.y = msg->twist.twist.linear.x;   // ENU x -> NED y
        ned_msg->twist.twist.linear.z = -msg->twist.twist.linear.z;  // ENU z -> NED -z

        // **Manteniamo l'orientazione in forma di quaternione senza modificarla**
        ned_msg->pose.pose.orientation = msg->pose.pose.orientation;

        // **Manteniamo la velocità angolare senza modificarla**
        ned_msg->twist.twist.angular = msg->twist.twist.angular;

        // **Manteniamo la matrice di covarianza**
        ned_msg->pose.covariance = msg->pose.covariance;
        ned_msg->twist.covariance = msg->twist.covariance;

        // **Pubblicazione del messaggio convertito**
        ground_truth_publisher_->publish(*ned_msg);
    }

    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odometry_subscriber_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr ground_truth_publisher_;
};

// **Funzione principale**
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OdometryConverter>());
    rclcpp::shutdown();
    return 0;
}

