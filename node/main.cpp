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

Node * handshark(int fd);

int main(int argc, const char *argv[])
{
  TcpClient clt("127.0.0.1", 7899, handshark);
  clt.run();
  return 0;
}

Node * handshark(int fd)
{
  Node *no = nullptr;
  char buf[1024] = {0};
  // recv whoareyou
  int cnt = recv(fd, buf, sizeof(buf), MSG_NOSIGNAL);
  if (cnt != 9 && strncmp(buf, "whoareyou", 9) != 0) {
    cout << "recv:" << cnt << endl;
    close(fd);
    return no;
    errExit("recv()", -1);
  }
  //sleep(2);
  // send iamyouson
  cnt = send(fd, "iamyouson", 9, MSG_NOSIGNAL);
  if (cnt != 9) {
    return no;
    errExit("send()", -1);
  }
  // recv uuid
  cnt = recv(fd, buf, sizeof(buf), MSG_NOSIGNAL);
  string t(buf);
  cout << "buf:" << buf << " cnt:" << cnt << endl;
  cout << "buf:" << t << endl;
  return no;
}
