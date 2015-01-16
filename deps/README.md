This directory contains all C/C++ function templates in order to wrap various parts of the nanomsg system. The `iojs` interface for protocols and transports etc.

# How it works
Access to nanomsg functions/protocols is given by `nmsg.cc`, where an `#include` near the top of the file, and directly below V8 class declarations, links to `templates.h`. Each of these linked template functions get assigned as methods/properties among other nanomsg symbols on a single module for recognition by the node require algorithm.

# V8 Efficient Garbage Collection
V8 reclaims memory used by objects that are no longer required in a process known as garbage collection. To ensure fast object allocation, short garbage collection pauses, and no memory fragmentation V8 employs a stop-the-world, generational, accurate, garbage collector. This means that V8:
* stops program execution when performing a garbage collection cycle.
* processes only part of the object heap in most garbage collection cycles. This minimizes the impact of stopping the application.
* always knows exactly where all objects and pointers are in memory. This avoids falsely identifying objects as pointers which can result in memory leaks.

In V8, the object heap is segmented into two parts: new space where objects are created, and old space to which objects surviving a garbage collection cycle are promoted. If an object is moved in a garbage collection cycle, V8 updates all pointers to the object.
