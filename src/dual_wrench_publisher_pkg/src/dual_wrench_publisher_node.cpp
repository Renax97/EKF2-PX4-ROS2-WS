/*APPLICAZIONE FORZA ISTANTANEA

#include <rclcpp/rclcpp.hpp>
#include <ros_gz_interfaces/msg/entity_wrench.hpp>
#include <px4_msgs/msg/load_cell_data.hpp>
#include <chrono>

using namespace std::chrono_literals;

class DualWrenchPublisherNode : public rclcpp::Node
{
public:
    DualWrenchPublisherNode() : Node("dual_wrench_publisher_node")
    {
        // Publisher per il topic EntityWrench
        entity_wrench_publisher_ = this->create_publisher<ros_gz_interfaces::msg::EntityWrench>("/world/default/wrench", 10);

        // Publisher per il topic LoadCellData
        load_cell_publisher_ = this->create_publisher<px4_msgs::msg::LoadCellData>("/fmu/in/load_cell_data", 10);

        // Timer per pubblicare dati ogni 10 secondi
        timer_ = this->create_wall_timer(10s, std::bind(&DualWrenchPublisherNode::publishData, this));
    }

private:
    void publishData()
    {
        // Creare e popolare il messaggio EntityWrench
        auto entity_wrench_msg = ros_gz_interfaces::msg::EntityWrench();
        entity_wrench_msg.entity.id = 66; // ID del base link = 28, ID di arm_link3 = 66 
        entity_wrench_msg.wrench.force.x = 0.0;
        entity_wrench_msg.wrench.force.y = 0.0;
        entity_wrench_msg.wrench.force.z = -500.0;
        entity_wrench_msg.wrench.torque.x = 0.0;
        entity_wrench_msg.wrench.torque.y = 0.0;
        entity_wrench_msg.wrench.torque.z = 0.0;

        // Pubblicare il messaggio EntityWrench
        RCLCPP_INFO(this->get_logger(), "Publishing EntityWrench: Fx=%.2f, Fy=%.2f, Fz=%.2f",
                    entity_wrench_msg.wrench.force.x, entity_wrench_msg.wrench.force.y, entity_wrench_msg.wrench.force.z);
        entity_wrench_publisher_->publish(entity_wrench_msg);

        // Creare e popolare il messaggio LoadCellData
        auto load_cell_msg = px4_msgs::msg::LoadCellData();
        load_cell_msg.timestamp = this->now().nanoseconds() / 1000; // Microsecondi
        load_cell_msg.force_x = entity_wrench_msg.wrench.force.x;
        load_cell_msg.force_y = entity_wrench_msg.wrench.force.y;
        load_cell_msg.force_z = entity_wrench_msg.wrench.force.z;
        load_cell_msg.torque_x = entity_wrench_msg.wrench.torque.x;
        load_cell_msg.torque_y = entity_wrench_msg.wrench.torque.y;
        load_cell_msg.torque_z = entity_wrench_msg.wrench.torque.z;

        // Pubblicare il messaggio LoadCellData
        RCLCPP_INFO(this->get_logger(), "Publishing LoadCellData: Fx=%.2f, Fy=%.2f, Fz=%.2f",
                    load_cell_msg.force_x, load_cell_msg.force_y, load_cell_msg.force_z);
        load_cell_publisher_->publish(load_cell_msg);
    }

    rclcpp::Publisher<ros_gz_interfaces::msg::EntityWrench>::SharedPtr entity_wrench_publisher_;
    rclcpp::Publisher<px4_msgs::msg::LoadCellData>::SharedPtr load_cell_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DualWrenchPublisherNode>());
    rclcpp::shutdown();
    return 0;
}

*/



/* APPLICAZIONE FORZA CON RAMPA
#include <rclcpp/rclcpp.hpp>
#include <ros_gz_interfaces/msg/entity_wrench.hpp>
#include <px4_msgs/msg/load_cell_data.hpp>
#include <chrono>

using namespace std::chrono_literals;

class DualWrenchPublisherNode : public rclcpp::Node
{
public:
    DualWrenchPublisherNode() : Node("dual_wrench_publisher_node"), applying_force_(false)
    {
        // Publisher per il topic EntityWrench
        entity_wrench_publisher_ = this->create_publisher<ros_gz_interfaces::msg::EntityWrench>("/world/default/wrench", 10);

        // Publisher per il topic LoadCellData
        load_cell_publisher_ = this->create_publisher<px4_msgs::msg::LoadCellData>("/fmu/in/load_cell_data", 10);

        // Timer per avviare la rampa di forza ogni 10 secondi
        trigger_timer_ = this->create_wall_timer(10s, std::bind(&DualWrenchPublisherNode::startForceApplication, this));

        // Timer per pubblicare la forza durante la rampa (ogni 10ms)
        publish_timer_ = this->create_wall_timer(10ms, std::bind(&DualWrenchPublisherNode::publishRampForce, this));

        // Parametri della rampa
        ramp_duration_ = 1.0;   // Durata della rampa in secondi
        max_force_z_ = -10.0;  // Forza massima lungo Z
    }

private:
    // Funzione per avviare la rampa di forza
    void startForceApplication()
    {
        applying_force_ = true;
        start_time_ = this->now();
        RCLCPP_INFO(this->get_logger(), "Inizio applicazione forza.");
    }

    // Funzione per pubblicare la forza crescente
    void publishRampForce()
    {
        if (applying_force_) {
            rclcpp::Time now = this->now();
            double elapsed_time = (now - start_time_).seconds();

            double force_z = 0.0;

            if (elapsed_time <= ramp_duration_) {
                // Incremento lineare della forza
                force_z = max_force_z_ * (elapsed_time / ramp_duration_);
            } else {
                // Dopo la rampa, azzera la forza
                force_z = 0.0;
                applying_force_ = false;
                RCLCPP_INFO(this->get_logger(), "Fine applicazione forza.");
            }

            // Creazione e pubblicazione del messaggio EntityWrench
            auto entity_wrench_msg = ros_gz_interfaces::msg::EntityWrench();
            entity_wrench_msg.entity.id = 66;  // ID del link su cui applicare la forza
            entity_wrench_msg.wrench.force.x = 0.0;
            entity_wrench_msg.wrench.force.y = 0.0;
            entity_wrench_msg.wrench.force.z = force_z;
            entity_wrench_msg.wrench.torque.x = 0.0;
            entity_wrench_msg.wrench.torque.y = 0.0;
            entity_wrench_msg.wrench.torque.z = 0.0;

            RCLCPP_INFO(this->get_logger(), "[EntityWrench] Fx=%.2f, Fy=%.2f, Fz=%.2f",
                        entity_wrench_msg.wrench.force.x, entity_wrench_msg.wrench.force.y, entity_wrench_msg.wrench.force.z);

            entity_wrench_publisher_->publish(entity_wrench_msg);

            // Creazione e pubblicazione del messaggio LoadCellData
            auto load_cell_msg = px4_msgs::msg::LoadCellData();
            load_cell_msg.timestamp = this->now().nanoseconds() / 1000;  // Microsecondi
            load_cell_msg.force_x = entity_wrench_msg.wrench.force.x;
            load_cell_msg.force_y = entity_wrench_msg.wrench.force.y;
            load_cell_msg.force_z = entity_wrench_msg.wrench.force.z;
            load_cell_msg.torque_x = entity_wrench_msg.wrench.torque.x;
            load_cell_msg.torque_y = entity_wrench_msg.wrench.torque.y;
            load_cell_msg.torque_z = entity_wrench_msg.wrench.torque.z;

            RCLCPP_INFO(this->get_logger(), "[LoadCellData] Fx=%.2f, Fy=%.2f, Fz=%.2f",
                        load_cell_msg.force_x, load_cell_msg.force_y, load_cell_msg.force_z);

            load_cell_publisher_->publish(load_cell_msg);
        }
    }

    // Publisher
    rclcpp::Publisher<ros_gz_interfaces::msg::EntityWrench>::SharedPtr entity_wrench_publisher_;
    rclcpp::Publisher<px4_msgs::msg::LoadCellData>::SharedPtr load_cell_publisher_;

    // Timer per avviare la rampa di forza ogni 10 secondi
    rclcpp::TimerBase::SharedPtr trigger_timer_;

    // Timer per pubblicare la forza durante la rampa
    rclcpp::TimerBase::SharedPtr publish_timer_;

    // Variabili di stato
    bool applying_force_;
    rclcpp::Time start_time_;

    // Parametri della rampa
    double ramp_duration_;  // Durata della rampa in secondi
    double max_force_z_;    // Forza massima lungo Z
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DualWrenchPublisherNode>());
    rclcpp::shutdown();
    return 0;
}
*/


//APPLICAZIONE FORZA CON RAMPA CRESCENTE E DECRESCENTE


#include <rclcpp/rclcpp.hpp>
#include <ros_gz_interfaces/msg/entity_wrench.hpp>
#include <px4_msgs/msg/load_cell_data.hpp>
#include <chrono>

using namespace std::chrono_literals;

class DualWrenchPublisherNode : public rclcpp::Node
{
public:
    DualWrenchPublisherNode() : Node("dual_wrench_publisher_node"), applying_force_(false)
    {
        // Publisher per il topic EntityWrench
        entity_wrench_publisher_ = this->create_publisher<ros_gz_interfaces::msg::EntityWrench>("/world/default/wrench", 10);

        // Publisher per il topic LoadCellData
        load_cell_publisher_ = this->create_publisher<px4_msgs::msg::LoadCellData>("/fmu/in/load_cell_data", 10);

        // Timer per avviare la rampa di forza ogni 10 secondi
        trigger_timer_ = this->create_wall_timer(10s, std::bind(&DualWrenchPublisherNode::startForceApplication, this));

        // Timer per pubblicare la forza durante la rampa (ogni 10ms)
        publish_timer_ = this->create_wall_timer(10ms, std::bind(&DualWrenchPublisherNode::publishRampForce, this));

        // Parametri della rampa
        total_duration_ = 3.0;   // Durata totale di applicazione (secondi)
        max_force_z_ = -10.0;    // Forza massima lungo Z
    }

private:
    // Funzione per avviare la sequenza di applicazione della forza
    void startForceApplication()
    {
        applying_force_ = true;
        start_time_ = this->now();
        RCLCPP_INFO(this->get_logger(), "Inizio applicazione forza.");
    }

    // Funzione per gestire le tre fasi di applicazione della forza
    void publishRampForce()
    {
        if (applying_force_) {
            rclcpp::Time now = this->now();
            double elapsed_time = (now - start_time_).seconds();
            double force_z = 0.0;

            // Suddivisione in tre fasi
            double ramp_phase_duration = total_duration_ / 3.0;
            double hold_phase_duration = total_duration_ / 3.0;
            double release_phase_duration = total_duration_ / 3.0;

            if (elapsed_time <= ramp_phase_duration) {
                // Fase 1: Rampa crescente
                force_z = max_force_z_ * (elapsed_time / ramp_phase_duration);
            } 
            else if (elapsed_time <= (ramp_phase_duration + hold_phase_duration)) {
                // Fase 2: Forza costante
                force_z = max_force_z_;
            } 
            else if (elapsed_time <= total_duration_) {
                // Fase 3: Rampa decrescente
                double dec_time = elapsed_time - (ramp_phase_duration + hold_phase_duration);
                force_z = max_force_z_ * (1 - (dec_time / release_phase_duration));
            } 
            else {
                // Fine del ciclo
                force_z = 0.0;
                applying_force_ = false;
                RCLCPP_INFO(this->get_logger(), "Fine applicazione forza.");
            }

            // Creazione e pubblicazione del messaggio EntityWrench
            auto entity_wrench_msg = ros_gz_interfaces::msg::EntityWrench();
            entity_wrench_msg.entity.id = 66;  // ID del link su cui applicare la forza
            entity_wrench_msg.wrench.force.x = 0.0;
            entity_wrench_msg.wrench.force.y = 0.0;
            entity_wrench_msg.wrench.force.z = force_z;
            entity_wrench_msg.wrench.torque.x = 0.0;
            entity_wrench_msg.wrench.torque.y = 0.0;
            entity_wrench_msg.wrench.torque.z = 0.0;

            RCLCPP_INFO(this->get_logger(), "[EntityWrench] Fx=%.2f, Fy=%.2f, Fz=%.2f",
                        entity_wrench_msg.wrench.force.x, entity_wrench_msg.wrench.force.y, entity_wrench_msg.wrench.force.z);

            entity_wrench_publisher_->publish(entity_wrench_msg);

            // Creazione e pubblicazione del messaggio LoadCellData
            auto load_cell_msg = px4_msgs::msg::LoadCellData();
            load_cell_msg.timestamp = this->now().nanoseconds() / 1000;  // Microsecondi
            load_cell_msg.force_x = entity_wrench_msg.wrench.force.x;
            load_cell_msg.force_y = entity_wrench_msg.wrench.force.y;
            load_cell_msg.force_z = entity_wrench_msg.wrench.force.z;
            load_cell_msg.torque_x = entity_wrench_msg.wrench.torque.x;
            load_cell_msg.torque_y = entity_wrench_msg.wrench.torque.y;
            load_cell_msg.torque_z = entity_wrench_msg.wrench.torque.z;

            RCLCPP_INFO(this->get_logger(), "[LoadCellData] Fx=%.2f, Fy=%.2f, Fz=%.2f",
                        load_cell_msg.force_x, load_cell_msg.force_y, load_cell_msg.force_z);

            load_cell_publisher_->publish(load_cell_msg);
        }
    }

    // Publisher
    rclcpp::Publisher<ros_gz_interfaces::msg::EntityWrench>::SharedPtr entity_wrench_publisher_;
    rclcpp::Publisher<px4_msgs::msg::LoadCellData>::SharedPtr load_cell_publisher_;

    // Timer per avviare la rampa ogni 10 secondi
    rclcpp::TimerBase::SharedPtr trigger_timer_;

    // Timer per pubblicare la forza durante la rampa
    rclcpp::TimerBase::SharedPtr publish_timer_;

    // Variabili di stato
    bool applying_force_;
    rclcpp::Time start_time_;

    // Parametri della rampa
    double total_duration_;  // Durata totale dell'applicazione (secondi)
    double max_force_z_;     // Forza massima lungo Z
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DualWrenchPublisherNode>());
    rclcpp::shutdown();
    return 0;
}




