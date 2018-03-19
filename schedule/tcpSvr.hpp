#ifndef __TCPSVR_HPP__
#define __TCPSVR_HPP__

#include <sys/socket.h>
#include <map>
#include "util/common.hpp"

using std::map;

typedef Node * (*registerFunc)(int fd);
typedef int (*dealFunc)(int fd);
typedef map<int, Node *> intNodeMap;

extern intNodeMap gNodeMap;

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
