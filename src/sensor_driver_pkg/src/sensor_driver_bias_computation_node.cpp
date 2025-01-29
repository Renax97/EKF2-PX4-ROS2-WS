#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <vector>

class ForceBiasCalculatorNode : public rclcpp::Node
{
public:
    ForceBiasCalculatorNode()
        : Node("force_bias_calculator_node"), sample_count_(0)
    {
        // Sottoscrizione al sensore di forza
        force_subscriber_ = this->create_subscription<geometry_msgs::msg::WrenchStamped>(
            "/world/default/model/x500_0/joint/link2_link3_joint/sensor/force_torque_sensor/forcetorque",
            10,
            std::bind(&ForceBiasCalculatorNode::forceCallback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Avviato il nodo di calibrazione del bias della forza.");
    }

    ~ForceBiasCalculatorNode()
    {
        // Calcolo della media al termine dell'esecuzione
        if (sample_count_ > 0) {
            double avg_force_x = sum_force_x_ / sample_count_;
            double avg_force_y = sum_force_y_ / sample_count_;
            double avg_force_z = sum_force_z_ / sample_count_;

            RCLCPP_INFO(this->get_logger(), "Bias medio calcolato:");
            RCLCPP_INFO(this->get_logger(), "Fx: %.4f N, Fy: %.4f N, Fz: %.4f N",
                        avg_force_x, avg_force_y, avg_force_z);
        } else {
            RCLCPP_WARN(this->get_logger(), "Nessun dato raccolto per il calcolo del bias.");
        }
    }

private:
    void forceCallback(const geometry_msgs::msg::WrenchStamped::SharedPtr msg)
    {
        // Accumulo dei dati di forza
        sum_force_x_ += msg->wrench.force.x;
        sum_force_y_ += msg->wrench.force.y;
        sum_force_z_ += msg->wrench.force.z;

        sample_count_++;

        // Stampa periodica (ogni 1000 campioni) per monitoraggio
        if (sample_count_ % 1000 == 0) {
            RCLCPP_INFO(this->get_logger(), "Campioni raccolti: %ld", sample_count_);
        }
    }

    // Sottoscrizione al sensore di forza
    rclcpp::Subscription<geometry_msgs::msg::WrenchStamped>::SharedPtr force_subscriber_;

    // Variabili per il calcolo della media
    double sum_force_x_ = 0.0;
    double sum_force_y_ = 0.0;
    double sum_force_z_ = 0.0;
    size_t sample_count_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ForceBiasCalculatorNode>());
    rclcpp::shutdown();
    return 0;
}

