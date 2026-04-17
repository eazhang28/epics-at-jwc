#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>
#include <string>

using namespace boost::asio;
using namespace boost::system;

class Driver : serial_port_base {
private:
  io_context io;
  serial_port port = serial_port(io, "/dev/ttyUSB0");
  error_code ec;

public:
  Driver();
  ~Driver();

  bool send(std::string value);

  int receive(unsigned char *data, int len);
  bool isopen();
  void Close();
  bool Open(std::string deviceName, int baud);
  bool bytes_received(int &bytelen);
};

#endif
