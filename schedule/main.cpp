#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include "tcpSvr.hpp"

using std::cout;
using std::endl;
using std::thread;

intNodeMap gNodeMap;

Node * registerNode(int fd);
int dealBuffer(int fd);

void recvFunc()
{
  // recv node connect
  TcpServer svr(7899, registerNode, dealBuffer);
  svr.run();
}

void selectFunc()
{
  // select node status
  int i = 0;
  while (true) {
    cout << "------------------" << i << endl;
    for (auto &x: gNodeMap) {
      cout << x.first << " :" << x.second->uuid << " " << x.second->ip << " " << x.second->port << endl;
    }
    sleep(2);
    ++i;
  }
}

int main(int argc, const char *argv[])
{
  //cout << "uuid:" << genUuid() << endl;
  thread recvTh (recvFunc);
  thread selectTh (selectFunc);

  recvTh.join();
  selectTh.join();
  cout << "done" << endl;
  return 0;
}

Node * registerNode(int fd)
{
  Node *no = nullptr;
  if (fd <= 0) {
    return no;
  }
  int cnt = 0;
  // send whoareyou
  cnt = send(fd, "whoareyou", 9, MSG_NOSIGNAL);
  if (cnt != 9) {
    printf("send0\n");
    return no;
  }
  char bu[10] = {0};
  // recv iamyouson
  cnt = recv(fd, bu, 9, MSG_NOSIGNAL);
  if (cnt != 9 || strncmp(bu, "iamyouson", 9) != 0) {
    printf("err:%s\n", strerror(errno));
    printf("recv0 cnt:%d bu:%s\n", cnt, bu);
    return no;
  }
  no = new Node;
  no->fd = fd;
  no->uuid = genUuid();
  // send uuid
  cnt = send(fd, no->uuid.c_str(), no->uuid.size(), MSG_NOSIGNAL);
  if (cnt != no->uuid.size()) {
    printf("send1\n");
    delete no;
    return nullptr;
  }
  //cnt = recv(fd, bu, sizeof(bu), MSG_NOSIGNAL);
  return no;
}

int dealBuffer(int fd)
{
  if (fd <= 0) {
    return -1;
  }
  char buf[1024] = {0};
  uint64_t sum = 0;
  int cnt = 0;
  while (true) {
    cnt = recv(fd, buf, sizeof(buf), MSG_NOSIGNAL);
    if (cnt <= 0) {
      break;
    }
    sum += cnt;
    printf("cnt:%d \n", cnt);
  }
  if (sum <= 0) {
    return -1;
  }
  return 0;
}
