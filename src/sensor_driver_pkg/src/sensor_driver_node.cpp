#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/wrench_stamped.hpp>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <cstdio>

class SensorDriverNode : public rclcpp::Node {
public:
    SensorDriverNode() : Node("sensor_driver_node") {
        // Dichiarazione e lettura dei parametri
        //this->declare_parameter<std::string>("serial_port", "/dev/ttyACM0"); QUESTO È PER KOALA
	this->declare_parameter<std::string>("serial_port", "/dev/ttyUSB0"); //QUESTO È PER IL FENICOTTERO
        this->declare_parameter<int>("baud_rate", 115200);

        this->get_parameter("serial_port", device_name_);
        this->get_parameter("baud_rate", baud_rate_);

        // Configurazione della porta seriale
        configure_serial();
        if (serial_port_ < 0) {
            RCLCPP_ERROR(this->get_logger(), "Failed to configure serial port!");
            rclcpp::shutdown();
            return;
        }

        // Creazione del publisher
        wrench_pub_ = this->create_publisher<geometry_msgs::msg::WrenchStamped>("/fmu/in/load_cell_data", 10);

        // Creazione del timer per il ciclo di lettura
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(2),
            std::bind(&SensorDriverNode::read_and_publish, this));
    }

    ~SensorDriverNode() {
        if (serial_port_ > 0) close(serial_port_);
    }

private:
    void configure_serial() {
        serial_port_ = open(device_name_.c_str(), O_RDWR);
        if (serial_port_ < 0) {
            RCLCPP_ERROR(this->get_logger(), "Error opening serial port: %s", device_name_.c_str());
            return;
        }

        struct termios tty;
        if (tcgetattr(serial_port_, &tty) != 0) {
            RCLCPP_ERROR(this->get_logger(), "Error from tcgetattr");
            return;
        }

        // Configurazione dei parametri seriali
        tty.c_cflag &= ~PARENB;  // Disabilita il bit di parità
        tty.c_cflag &= ~CSTOPB;  // 1 bit di stop
        tty.c_cflag |= CS8;      // 8 bit per byte
        tty.c_cflag &= ~CRTSCTS; // Disabilita il controllo di flusso hardware
        tty.c_cflag |= CREAD | CLOCAL; // Abilita lettura e ignora le linee di controllo

        tty.c_lflag &= ~ICANON;  // Modalità non canonica
        tty.c_lflag &= ~ECHO;    // Disabilita echo
        tty.c_lflag &= ~ECHOE;
        tty.c_lflag &= ~ECHONL;
        tty.c_lflag &= ~ISIG;

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Disabilita controllo di flusso software
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
        tty.c_oflag &= ~OPOST;   // Disabilita interpretazione speciale dell'output
        tty.c_oflag &= ~ONLCR;

        tty.c_cc[VTIME] = 10;    // Timeout di 1 secondo (10 decisecondi)
        tty.c_cc[VMIN] = 0;      // Nessun byte minimo richiesto

        cfsetispeed(&tty, B115200);
        cfsetospeed(&tty, B115200);

        if (tcsetattr(serial_port_, TCSANOW, &tty) != 0) {
            RCLCPP_ERROR(this->get_logger(), "Error from tcsetattr");
        }
    }

    void read_and_publish() {
        uint8_t buffer[25];
        ssize_t n = read(serial_port_, &buffer[0], 1);

        if (n > 0 && buffer[0] == '$') {
            for (int i = 1; i < 25; i++) {
                n = read(serial_port_, &buffer[i], 1);
                if (n <= 0) {
                    RCLCPP_ERROR(this->get_logger(), "Failed to read byte %d", i);
                    return;
                }
            }

            float wrench[6];
            deserialize(buffer, wrench);

            /*// Debugging dei valori letti
            RCLCPP_INFO(this->get_logger(),
                        "Wrench: Force [%f, %f, %f], Torque [%f, %f, %f]",
                        wrench[0], wrench[1], wrench[2],
                        wrench[3], wrench[4], wrench[5]);*/

            // Popolazione del messaggio ROS
            auto wrench_msg = geometry_msgs::msg::WrenchStamped();
            wrench_msg.header.stamp = this->now();
            wrench_msg.header.frame_id = "";
            wrench_msg.wrench.force.x = wrench[0];
            wrench_msg.wrench.force.y = wrench[1];
            wrench_msg.wrench.force.z = wrench[2];
            wrench_msg.wrench.torque.x = wrench[3];
            wrench_msg.wrench.torque.y = wrench[4];
            wrench_msg.wrench.torque.z = wrench[5];

            wrench_pub_->publish(wrench_msg);
        } else if (n < 0) {
            RCLCPP_ERROR(this->get_logger(), "Error reading serial port: %s", strerror(errno));
        }
    }

    void deserialize(const uint8_t *buffer, float *wrench) {
        uint32_t temp =0;
    temp |= buffer[1]<<24;
    temp |= buffer[2]<<16;
    temp |= buffer[3]<<8;
    temp |= buffer[4];
    wrench[0] = *((float*)&temp);
    temp=0;
    temp |= buffer[5]<<24;
    temp |= buffer[6]<<16;
    temp |= buffer[7]<<8;
    temp |= buffer[8];
    wrench[1] = *((float*)&temp);
    temp=0;
    temp |= buffer[9]<<24;
    temp |= buffer[10]<<16;
    temp |= buffer[11]<<8;
    temp |= buffer[12];
    wrench[2] = *((float*)&temp);
    temp=0;
    temp |= buffer[13]<<24;
    temp |= buffer[14]<<16;
    temp |= buffer[15]<<8;
    temp |= buffer[16];
    wrench[3] = *((float*)&temp);
    temp=0;
    temp |= buffer[17]<<24;
    temp |= buffer[18]<<16;
    temp |= buffer[19]<<8;
    temp |= buffer[20];
    wrench[4] = *((float*)&temp);
    temp=0;
    temp |= buffer[21]<<24;
    temp |= buffer[22]<<16;
    temp |= buffer[23]<<8;
    temp |= buffer[24];
    wrench[5] = *((float*)&temp);
    }

    std::string device_name_;   // Nome del dispositivo seriale
    int baud_rate_;             // Baud rate configurato
    int serial_port_ = -1;      // File descriptor della porta seriale
    rclcpp::Publisher<geometry_msgs::msg::WrenchStamped>::SharedPtr wrench_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

// Funzione main
int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SensorDriverNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
