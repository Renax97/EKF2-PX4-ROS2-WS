#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <sstream>

class ForceReaderNode : public rclcpp::Node {
public:
    ForceReaderNode() : Node("force_reader_node") {
        // Dichiarazione e lettura dei parametri
        this->declare_parameter<std::string>("serial_port", "/dev/ttyUSB0");
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
        force_pub_ = this->create_publisher<std_msgs::msg::Float64>("load_cell/force_z", 10);

        // Creazione del timer per il ciclo di lettura
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&ForceReaderNode::read_and_publish, this));
    }

    ~ForceReaderNode() {
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
        uint8_t buffer[256];
        ssize_t n = read(serial_port_, buffer, sizeof(buffer)); // Legge fino a 256 byte

        if (n > 0) {
            // Converte i dati grezzi in una stringa
            std::string data(reinterpret_cast<const char*>(buffer), n);
            RCLCPP_INFO(this->get_logger(), "Dati grezzi ricevuti: '%s'", data.c_str());

            // Cerca il prefisso "val: " nella stringa
            size_t pos = data.find("val: ");
            if (pos != std::string::npos) {
                try {
                    // Estrai il valore numerico dopo "val: "
                    std::string value_str = data.substr(pos + 5); // Salta "val: "
                    double value = std::stod(value_str);

                    // Pubblica il valore
                    auto force_msg = std_msgs::msg::Float64();
                    force_msg.data = value;
                    force_pub_->publish(force_msg);

                    RCLCPP_INFO(this->get_logger(), "Valore parsato e pubblicato: %f", value);
                } catch (const std::exception& e) {
                    RCLCPP_ERROR(this->get_logger(), "Errore nel parsing del valore numerico: '%s', errore: %s", data.c_str(), e.what());
                }
            } else {
                RCLCPP_ERROR(this->get_logger(), "Formato stringa non riconosciuto: '%s'", data.c_str());
            }
        } else if (n < 0) {
            RCLCPP_ERROR(this->get_logger(), "Errore nella lettura dalla porta seriale: %s", strerror(errno));
        }
    }

    std::string device_name_;   // Nome del dispositivo seriale
    int baud_rate_;             // Baud rate configurato
    int serial_port_ = -1;      // File descriptor della porta seriale
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr force_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

// Funzione main
int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ForceReaderNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}


