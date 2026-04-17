#include "driver.hpp"
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/serial_port_base.hpp>
#include <iostream>
#include <string>
extern "C" {
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
}
using namespace boost::asio;
using namespace std;

Driver::Driver() {
  this->port.set_option(serial_port_base::baud_rate(115200));
  this->port.set_option(serial_port_base::character_size(8));
  this->port.set_option(
      serial_port_base::stop_bits(serial_port_base::stop_bits::one));
  this->port.set_option(
      serial_port_base::parity(serial_port_base::parity::none));
}

Driver::~Driver() {
  if (port.is_open()) {

    Close();
  }
}

void Driver::Close() {
  port.close(ec);
  if (ec) {
    std::cout << "Error closing serial" << ec.message() << std::endl;
  }
}

bool Driver::Open(std::string deviceName, int baud) {
  struct termios tty;
  return true;
}

bool Driver::isopen() { return port.is_open(); }

bool Driver::send(std::string input) {
  if (!port.is_open())
    return false;
  ssize_t wrtsize = write(port, buffer(input));
  return (wrtsize == input.size());
}

int Driver::receive(unsigned char *data, int len) {
  if (!port.is_open())
    return -1;
  return 1;
  // async_read_until(port, buffer());
  // return lenrcv;
}

bool Driver::bytes_received(int &bytelen) {
  if (!port.is_open())
    return false;
  return true;
}
