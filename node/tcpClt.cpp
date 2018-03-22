#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "util/err.hpp"
#include "tcpClt.hpp"

TcpClient::TcpClient(string addr, int port, handFunc hand, sendFunc sed):
  mFd(0), mHand(hand), mSend(sed)
{
  mFd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == mFd) {
    errExit("socket()", -1);
  }
  struct sockaddr_in loc;
  memset(&loc, 0, sizeof(loc));
  loc.sin_family = AF_INET;
  loc.sin_addr.s_addr = htons(INADDR_ANY);
  loc.sin_port = htons(0);
  if (-1 == bind(mFd, (struct sockaddr *)&loc, sizeof(struct sockaddr))) {
    errExit("bind()", -1);
  }
  struct sockaddr_in svr;
  memset(&svr, 0, sizeof(svr));
  svr.sin_family = AF_INET;
  svr.sin_addr.s_addr = inet_addr(addr.c_str());
  svr.sin_port = htons(port);
  if (-1 == connect(mFd, (struct sockaddr *)&svr, sizeof(svr))) {
    printf("err:%s\n", strerror(errno));
    errExit("connect()", -1);
  }
}

TcpClient::~TcpClient()
{
  ;
}

void TcpClient::run()
{
  if (-1 == mHand(mFd)) {
    printf("handshark err\n");
    return;
  }
  printf("handshark ok\n");
  if (-1 == mSend(mFd)) {
    printf("sendnodeinfo err\n");
    return;
  }
  printf("sendnodeinfo ok\n");
}
