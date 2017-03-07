#include <boost/asio.hpp>
#include <iostream>
int main(int argc, char** argv) {
  try {
    boost::system::error_code ec;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::iterator  endpoint_iterator = resolver.resolve({argv[1],"http"},ec);
    //  boost::asio::ip::tcp::resolver::iterator  endpoint_iterator = resolver.resolve({host_uri,host_port},ec);
  if (ec) throw boost::system::system_error(ec);
  boost::asio::ip::tcp::socket  socket(io_service);
  boost::asio::connect(socket, endpoint_iterator,ec);
  socket.set_option(boost::asio::ip::tcp::no_delay(true));
  if (ec) throw boost::system::system_error(ec);
  boost::asio::streambuf requestBuf;
  std::ostream request_stream(&requestBuf);

  //request_stream << "garbageljkafsdhladsfjio\r\n\r\n";

  request_stream << "GET / HTTP/1.0\r\n";
  request_stream << "Host: " << argv[1] << "\r\n";
  request_stream << "Accept: */*\r\n";
  request_stream << "Connection: close\r\n\r\n";


  //  request_stream << request.ToString();
  // Send the request.
  std::cout << "Sending " << &requestBuf << std::endl;
  boost::asio::write(socket, requestBuf,ec);
  if (ec) throw boost::system::system_error(ec);
  std::cout<<"Reading"<<std::endl;
  boost::asio::streambuf response_stream_buf;
  //boost::asio::read_until(socket, response_stream_buf, "\r\n", ec);
  boost::asio::read(socket,response_stream_buf,boost::asio::transfer_at_least(1),ec);
  std::cout<<"Done Reading " << &response_stream_buf<<std::endl;
  } catch (std::exception& e) {
    std::cout << "Exception: " << e.what() << std::endl;
  }
  return 0;
  }
