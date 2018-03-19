#ifndef __TCPCLT_HPP__
#define __TCPCLT_HPP__

#include "util/common.hpp"

typedef Node * (*handFunc)(int fd);

class TcpClient {
public:
  TcpClient(string addr, int port, handFunc hand = nullptr);
  ~TcpClient();

  void run();

private:
  int mFd;
  handFunc mHand; // handshark
};

#endif // __TCPCLT_HPP__
