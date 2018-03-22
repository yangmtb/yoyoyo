#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "util/err.hpp"
#include <iostream>
#include <string>
#include "tcpClt.hpp"

using std::cout;
using std::endl;
using std::string;

int handshark(int fd);
int sendNodeInfo(int fd);

Node gNode;

int main(int argc, const char *argv[])
{
  Task t;
  t.name = "task0";
  t.capacity = 888888;
  t.usable = 888888;
  gNode.fd = 4;
  gNode.uuid = genUuid();
  gNode.tasks[0] = t;
  printf("void *:%d\n", sizeof(void *));
  printf("task:%d, name:%d, capacity:%d, usable:%d\n", sizeof(t), sizeof(t.name), sizeof(t.capacity), sizeof(t.usable));
  printf("node:%d, fd:%d, uuid:%d, name:%d, status:%d, ip:%d, port:%d, tasks:%d\n", sizeof(gNode), sizeof(gNode.fd), sizeof(gNode.uuid), sizeof(gNode.name), sizeof(gNode.status), sizeof(gNode.ip), sizeof(gNode.port), sizeof(gNode.tasks));
  cout << "size:" << sizeof(gNode) << " t:" << sizeof(t) << endl;
  return 0;
  TcpClient clt("127.0.0.1", 7899, handshark, sendNodeInfo);
  clt.run();
  return 0;
}

int handshark(int fd)
{
  char buf[1024] = {0};
  // recv whoareyou
  int cnt = recv(fd, buf, sizeof(buf), MSG_NOSIGNAL);
  if (cnt != 9 && strncmp(buf, "whoareyou", 9) != 0) {
    cout << "recv:" << cnt << endl;
    close(fd);
    return -1;
    errExit("recv()", -1);
  }
  //sleep(2);
  // send iamyouson
  cnt = send(fd, "iamyouson", 9, MSG_NOSIGNAL);
  if (cnt != 9) {
    return -1;
    errExit("send()", -1);
  }
  memset(buf, 0, sizeof(buf));
  // recv uuid
  cnt = recv(fd, buf, sizeof(buf), MSG_NOSIGNAL);
  gNode.fd = fd;
  gNode.uuid = buf;
  //cout << "buf:" << buf << " cnt:" << cnt << endl;
  cout << "uuid:" << gNode.uuid << endl;
  //sleep(4);
  return 0;
}

int sendNodeInfo(int fd)
{
  if (fd != gNode.fd) {
    return -1;
  }
  Task t;
  t.name = "task0";
  t.capacity = 888888;
  t.usable = 888888;
  gNode.tasks = &t;
  cout << "size:" << sizeof(gNode) << " t:" << sizeof(t) << endl;
  return 0;
}
