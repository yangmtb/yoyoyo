#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <string>
#include <cstdio>
#include <uuid/uuid.h>
#include <fcntl.h>

using std::string;

typedef struct NodeStruct Node;
typedef struct TaskStruct Task;

struct NodeStruct {
  int fd;
  string uuid;
  string name;
  int status;
  string ip;
  int port;
  Task tasks[10];
};

struct TaskStruct {
  string name;
  uint64_t capacity;
  uint64_t usable;
};

inline int setNoblock(int fd)
{
  int flag = 0;
  if (-1 == (flag = fcntl(fd, F_GETFL, 0))) {
    flag = 0;
  }
  return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

inline string genUuid()
{
  uuid_t uuid;
  uuid_generate(uuid);
  char buf[32] = {0};
  for (int i = 0; i < 16; ++i) {
    sprintf(buf+2*i, "%02x", uuid[i]);
  }
  return string(buf);
}

#endif // __COMMON_HPP__
