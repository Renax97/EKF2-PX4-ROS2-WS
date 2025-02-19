/* APPLICAZIONE FORZA ISTANTANEA

#include <rclcpp/rclcpp.hpp>
#include <ros_gz_interfaces/msg/entity_wrench.hpp>

class ForcePublisherNode : public rclcpp::Node
{
public:
  ForcePublisherNode()
      : Node("force_publisher_node"), simulated_time_(0)
  {
    // Verifica se il parametro 'use_sim_time' è già dichiarato
    if (!this->has_parameter("use_sim_time")) {
        this->declare_parameter("use_sim_time", true);
    }

    // Crea un publisher per il topic
    publisher_ = this->create_publisher<ros_gz_interfaces::msg::EntityWrench>("/world/default/wrench", 10);

    // Timer per inviare il messaggio periodicamente
    timer_ = this->create_wall_timer(std::chrono::seconds(10), std::bind(&ForcePublisherNode::publish_force, this));
  }

private:
  void publish_force()
  {
    auto msg = ros_gz_interfaces::msg::EntityWrench();

    // Imposta il timestamp corrente sincronizzato
    auto now = this->now();
    msg.header.stamp.sec = now.seconds();
    msg.header.stamp.nanosec = now.nanoseconds() - (msg.header.stamp.sec * 1'000'000'000);

    // Configura il messaggio
    msg.entity.id = 66; // ID dell'entità
    msg.wrench.force.x = 0.0;
    msg.wrench.force.y = 0.0;
    msg.wrench.force.z = -500.0;
    msg.wrench.torque.x = 0.0;
    msg.wrench.torque.y = 0.0;
    msg.wrench.torque.z = 0.0;

    // Log con timestamp
    RCLCPP_INFO(this->get_logger(), "Publishing force at time [%d.%d]",
                msg.header.stamp.sec, msg.header.stamp.nanosec);

    // Pubblica il messaggio
    publisher_->publish(msg);
  }

  rclcpp::Publisher<ros_gz_interfaces::msg::EntityWrench>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int simulated_time_; // Contatore per il tempo simulato
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ForcePublisherNode>());
  rclcpp::shutdown();
  return 0;
}

*/



/* APPLICAZIONE FORZA CON RAMPA
#include <rclcpp/rclcpp.hpp>
#include <ros_gz_interfaces/msg/entity_wrench.hpp>
#include <chrono>

using namespace std::chrono_literals;

class ForcePublisherNode : public rclcpp::Node
{
public:
  ForcePublisherNode()
      : Node("force_publisher_node"), applying_force_(false)
  {
    // Verifica se il parametro 'use_sim_time' è già dichiarato
    if (!this->has_parameter("use_sim_time")) {
      this->declare_parameter("use_sim_time", true);
    }

    // Publisher per il topic /world/default/wrench
    publisher_ = this->create_publisher<ros_gz_interfaces::msg::EntityWrench>("/world/default/wrench", 10);

    // Timer per attivare la forza ogni 10 secondi
    trigger_timer_ = this->create_wall_timer(10s, std::bind(&ForcePublisherNode::start_force_application, this));

    // Timer per pubblicare la forza durante la rampa (ogni 10ms)
    publish_timer_ = this->create_wall_timer(10ms, std::bind(&ForcePublisherNode::publish_ramp_force, this));

    // Parametri della rampa
    ramp_duration_ = 1.0;    // Durata della rampa in secondi
    max_force_z_ = -10.0;   // Forza massima lungo Z
  }

private:
  // Funzione che avvia la rampa di forza
  void start_force_application()
  {
    applying_force_ = true;
    start_time_ = this->now();
    RCLCPP_INFO(this->get_logger(), "Inizio applicazione forza.");
  }

  // Funzione che gestisce la rampa di forza
  void publish_ramp_force()
  {
    if (applying_force_) {
      auto now = this->now();
      double elapsed_time = (now - start_time_).seconds();

      double force_z = 0.0;

      if (elapsed_time <= ramp_duration_) {
        // Applica una forza crescente (rampa)
        force_z = max_force_z_ * (elapsed_time / ramp_duration_);
      } else {
        // Dopo 1 secondo, azzera la forza
        force_z = 0.0;
        applying_force_ = false;
        RCLCPP_INFO(this->get_logger(), "Fine applicazione forza.");
      }

      // Creazione del messaggio
      auto msg = ros_gz_interfaces::msg::EntityWrench();

      // Timestamp corrente
      msg.header.stamp.sec = now.seconds();
      msg.header.stamp.nanosec = now.nanoseconds() - (msg.header.stamp.sec * 1'000'000'000);

      // Configurazione del messaggio di forza
      msg.entity.id = 66;  // ID dell'entità
      msg.wrench.force.x = 0.0;
      msg.wrench.force.y = 0.0;
      msg.wrench.force.z = force_z;
      msg.wrench.torque.x = 0.0;
      msg.wrench.torque.y = 0.0;
      msg.wrench.torque.z = 0.0;

      // Log del valore della forza applicata
      RCLCPP_INFO(this->get_logger(), "Publishing ramp force: Fz=%.2f", msg.wrench.force.z);

      // Pubblica il messaggio
      publisher_->publish(msg);
    }
  }

  // Publisher per la forza
  rclcpp::Publisher<ros_gz_interfaces::msg::EntityWrench>::SharedPtr publisher_;

  // Timer per avviare la rampa ogni 10 secondi
  rclcpp::TimerBase::SharedPtr trigger_timer_;

  // Timer per pubblicare la forza durante la rampa
  rclcpp::TimerBase::SharedPtr publish_timer_;

  // Variabili di stato
  bool applying_force_;
  rclcpp::Time start_time_;

  // Parametri della rampa
  double ramp_duration_;
  double max_force_z_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ForcePublisherNode>());
  rclcpp::shutdown();
  return 0;
}
*/


// APPLICAZIONE FORZA CON RAMPA CRESCENTE E DECRESCENTE

/*
#include <rclcpp/rclcpp.hpp>
#include <ros_gz_interfaces/msg/entity_wrench.hpp>
#include <chrono>

using namespace std::chrono_literals;

class ForcePublisherNode : public rclcpp::Node
{
public:
  ForcePublisherNode()
      : Node("force_publisher_node"), applying_force_(false)
  {
    // Verifica se il parametro 'use_sim_time' è già dichiarato
    if (!this->has_parameter("use_sim_time")) {
      this->declare_parameter("use_sim_time", true);
    }

    // Publisher per il topic /world/default/wrench
    publisher_ = this->create_publisher<ros_gz_interfaces::msg::EntityWrench>("/world/default/wrench", 10);

    // Timer per avviare la forza ogni 10 secondi
    trigger_timer_ = this->create_wall_timer(10s, std::bind(&ForcePublisherNode::start_force_application, this));

    // Timer per pubblicare la forza durante la rampa (ogni 10ms)
    publish_timer_ = this->create_wall_timer(4ms, std::bind(&ForcePublisherNode::publish_ramp_force, this));

    // Parametri configurabili
    total_duration_ = 0.1;   // Durata totale del ciclo (secondi)
    max_force_z_ = -10.0;    // Forza massima lungo Z
  }

private:
  // Avvia la sequenza di applicazione della forza
  void start_force_application()
  {
    applying_force_ = true;
    start_time_ = this->now();
    RCLCPP_INFO(this->get_logger(), "Inizio applicazione forza.");
  }

  // Gestisce le tre fasi della forza
  void publish_ramp_force()
  {
    if (applying_force_) {
      auto now = this->now();
      double elapsed_time = (now - start_time_).seconds();
      double force_z = 0.0;

      // Suddivisione delle fasi
      double ramp_phase_duration = total_duration_ / 3.0;  // 1/3 rampa crescente
      double hold_phase_duration = total_duration_ / 3.0;  // 1/3 forza costante
      double release_phase_duration = total_duration_ / 3.0;  // 1/3 rampa decrescente

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

      // Creazione e pubblicazione del messaggio
      auto msg = ros_gz_interfaces::msg::EntityWrench();
      msg.header.stamp.sec = now.seconds();
      msg.header.stamp.nanosec = now.nanoseconds() - (msg.header.stamp.sec * 1'000'000'000);
      msg.entity.id = 66;  // ID dell'entità
      msg.wrench.force.x = -1.0;
      msg.wrench.force.y = -2.0;
      msg.wrench.force.z = force_z;
      msg.wrench.torque.x = 0.0;
      msg.wrench.torque.y = 0.0;
      msg.wrench.torque.z = 0.0;

      // Log della forza pubblicata
      RCLCPP_INFO(this->get_logger(), "Publishing force: Fz=%.2f", msg.wrench.force.z);

      publisher_->publish(msg);
    }
  }

  // Publisher per la forza
  rclcpp::Publisher<ros_gz_interfaces::msg::EntityWrench>::SharedPtr publisher_;

  // Timer per avviare la forza ogni 10 secondi
  rclcpp::TimerBase::SharedPtr trigger_timer_;

  // Timer per pubblicare la forza durante la rampa
  rclcpp::TimerBase::SharedPtr publish_timer_;

  // Variabili di stato
  bool applying_force_;
  rclcpp::Time start_time_;

  // Parametri configurabili
  double total_duration_;  // Durata totale dell'applicazione (secondi)
  double max_force_z_;     // Forza massima lungo Z
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ForcePublisherNode>());
  rclcpp::shutdown();
  return 0;
}
*/


//APPLICAZIONE FORZA COSTANTE 




#include <rclcpp/rclcpp.hpp>
#include <ros_gz_interfaces/msg/entity_wrench.hpp>
#include <chrono>

using namespace std::chrono_literals;

class ForcePublisherNode : public rclcpp::Node
{
public:
  ForcePublisherNode()
      : Node("force_publisher_node"), applying_force_(false)
  {
    // Verifica se il parametro 'use_sim_time' è già dichiarato
    if (!this->has_parameter("use_sim_time")) {
      this->declare_parameter("use_sim_time", true);
    }

    // Publisher per il topic /world/default/wrench
    publisher_ = this->create_publisher<ros_gz_interfaces::msg::EntityWrench>("/world/default/wrench", 10);

    // Timer per avviare la forza ogni 10 secondi
    trigger_timer_ = this->create_wall_timer(10s, std::bind(&ForcePublisherNode::start_force_application, this));

    // Timer per pubblicare la forza costante (ogni 10ms)
    publish_timer_ = this->create_wall_timer(4ms, std::bind(&ForcePublisherNode::publish_constant_force, this));

    // Parametri configurabili
    duration_ = 0.4;       // Durata dell'applicazione della forza (secondi)
    constant_force_z_ = -9.5; // Forza costante lungo Z
  }

private:
  // Avvia la sequenza di applicazione della forza
  void start_force_application()
  {
    applying_force_ = true;
    start_time_ = this->now();
    RCLCPP_INFO(this->get_logger(), "Inizio applicazione forza per %.2f secondi.", duration_);
  }

  // Pubblica la forza costante per la durata specificata
  void publish_constant_force()
  {
    if (applying_force_) {
      auto now = this->now();
      double elapsed_time = (now - start_time_).seconds();

      if (elapsed_time <= duration_) {
        // Creazione del messaggio
        auto msg = ros_gz_interfaces::msg::EntityWrench();
        msg.header.stamp.sec = now.seconds();
        msg.header.stamp.nanosec = now.nanoseconds() - (msg.header.stamp.sec * 1'000'000'000);
        msg.entity.id = 74; // ID dell'entità
        //ID VECCHIO 484
        msg.wrench.force.x = 0.0;
        msg.wrench.force.y = 0.0;
        msg.wrench.force.z = constant_force_z_;
        msg.wrench.torque.x = 0.0;
        msg.wrench.torque.y = 0.0;
        msg.wrench.torque.z = 0.0;

        // Log della forza pubblicata
        RCLCPP_INFO(this->get_logger(), "Publishing constant force: Fz=%.2f", msg.wrench.force.z);

        // Pubblicazione del messaggio
        publisher_->publish(msg);
      } else {
        // Fine del ciclo di applicazione della forza
        applying_force_ = false;
        RCLCPP_INFO(this->get_logger(), "Fine applicazione forza.");
      }
    }
  }

  // Publisher per la forza
  rclcpp::Publisher<ros_gz_interfaces::msg::EntityWrench>::SharedPtr publisher_;

  // Timer per avviare la forza ogni 10 secondi
  rclcpp::TimerBase::SharedPtr trigger_timer_;

  // Timer per pubblicare la forza costante
  rclcpp::TimerBase::SharedPtr publish_timer_;

  // Variabili di stato
  bool applying_force_;
  rclcpp::Time start_time_;

  // Parametri configurabili
  double duration_;          // Durata dell'applicazione della forza (secondi)
  double constant_force_z_;  // Forza costante lungo Z
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ForcePublisherNode>());
  rclcpp::shutdown();
  return 0;
}





/*
#include <rclcpp/rclcpp.hpp>
#include <ros_gz_interfaces/msg/entity_wrench.hpp>
#include <px4_msgs/msg/vehicle_odometry.hpp>
#include <chrono>

using namespace std::chrono_literals;

class ForcePublisherNode : public rclcpp::Node
{
public:
  ForcePublisherNode()
      : Node("force_publisher_node"), force_applied_(false)
  {
    // Dichiarazione parametro 'use_sim_time'
    if (!this->has_parameter("use_sim_time")) {
      this->declare_parameter("use_sim_time", true);
    }

    // Publisher per il topic /world/default/wrench
    publisher_ = this->create_publisher<ros_gz_interfaces::msg::EntityWrench>("/world/default/wrench", 10);

    // Subscriber per la posizione del drone
    subscription_ = this->create_subscription<px4_msgs::msg::VehicleOdometry>(
        "/fmu/out/vehicle_odometry", rclcpp::SensorDataQoS(),
        std::bind(&ForcePublisherNode::odometry_callback, this, std::placeholders::_1));

    // Parametri configurabili
    total_duration_ = 0.4; // Durata totale della forza
    max_force_z_ = 3.0;  // Forza massima lungo Z
  }

private:
  // Callback per leggere la posizione Z dal topic /fmu/out/vehicle_odometry
  void odometry_callback(const px4_msgs::msg::VehicleOdometry::SharedPtr msg)
  {
    if (force_applied_) return; // Se la forza è già stata applicata, ignora i messaggi

    double position_z = msg->position[2];

    if (position_z <= -1.8)  // Attiva solo alla prima lettura di z == -1
    {
      force_applied_ = true; // Blocca future applicazioni
      RCLCPP_INFO(this->get_logger(), "Inizio applicazione forza (position.z = -1).");
      apply_force_once();
    }
  }

  // Applica la forza una sola volta
  void apply_force_once()
  {
    auto now = this->now();
    auto msg = ros_gz_interfaces::msg::EntityWrench();
    
    msg.header.stamp.sec = now.seconds();
    msg.header.stamp.nanosec = now.nanoseconds() - (msg.header.stamp.sec * 1'000'000'000);
    msg.entity.id = 74; // ID dell'entità
    
    msg.wrench.force.x = 0.1;
    msg.wrench.force.y = 0.05;
    msg.wrench.force.z = max_force_z_; // Applica la forza massima subito
    msg.wrench.torque.x = 0.0;
    msg.wrench.torque.y = 0.0;
    msg.wrench.torque.z = 0.0;

    publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "Forza applicata: Fz=%.2f", msg.wrench.force.z);
  }

  // Publisher per la forza
  rclcpp::Publisher<ros_gz_interfaces::msg::EntityWrench>::SharedPtr publisher_;

  // Subscriber per la posizione del drone
  rclcpp::Subscription<px4_msgs::msg::VehicleOdometry>::SharedPtr subscription_;

  // Variabili di stato
  bool force_applied_;  // Indica se la forza è già stata applicata

  // Parametri configurabili
  double total_duration_; // Durata totale dell'applicazione (secondi)
  double max_force_z_;    // Forza massima lungo Z
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ForcePublisherNode>());
  rclcpp::shutdown();
  return 0;
}

*/


