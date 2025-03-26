
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <std_msgs/msg/float64.hpp>
#include <cmath>  // For sqrt()

class ForceControlledJoint : public rclcpp::Node {
public:
    ForceControlledJoint() : Node("force_controlled_joint"), 
        filtered_force_(0.0), 
        joint_lowered_(false), 
        waiting_for_timer_(false) {

        // Subscribe to force sensor topic
        force_subscriber_ = this->create_subscription<geometry_msgs::msg::WrenchStamped>(
            "/world/default/model/x500_vision_0/joint/link2_link3_joint/sensor/force_torque_sensor/forcetorque",
            10,
            std::bind(&ForceControlledJoint::force_callback, this, std::placeholders::_1)
        );

        // Publisher for joint velocity command
        joint_command_publisher_ = this->create_publisher<std_msgs::msg::Float64>(
            "/model/x500_vision_0/joint/link2_link3_joint/cmd_vel", 10
        );

        // Timer for delayed command execution
        command_timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),  // 1.5 seconds
            std::bind(&ForceControlledJoint::send_joint_command, this)
        );
        command_timer_->cancel();  // Initially stop the timer

        // Parameters
        force_threshold_ = 0.1;  // Threshold for force activation (N)
        filter_alpha_ = 0.1;  // Low-pass filter coefficient
    }

private:
    rclcpp::Subscription<geometry_msgs::msg::WrenchStamped>::SharedPtr force_subscriber_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr joint_command_publisher_;
    rclcpp::TimerBase::SharedPtr command_timer_;  // Timer for delayed action

    double filtered_force_;
    double force_threshold_;
    double filter_alpha_;
    bool joint_lowered_;  
    bool waiting_for_timer_;  // Flag to track if the delay timer is running

    void force_callback(const geometry_msgs::msg::WrenchStamped::SharedPtr msg) {
        // Calculate force magnitude (absolute value of force_y)
        double force_y = msg->wrench.force.y;
        double force_magnitude = std::abs(force_y);

        // Apply low-pass filter: y_filtered = alpha * new_value + (1 - alpha) * old_value
        filtered_force_ = filter_alpha_ * force_magnitude + (1 - filter_alpha_) * filtered_force_;

        RCLCPP_INFO(this->get_logger(), "Filtered Force Magnitude: %.2f N", filtered_force_);

        // If force exceeds threshold and the timer is not already running
        if (filtered_force_ > force_threshold_ && !waiting_for_timer_ && !joint_lowered_) {
            RCLCPP_INFO(this->get_logger(), "Force exceeded threshold! Waiting 1.5 seconds before command...");
            waiting_for_timer_ = true;  // Prevent multiple timers from starting
            command_timer_->reset();  // Start the delay timer
        } 
        // If force drops significantly below threshold, reset the state
        else if (filtered_force_ < (force_threshold_ * 0.8) && (joint_lowered_ || waiting_for_timer_)) {
            RCLCPP_INFO(this->get_logger(), "Force below threshold, resetting flags...");
            joint_lowered_ = false;
            waiting_for_timer_ = false;
            command_timer_->cancel();  // Stop any active timer
        }
    }

    void send_joint_command() {
        if (waiting_for_timer_ && !joint_lowered_) {
            RCLCPP_INFO(this->get_logger(), "1.5 seconds passed. Rotating joint...");

            std_msgs::msg::Float64 command_msg;
            command_msg.data = -3.0;  // Command to lower the joint
            joint_command_publisher_->publish(command_msg);

            joint_lowered_ = true;  // Mark joint as lowered
            waiting_for_timer_ = false;  // Reset timer state
        }
    }
};

// Main entry point for the ROS2 node
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ForceControlledJoint>());
    rclcpp::shutdown();
    return 0;
}











/*


#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <std_msgs/msg/float64.hpp>
#include <cmath>  // Per sqrt()

class ForceControlledJoint : public rclcpp::Node {
public:
    ForceControlledJoint() : Node("force_controlled_joint"), filtered_force_(0.0), joint_lowered_(false) {
        // Sottoscrizione al sensore di forza
        force_subscriber_ = this->create_subscription<geometry_msgs::msg::WrenchStamped>(
            "/world/default/model/x500_0/joint/link2_link3_joint/sensor/force_torque_sensor/forcetorque",
            10,
            std::bind(&ForceControlledJoint::force_callback, this, std::placeholders::_1)
        );

        // Publisher per controllare il giunto
        joint_command_publisher_ = this->create_publisher<std_msgs::msg::Float64>(
            "/model/x500_0/joint/link2_link3_joint/cmd_vel", 10
        );

        // Parametri
        force_threshold_ = 4.0;  // Soglia della forza totale (Newton)
        filter_alpha_ = 0.1;  // Filtro passa-basso
    }

private:
    rclcpp::Subscription<geometry_msgs::msg::WrenchStamped>::SharedPtr force_subscriber_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr joint_command_publisher_;
    double filtered_force_;
    double force_threshold_;
    double filter_alpha_;
    bool joint_lowered_;  // Flag per evitare pubblicazioni continue

    void force_callback(const geometry_msgs::msg::WrenchStamped::SharedPtr msg) {
        // Calcola il modulo della forza totale (F = sqrt(Fx^2 + Fy^2 + Fz^2))
        //double force_x = msg->wrench.force.x;
        double force_y = msg->wrench.force.y;
        //double force_z = msg->wrench.force.z;
        //double force_magnitude = std::sqrt(force_x * force_x + force_y * force_y + force_z * force_z);
        double force_magnitude = std::abs(force_y);

        // Filtro passa-basso: y_filtered = alpha * new_value + (1 - alpha) * old_value
        filtered_force_ = filter_alpha_ * force_magnitude + (1 - filter_alpha_) * filtered_force_;

        RCLCPP_INFO(this->get_logger(), "Filtered Force Magnitude: %.2f N", filtered_force_);

        // Se la forza supera la soglia e il giunto non è già abbassato
        if (filtered_force_ > force_threshold_ && !joint_lowered_) {
            RCLCPP_INFO(this->get_logger(), "Force threshold exceeded! Rotating joint...");

            std_msgs::msg::Float64 command_msg;
            command_msg.data = -100.0;  // Comando per abbassare il giunto
            joint_command_publisher_->publish(command_msg);

            joint_lowered_ = true;  // Evita pubblicazioni continue
        } 
        // Se la forza torna sotto la soglia, resettiamo il flag
        else if (filtered_force_ < (force_threshold_ * 0.8) && joint_lowered_) {
            RCLCPP_INFO(this->get_logger(), "Force below threshold, resetting joint flag...");
            joint_lowered_ = false;
        }
    }
};

// Punto di ingresso del nodo ROS 2
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ForceControlledJoint>());
    rclcpp::shutdown();
    return 0;
}




*/


/*










#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <std_msgs/msg/float64.hpp>
#include <cmath>  // Per sqrt()

class ForceControlledJoint : public rclcpp::Node {
public:
    ForceControlledJoint() : Node("force_controlled_joint"), filtered_force_(0.0), joint_lowered_(false) {
        // Sottoscrizione al sensore di forza
        force_subscriber_ = this->create_subscription<geometry_msgs::msg::WrenchStamped>(
            "/world/default/model/x500_0/link/arm_link3/sensor/contact_sensor/contact",
            10,
            std::bind(&ForceControlledJoint::force_callback, this, std::placeholders::_1)
        );

        // Publisher per controllare il giunto
        joint_command_publisher_ = this->create_publisher<std_msgs::msg::Float64>(
            "/model/x500_0/joint3/controller",
            10
        );

        // Parametri
        force_threshold_ = 5.0;  // Soglia della forza totale (Newton)
        filter_alpha_ = 0.1;  // Filtro passa-basso
    }

private:
    rclcpp::Subscription<geometry_msgs::msg::WrenchStamped>::SharedPtr force_subscriber_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr joint_command_publisher_;
    double filtered_force_;
    double force_threshold_;
    double filter_alpha_;
    bool joint_lowered_;  // Flag per evitare pubblicazioni continue

    void force_callback(const geometry_msgs::msg::WrenchStamped::SharedPtr msg) {
        // Calcola il modulo della forza totale (F = sqrt(Fx^2 + Fy^2 + Fz^2))
        //double force_x = msg->wrench.force.x;
        double force_y = msg->wrench.force.y;
        //double force_z = msg->wrench.force.z;
        //double force_magnitude = std::sqrt(force_x * force_x + force_y * force_y + force_z * force_z);
        double force_magnitude = std::abs(force_y);

        // Filtro passa-basso: y_filtered = alpha * new_value + (1 - alpha) * old_value
        filtered_force_ = filter_alpha_ * force_magnitude + (1 - filter_alpha_) * filtered_force_;

        RCLCPP_INFO(this->get_logger(), "Filtered Force Magnitude: %.2f N", filtered_force_);

        // Se la forza supera la soglia e il giunto non è già abbassato
        if (filtered_force_ > force_threshold_ && !joint_lowered_) {
            RCLCPP_INFO(this->get_logger(), "Force threshold exceeded! Rotating joint...");

            std_msgs::msg::Float64 command_msg;
            command_msg.data = -1.0;  // Comando per abbassare il giunto
            joint_command_publisher_->publish(command_msg);

            joint_lowered_ = true;  // Evita pubblicazioni continue
        } 
        // Se la forza torna sotto la soglia, resettiamo il flag
        else if (filtered_force_ < (force_threshold_ * 0.8) && joint_lowered_) {
            RCLCPP_INFO(this->get_logger(), "Force below threshold, resetting joint flag...");
            joint_lowered_ = false;
        }
    }
};

// Punto di ingresso del nodo ROS 2
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ForceControlledJoint>());
    rclcpp::shutdown();
    return 0;
}

*/


