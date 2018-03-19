#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "tcpSvr.hpp"
#include "util/err.hpp"

TcpServer::TcpServer(int port, registerFunc rgst, dealFunc deal, int listenCount):
  mRegister(rgst), mDeal(deal), mEpollFd(0), mListenFd(0)
{
  mListenFd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == mListenFd) {
    errExit("socket()", -1);
  }
  struct sockaddr_in svr;
  memset(&svr, 0, sizeof(svr));
  svr.sin_family = AF_INET;
  //svr.sin_addr.s_addr = inet_addr(addr.c_str());
  svr.sin_addr.s_addr = htonl(INADDR_ANY);
  svr.sin_port = htons(port);
  if (-1 == bind(mListenFd, (struct sockaddr *)(&svr), sizeof(struct sockaddr))) {
    errExit("bind()", -1);
  }
  if (-1 == listen(mListenFd, listenCount)) {
    errExit("listen()", -1);
  }
  mEpollFd = epoll_create1(0);
  if (-1 == mEpollFd) {
    errExit("epoll_crate1()", -1);
  }
  ctlEvent(mListenFd, true);
}

TcpServer::~TcpServer()
{
  ;
}

int TcpServer::epollLoop(int max, int timeout)
{
  struct sockaddr_in clt;
  struct timeval tmeout = {3, 0};
  socklen_t cltLen;
  int nfds = 0, fd = 0, bufLen = 0;
  struct epoll_event evts[max];
  Node *no = nullptr;
  auto it = gNodeMap.end();
  while (true) {
    nfds = epoll_wait(mEpollFd, evts, max, timeout);
    for (int i = 0; i < nfds; ++i) {
      if (evts[i].data.fd == mListenFd) {
        //fd = accept4(mListenFd, (struct sockaddr *)&clt, &cltLen, SOCK_NONBLOCK);
        fd = accept(mListenFd, (struct sockaddr *)&clt, &cltLen);
        //setNoblock(fd);
        //setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tmeout, sizeof(tmeout));
        setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tmeout, sizeof(tmeout));
        printf("clt addr:%s port:%d\n", inet_ntoa(clt.sin_addr), ntohs(clt.sin_port));
        no = nullptr;
        if (nullptr != mRegister && (no = mRegister(fd)) != nullptr) {
          no->ip = inet_ntoa(clt.sin_addr);
          no->port = ntohs(clt.sin_port);
          it = gNodeMap.find(fd);
          if (it == gNodeMap.end()) {
            gNodeMap[fd] = no;
            //printf("join:%d\n", fd);
            ctlEvent(fd, true);
          } else {
            printf("exist:%d\n", fd);
          }
        } else {
          printf("no register\n");
          close(fd);
        }
      } else if (evts[i].events & EPOLLIN) {
        if ((fd = evts[i].data.fd) < 0) {
          continue;
        } else if (nullptr == mDeal || 0 != mDeal(fd)) {
          gNodeMap.erase(fd);
          ctlEvent(fd, false);
          continue;
        }
        printf("in:%d\n", fd);
      } else if (evts[i].events & EPOLLOUT) {
        printf("out\n");
      } else {
        printf("others\n");// others
      }
    }
  }
  return 0;
}

void TcpServer::ctlEvent(int fd, bool flag)
{
  struct epoll_event ev;
  ev.data.fd = fd;
  ev.events = flag ? EPOLLIN : 0;
  epoll_ctl(mEpollFd, flag ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
  //printf("epoll fd:%d fd:%d\n", mEpollFd, fd);
  if (flag) {
    if (fd != mListenFd) {
      printf("comming:%d\n", fd);// coming
    }
  } else {
    printf("go:%d\n", fd);// go
    close(fd);
  }
}

void TcpServer::run()
{
  epollLoop(99, 100);
}
