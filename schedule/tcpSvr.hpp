#ifndef __TCPSVR_HPP__
#define __TCPSVR_HPP__

#include <sys/socket.h>
#include "util/common.hpp"

typedef Node * (*registerFunc)(int fd);
typedef int (*dealFunc)(int fd);

class TcpServer {
public:
  TcpServer(int port = 0, registerFunc rgst = nullptr, dealFunc deal = nullptr, int listenCount = 16);
  ~TcpServer();

  void run();

private:
  int epollLoop(int max, int timeout);
  void ctlEvent(int fd, bool flag);

private:
  int mEpollFd;
  int mListenFd;
  registerFunc mRegister;
  dealFunc mDeal;
};

#endif //__TCPSVR_HPP__
