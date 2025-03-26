

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <px4_msgs/msg/load_cell_data.hpp>
#include <std_msgs/msg/header.hpp>

class ForceSensorReaderNode : public rclcpp::Node
{
public:
    ForceSensorReaderNode() : Node("force_sensor_reader_node")
    {
        // Define QoS profile for force sensor topic (reliable, keep last message)
        rclcpp::QoS qos_profile = rclcpp::QoS(rclcpp::KeepLast(10))
                                      .reliable()
                                      .durability_volatile();

        // Subscriber for Gazebo force/torque sensor
        force_sensor_subscriber_ = this->create_subscription<geometry_msgs::msg::WrenchStamped>(
            "/world/default/model/x500_0/joint/link2_link3_joint/sensor/force_torque_sensor/forcetorque",
            qos_profile,
            std::bind(&ForceSensorReaderNode::forceSensorCallback, this, std::placeholders::_1));

        // Publisher for PX4 Load Cell data
        load_cell_publisher_ = this->create_publisher<px4_msgs::msg::LoadCellData>("/fmu/in/load_cell_data", 10);
    }

private:
    void forceSensorCallback(const geometry_msgs::msg::WrenchStamped::SharedPtr msg)
    {
        // Create LoadCellData message
        auto load_cell_msg = px4_msgs::msg::LoadCellData();

        // ✅ Use Gazebo's timestamp instead of ROS 2 system time
        load_cell_msg.timestamp = msg->header.stamp.sec * 1000000ULL + msg->header.stamp.nanosec / 1000;  // Convert to microseconds

        // Add force/torque values
        load_cell_msg.force_x = msg->wrench.force.x;
        load_cell_msg.force_y = msg->wrench.force.y;
        load_cell_msg.force_z = msg->wrench.force.z;
        load_cell_msg.torque_x = msg->wrench.torque.x;
        load_cell_msg.torque_y = msg->wrench.torque.y;
        load_cell_msg.torque_z = msg->wrench.torque.z;

        // ✅ Debugging: Print timestamp and force data
        RCLCPP_INFO(this->get_logger(), 
                    "Publishing LoadCellData: Time diff (µs): %lld, Fx=%.2f, Fy=%.2f, Fz=%.2f",
                    this->now().nanoseconds() / 1000 - load_cell_msg.timestamp,
                    load_cell_msg.force_x, load_cell_msg.force_y, load_cell_msg.force_z);

        // Publish the message
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
#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <px4_msgs/msg/load_cell_data.hpp>

class ForceSensorReaderNode : public rclcpp::Node
{
public:
    ForceSensorReaderNode() : Node("force_sensor_reader_node")
    {
        // Subscriber per leggere i dati dal sensore di forza di Gazebo
        force_sensor_subscriber_ = this->create_subscription<geometry_msgs::msg::WrenchStamped>(
            "/world/default/model/x500_vision_0/joint/link2_link3_joint/sensor/force_torque_sensor/forcetorque",
            1,
            std::bind(&ForceSensorReaderNode::forceSensorCallback, this, std::placeholders::_1));

        // Publisher per inviare i dati a PX4
        load_cell_publisher_ = this->create_publisher<px4_msgs::msg::LoadCellData>("/fmu/in/load_cell_data", 1);
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


*/


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
            "/world/default/model/x500_vision_0/link/arm_link3/sensor/contact_sensor/contact",
            10,
            std::bind(&ContactSensorReaderNode::contactSensorCallback, this, std::placeholders::_1));

        // Publisher per inviare i dati a PX4
        load_cell_publisher_ = this->create_publisher<px4_msgs::msg::LoadCellData>("/fmu/in/load_cell_data", 10);

        // Timer per inviare sempre dati anche se non ci sono contatti
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),  // Frequenza di pubblicazione (10 Hz)
            std::bind(&ContactSensorReaderNode::publishZeroIfNoContact, this));

        last_contact_time_ = this->now();
    }

private:
    rclcpp::Time last_contact_time_;
    rclcpp::TimerBase::SharedPtr timer_;

    void contactSensorCallback(const ros_gz_interfaces::msg::Contacts::SharedPtr msg)
    {
        auto load_cell_msg = px4_msgs::msg::LoadCellData();
        load_cell_msg.timestamp = this->now().nanoseconds() / 1000;  // Microsecondi

        if (msg->contacts.empty()) {
            RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 2000, 
                                 "Nessun contatto rilevato. Valori nulli saranno pubblicati automaticamente.");
        } else {
            // Prendiamo il primo contatto
            const auto& contact = msg->contacts[0];

            if (!contact.wrenches.empty()) {
                const auto& wrench = contact.wrenches[0].body_1_wrench.force;
                load_cell_msg.force_x = wrench.x;
                load_cell_msg.force_y = wrench.y;
                load_cell_msg.force_z = wrench.z;

                RCLCPP_INFO(this->get_logger(), "Publishing LoadCellData: Fx=%.2f, Fy=%.2f, Fz=%.2f",
                            load_cell_msg.force_x, load_cell_msg.force_y, load_cell_msg.force_z);

                last_contact_time_ = this->now();  // Aggiorna il tempo dell'ultimo contatto
            }
        }

        // Pubblica il messaggio per PX4
        load_cell_publisher_->publish(load_cell_msg);
    }

    void publishZeroIfNoContact()
    {
        rclcpp::Duration elapsed_time = this->now() - last_contact_time_;
        
        if (elapsed_time.seconds() > 0.2) {  // Se non ci sono contatti da più di 200ms
            auto load_cell_msg = px4_msgs::msg::LoadCellData();
            load_cell_msg.timestamp = this->now().nanoseconds() / 1000;
            load_cell_msg.force_x = 0.0;
            load_cell_msg.force_y = 0.0;
            load_cell_msg.force_z = 0.0;

            RCLCPP_DEBUG(this->get_logger(), "Nessun contatto recente, pubblicazione valori nulli.");

            load_cell_publisher_->publish(load_cell_msg);
        }
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

