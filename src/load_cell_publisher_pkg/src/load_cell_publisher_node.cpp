#include <rclcpp/rclcpp.hpp>
#include <px4_msgs/msg/load_cell_data.hpp>
#include <chrono>

using namespace std::chrono_literals;

class LoadCellPublisherNode : public rclcpp::Node
{
public:
    LoadCellPublisherNode() : Node("load_cell_publisher_node")
    {
        // Crea il publisher
        publisher_ = this->create_publisher<px4_msgs::msg::LoadCellData>("/fmu/in/load_cell_data", 10);

        // Timer per pubblicare dati ogni 10 secondi
        timer_ = this->create_wall_timer(10s, std::bind(&LoadCellPublisherNode::publishData, this));
    }

private:
    void publishData()
    {
        // Creare e popolare il messaggio
        auto message = px4_msgs::msg::LoadCellData();
        message.timestamp = this->now().nanoseconds() / 1000;  // Microsecondi
        message.force_x = 0.0;  // Valore fittizio
        message.force_y = 0.0;  // Valore fittizio
        message.force_z = -2.0; // Valore fittizio
        message.torque_x = 0.0;  // Valore fittizio
        message.torque_y = 0.0;  // Valore fittizio
        message.torque_z = 0.0;  // Valore fittizio

        // Pubblicare il messaggio
        RCLCPP_INFO(this->get_logger(), "Publishing load cell data: Fx=%.2f, Fy=%.2f, Fz=%.2f",
                    message.force_x, message.force_y, message.force_z);
        publisher_->publish(message);
    }

    rclcpp::Publisher<px4_msgs::msg::LoadCellData>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LoadCellPublisherNode>());
    rclcpp::shutdown();
    return 0;
}








