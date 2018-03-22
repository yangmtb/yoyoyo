#ifndef __TCPCLT_HPP__
#define __TCPCLT_HPP__

#include "util/common.hpp"

typedef int (*handFunc)(int fd);
typedef int (*sendFunc)(int fd);

class TcpClient {
public:
  TcpClient(string addr, int port, handFunc hand = nullptr, sendFunc sed = nullptr);
  ~TcpClient();

  void run();

private:
  int mFd;
  handFunc mHand; // handshark
  sendFunc mSend; // sendnodeinfo
};

#endif // __TCPCLT_HPP__
