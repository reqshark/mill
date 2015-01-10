/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://www.wtfpl.net/>
 */

#include <node.h>
#include <node_version.h>
#include <node_buffer.h>
#include <uv.h>
#include <v8.h>

extern "C" {
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/pipeline.h>
#include <nanomsg/bus.h>
#include <nanomsg/pair.h>
#include <nanomsg/reqrep.h>
}
//#include "nan.h"
#include "templates.h"

static void Init (Handle<Object> target) {

  //  Utility functions
  target->Set(String::NewSymbol("sleep"),
      FunctionTemplate::New(Sleep)->GetFunction());

  target->Set(String::NewSymbol("usleep"),
      FunctionTemplate::New(USleep)->GetFunction());

  //  Socket functions
  target->Set(String::NewSymbol("socket"),
      FunctionTemplate::New(Socket)->GetFunction());

  target->Set(String::NewSymbol("close"),
      FunctionTemplate::New(Close)->GetFunction());

  target->Set(String::NewSymbol("setsockopt"),
      FunctionTemplate::New(SetSockOpt)->GetFunction());

  target->Set(String::NewSymbol("getsockopt"),
      FunctionTemplate::New(GetSockOpt)->GetFunction());

  target->Set(String::NewSymbol("connect"),
      FunctionTemplate::New(Connect)->GetFunction());

  target->Set(String::NewSymbol("bind"),
      FunctionTemplate::New(Bind)->GetFunction());

  target->Set(String::NewSymbol("send"),
      FunctionTemplate::New(Send)->GetFunction());

  target->Set(String::NewSymbol("recv"),
      FunctionTemplate::New(Recv)->GetFunction());

  target->Set(String::NewSymbol("errno"),
      FunctionTemplate::New(Errno)->GetFunction());

  target->Set(String::NewSymbol("strerror"),
      FunctionTemplate::New(StrErr)->GetFunction());

  //  SP address families.
  NODE_DEFINE_CONSTANT(target, AF_SP);
  NODE_DEFINE_CONSTANT(target, AF_SP_RAW);

  //  Socket option levels: Negative numbers are reserved for transports,
  //    positive for socket types.
  NODE_DEFINE_CONSTANT(target, NN_SOL_SOCKET);

  //  Generic socket options (NN_SOL_SOCKET level).
  NODE_DEFINE_CONSTANT(target, NN_LINGER);
  NODE_DEFINE_CONSTANT(target, NN_SNDBUF);
  NODE_DEFINE_CONSTANT(target, NN_RCVBUF);
  NODE_DEFINE_CONSTANT(target, NN_SNDTIMEO);
  NODE_DEFINE_CONSTANT(target, NN_RCVTIMEO);
  NODE_DEFINE_CONSTANT(target, NN_RECONNECT_IVL);
  NODE_DEFINE_CONSTANT(target, NN_RECONNECT_IVL_MAX);
  NODE_DEFINE_CONSTANT(target, NN_SNDPRIO);
  NODE_DEFINE_CONSTANT(target, NN_SNDFD);
  NODE_DEFINE_CONSTANT(target, NN_RCVFD);
  NODE_DEFINE_CONSTANT(target, NN_DOMAIN);
  NODE_DEFINE_CONSTANT(target, NN_PROTOCOL);
  NODE_DEFINE_CONSTANT(target, NN_IPV4ONLY);

  //  Send/recv options.
  NODE_DEFINE_CONSTANT(target, NN_DONTWAIT);

  //  Socket types
  NODE_DEFINE_CONSTANT(target, NN_REQ);
  NODE_DEFINE_CONSTANT(target, NN_REP);
  NODE_DEFINE_CONSTANT(target, NN_PAIR);
  NODE_DEFINE_CONSTANT(target, NN_PUSH);
  NODE_DEFINE_CONSTANT(target, NN_PULL);
  NODE_DEFINE_CONSTANT(target, NN_PUB);
  NODE_DEFINE_CONSTANT(target, NN_SUB);
  NODE_DEFINE_CONSTANT(target, NN_BUS);
}

// Register the initialization function.
NODE_MODULE(nanomsg, Init)
