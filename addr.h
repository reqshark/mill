/*

  Copyright (c) 2015 Bent Cardan

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

char *ntop (ipaddr addr) {
  char *ip = NULL;
  if (((struct sockaddr*)&addr)->sa_family == AF_INET) {
    ip = (char *)malloc(INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(((struct sockaddr_in*)&addr)->sin_addr), ip, INET_ADDRSTRLEN);
  } else {
    ip = (char *)malloc(INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &(((struct sockaddr_in6*)&addr)->sin6_addr), ip, INET6_ADDRSTRLEN);
  }
  return ip;
}
