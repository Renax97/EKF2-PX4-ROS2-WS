#include <rclcpp/rclcpp.hpp>
#include <px4_msgs/msg/vehicle_odometry.hpp>
#include <px4_msgs/msg/estimator_states.hpp>
#include <px4_msgs/msg/estimator_status.hpp>
#include <px4_msgs/msg/external_wrench_estimation.hpp>
#include <px4_msgs/msg/trajectory_setpoint.hpp>
#include "nav_msgs/msg/odometry.hpp"

#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

class ZVelocityLogger : public rclcpp::Node
{
public:
    ZVelocityLogger()
    : Node("z_velocity_logger")
    {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream filename_ss;
        filename_ss << "z_velocity_log_" << std::put_time(std::localtime(&now_time_t), "%Y%m%d_%H%M%S") << ".csv";
        csv_filename_ = filename_ss.str();

        csv_file_.open(csv_filename_, std::ios::out);
        if (!csv_file_.is_open()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to open file: %s", csv_filename_.c_str());
            return;
        }

        // ✅ Expanded CSV header
        csv_file_ << "ROS_Timestamp_Sec, PX4_Timestamp_us, Z_Velocity_PX4_m_s, Z_Velocity_GT_m_s, "
                  << "Estimator_Vz_m_s, Output_Tracking_Error_Vel, "
                  << "Force_Z, Torque_X_Kalman_Gain, Torque_Y_Pseudo_Vel, Torque_Z_Filtered_Force, "
                  << "Trajectory_Setpoint_Vel_Z, "
                  << "Estimator_Pos_Z_m, PX4_Odom_Pos_Z_m, GT_Pos_Z_m\n";  // ✅ Added new fields
        csv_file_.flush();

        // Subscriptions
        px4_subscription_ = this->create_subscription<px4_msgs::msg::VehicleOdometry>(
            "/fmu/out/vehicle_odometry",
            rclcpp::SensorDataQoS(),
            std::bind(&ZVelocityLogger::px4OdometryCallback, this, std::placeholders::_1)
        );

        gt_subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
            "/model/x500_vision_0/ground_truth",
            rclcpp::SensorDataQoS(),
            std::bind(&ZVelocityLogger::groundTruthCallback, this, std::placeholders::_1)
        );

        estimator_states_subscription_ = this->create_subscription<px4_msgs::msg::EstimatorStates>(
            "/fmu/out/estimator_states",
            rclcpp::SensorDataQoS(),
            std::bind(&ZVelocityLogger::estimatorStatesCallback, this, std::placeholders::_1)
        );

        estimator_status_subscription_ = this->create_subscription<px4_msgs::msg::EstimatorStatus>(
            "/fmu/out/estimator_status",
            rclcpp::SensorDataQoS(),
            std::bind(&ZVelocityLogger::estimatorStatusCallback, this, std::placeholders::_1)
        );

        external_wrench_subscription_ = this->create_subscription<px4_msgs::msg::ExternalWrenchEstimation>(
            "/fmu/out/external_wrench_estimation",
            rclcpp::SensorDataQoS(),
            std::bind(&ZVelocityLogger::externalWrenchCallback, this, std::placeholders::_1)
        );

        trajectory_setpoint_subscription_ = this->create_subscription<px4_msgs::msg::TrajectorySetpoint>(
            "/fmu/in/trajectory_setpoint",
            rclcpp::SensorDataQoS(),
            std::bind(&ZVelocityLogger::trajectorySetpointCallback, this, std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "Logger Node started. Writing to %s", csv_filename_.c_str());
    }

    ~ZVelocityLogger()
    {
        if (csv_file_.is_open()) {
            csv_file_.close();
        }
    }

private:
    // PX4 Odometry
    void px4OdometryCallback(const px4_msgs::msg::VehicleOdometry::SharedPtr msg)
    {
        latest_px4_z_velocity_ = msg->velocity[2];
        latest_px4_position_z_ = msg->position[2];  // ✅ PX4 odometry position Z
        latest_px4_timestamp_us_ = msg->timestamp;
        writeToCSV();
    }

    // Ground Truth
    void groundTruthCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        latest_gt_z_velocity_ = msg->twist.twist.linear.z;

        // ✅ Ground truth position Z (with compensation for offset)
        latest_gt_position_z_ = msg->pose.pose.position.z + 0.227;

        writeToCSV();
    }

    // Estimator States (vz and position z)
    void estimatorStatesCallback(const px4_msgs::msg::EstimatorStates::SharedPtr msg)
    {
        if (msg->n_states > 6) {
            latest_estimator_vz_ = msg->states[6];
        }

        if (msg->n_states > 9) {
            latest_estimator_pos_z_ = msg->states[9];  // ✅ Estimator position Z
        }

        writeToCSV();
    }

    // Estimator Status (output_tracking_error[1])
    void estimatorStatusCallback(const px4_msgs::msg::EstimatorStatus::SharedPtr msg)
    {
        latest_output_tracking_error_vel_ = msg->output_tracking_error[1];
        writeToCSV();
    }

    // External Wrench Estimation (force_z, torque_x, torque_y, torque_z)
    void externalWrenchCallback(const px4_msgs::msg::ExternalWrenchEstimation::SharedPtr msg)
    {
        latest_force_z_ = msg->force_z;
        latest_torque_x_kalman_gain_ = msg->torque_x;
        latest_torque_y_pseudo_velocity_ = msg->torque_y;
        latest_torque_z_filtered_force_ = msg->torque_z;
        writeToCSV();
    }

    // Trajectory Setpoint (velocity[2])
    void trajectorySetpointCallback(const px4_msgs::msg::TrajectorySetpoint::SharedPtr msg)
    {
        latest_trajectory_setpoint_vel_z_ = msg->velocity[2];
        writeToCSV();
    }

    void writeToCSV()
    {
        double ros_timestamp_sec = this->now().seconds();

        if (csv_file_.is_open()) {
            csv_file_ << std::fixed << std::setprecision(6)
                      << ros_timestamp_sec << ","
                      << latest_px4_timestamp_us_ << ","
                      << latest_px4_z_velocity_ << ","
                      << latest_gt_z_velocity_ << ","
                      << latest_estimator_vz_ << ","
                      << latest_output_tracking_error_vel_ << ","
                      << latest_force_z_ << ","
                      << latest_torque_x_kalman_gain_ << ","
                      << latest_torque_y_pseudo_velocity_ << ","
                      << latest_torque_z_filtered_force_ << ","
                      << latest_trajectory_setpoint_vel_z_ << ","
                      << latest_estimator_pos_z_ << ","            // ✅ Estimator position Z
                      << latest_px4_position_z_ << ","             // ✅ PX4 odometry position Z
                      << latest_gt_position_z_                     // ✅ GT position Z (compensated)
                      << "\n";

            csv_file_.flush();
        }

        RCLCPP_INFO(this->get_logger(),
            "[ROS Time: %.6f] PX4 Z Vel: %.4f m/s | GT Z Vel: %.4f m/s | Est Vz: %.4f | "
            "Track Err Vel: %.4f | Fz: %.4f | Tx(KG): %.4f | Ty(PV): %.4f | Tz(FF): %.4f | "
            "TSP Vel Z: %.4f | Est Pos Z: %.4f | PX4 Pos Z: %.4f | GT Pos Z (comp): %.4f",
            ros_timestamp_sec,
            latest_px4_z_velocity_,
            latest_gt_z_velocity_,
            latest_estimator_vz_,
            latest_output_tracking_error_vel_,
            latest_force_z_,
            latest_torque_x_kalman_gain_,
            latest_torque_y_pseudo_velocity_,
            latest_torque_z_filtered_force_,
            latest_trajectory_setpoint_vel_z_,
            latest_estimator_pos_z_,
            latest_px4_position_z_,
            latest_gt_position_z_);
    }

    std::string csv_filename_;
    std::ofstream csv_file_;

    // Subscriptions
    rclcpp::Subscription<px4_msgs::msg::VehicleOdometry>::SharedPtr px4_subscription_;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr gt_subscription_;
    rclcpp::Subscription<px4_msgs::msg::EstimatorStates>::SharedPtr estimator_states_subscription_;
    rclcpp::Subscription<px4_msgs::msg::EstimatorStatus>::SharedPtr estimator_status_subscription_;
    rclcpp::Subscription<px4_msgs::msg::ExternalWrenchEstimation>::SharedPtr external_wrench_subscription_;
    rclcpp::Subscription<px4_msgs::msg::TrajectorySetpoint>::SharedPtr trajectory_setpoint_subscription_;

    // Latest values for CSV and logging
    float latest_px4_z_velocity_ = 0.0;
    uint64_t latest_px4_timestamp_us_ = 0;

    float latest_gt_z_velocity_ = 0.0;

    float latest_estimator_vz_ = 0.0;
    float latest_output_tracking_error_vel_ = 0.0;

    float latest_force_z_ = 0.0;
    float latest_torque_x_kalman_gain_ = 0.0;
    float latest_torque_y_pseudo_velocity_ = 0.0;
    float latest_torque_z_filtered_force_ = 0.0;

    float latest_trajectory_setpoint_vel_z_ = 0.0;

    // ✅ New position data
    float latest_estimator_pos_z_ = 0.0;
    float latest_px4_position_z_ = 0.0;
    float latest_gt_position_z_ = 0.0;  // Already includes +2.227 compensation
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ZVelocityLogger>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

