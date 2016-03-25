//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <server/server.hpp>

int main(int argc, char* argv[])
{
  const std::string ip = "0.0.0.0";
  const std::string port = "8000";
  const std::string path = "./";

  std::cout << "Serving HTTP on " << ip << " port " << port << " ..." << std::endl;

  http::server::server s(ip, port, path);
  s.run();

  return 0;
}
