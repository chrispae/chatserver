# Chat server

C++ implementation of a chat server based on sockets

## ToDo

- [ ] provide single-threaded server
  - [x] provide echo-server
    - client can send arbitrary amount of messages
    - connection gets terminated on client disconnect
    - server logs each incoming connection, incoming message and disconnected client
  - [ ] provide calculator server
  - [ ] provide multi-user chat
- [x] provide multi-threaded server
  - echo-loop is surrounded by a lambda, that is passed to a detached thread
- [ ] provide documentation
  - project could be complex enough to benefit from separat documentation
  - maybe using doxygen
