#ifndef __ERR_HPP__
#define __ERR_HPP__

#include <cstdio>

inline void errExit(const char *msg, int code)
{
  printf("%s\n", msg);
  exit(code);
}

#endif //__ERR_HPP__
