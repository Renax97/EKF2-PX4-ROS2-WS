


#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <px4_msgs/msg/load_cell_data.hpp>

class ForceSensorReaderNode : public rclcpp::Node
{
public:
    ForceSensorReaderNode() : Node("force_sensor_reader_node")
    {
        // Subscriber per leggere i dati dal sensore di forza di Gazebo
        force_sensor_subscriber_ = this->create_subscription<geometry_msgs::msg::WrenchStamped>(
            "/world/default/model/x500_0/joint/link2_link3_joint/sensor/force_torque_sensor/forcetorque",
            10,
            std::bind(&ForceSensorReaderNode::forceSensorCallback, this, std::placeholders::_1));

        // Publisher per inviare i dati a PX4
        load_cell_publisher_ = this->create_publisher<px4_msgs::msg::LoadCellData>("/fmu/in/load_cell_data", 10);
    }

private:
    void forceSensorCallback(const geometry_msgs::msg::WrenchStamped::SharedPtr msg)
    {
        // Crea e popola il messaggio LoadCellData
        auto load_cell_msg = px4_msgs::msg::LoadCellData();
        load_cell_msg.timestamp = this->now().nanoseconds() / 1000;  // Microsecondi
        load_cell_msg.force_x = msg->wrench.force.x;
        load_cell_msg.force_y = msg->wrench.force.y;
        load_cell_msg.force_z = msg->wrench.force.z;
        load_cell_msg.torque_x = msg->wrench.torque.x;
        load_cell_msg.torque_y = msg->wrench.torque.y;
        load_cell_msg.torque_z = msg->wrench.torque.z;

        // Pubblica il messaggio per PX4
        RCLCPP_INFO(this->get_logger(), "Publishing LoadCellData: Fx=%.2f, Fy=%.2f, Fz=%.2f",
                    load_cell_msg.force_x, load_cell_msg.force_y, load_cell_msg.force_z);
        load_cell_publisher_->publish(load_cell_msg);
    }

    rclcpp::Subscription<geometry_msgs::msg::WrenchStamped>::SharedPtr force_sensor_subscriber_;
    rclcpp::Publisher<px4_msgs::msg::LoadCellData>::SharedPtr load_cell_publisher_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ForceSensorReaderNode>());
    rclcpp::shutdown();
    return 0;
}






/*

#include <rclcpp/rclcpp.hpp>
#include <ros_gz_interfaces/msg/contacts.hpp>  // Messaggio dei contatti
#include <px4_msgs/msg/load_cell_data.hpp>    // Messaggio per PX4

class ContactSensorReaderNode : public rclcpp::Node
{
public:
    ContactSensorReaderNode() : Node("contact_sensor_reader_node")
    {
        // Subscriber per il sensore di contatto
        contact_sensor_subscriber_ = this->create_subscription<ros_gz_interfaces::msg::Contacts>(
            "/world/default/model/x500_0/link/arm_link3/sensor/contact_sensor/contact",
            10,
            std::bind(&ContactSensorReaderNode::contactSensorCallback, this, std::placeholders::_1));

        // Publisher per inviare i dati a PX4
        load_cell_publisher_ = this->create_publisher<px4_msgs::msg::LoadCellData>("/fmu/in/load_cell_data", 10);
    }

private:
    void contactSensorCallback(const ros_gz_interfaces::msg::Contacts::SharedPtr msg)
    {
        auto load_cell_msg = px4_msgs::msg::LoadCellData();
        load_cell_msg.timestamp = this->now().nanoseconds() / 1000;  // Microsecondi

        if (msg->contacts.empty()) {
            RCLCPP_WARN(this->get_logger(), "Nessun contatto rilevato. Pubblicazione di valori nulli.");
            
            // Nessun contatto → Pubblica valori nulli
            load_cell_msg.force_x = 0.0;
            load_cell_msg.force_y = 0.0;
            load_cell_msg.force_z = 0.0;
        } else {
            // Prendiamo il primo contatto
            const auto& contact = msg->contacts[0];

            if (contact.wrenches.empty()) {
                RCLCPP_WARN(this->get_logger(), "Contatto rilevato ma nessun wrench trovato. Pubblicazione di valori nulli.");
                
                // Contatto senza forze → Pubblica valori nulli
                load_cell_msg.force_x = 0.0;
                load_cell_msg.force_y = 0.0;
                load_cell_msg.force_z = 0.0;
            } else {
                // Prendiamo il wrench del primo contatto
                const auto& wrench = contact.wrenches[0].body_1_wrench.force;
                load_cell_msg.force_x = wrench.x;
                load_cell_msg.force_y = wrench.y;
                load_cell_msg.force_z = wrench.z;

                RCLCPP_INFO(this->get_logger(), "Publishing LoadCellData: Fx=%.2f, Fy=%.2f, Fz=%.2f",
                            load_cell_msg.force_x, load_cell_msg.force_y, load_cell_msg.force_z);
            }
        }

        // Pubblica il messaggio per PX4
        load_cell_publisher_->publish(load_cell_msg);
    }

    rclcpp::Subscription<ros_gz_interfaces::msg::Contacts>::SharedPtr contact_sensor_subscriber_;
    rclcpp::Publisher<px4_msgs::msg::LoadCellData>::SharedPtr load_cell_publisher_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ContactSensorReaderNode>());
    rclcpp::shutdown();
    return 0;
}

*/


