Heliocentric Networking
=======================
Originally, I proposed a P2P networking strategy that I believed to be the
optimal solution for our game. However, after Geoff's presentation on Friday, I
think our best option is to begin with a server/client architecture that is
designed in such a way that the "Simulation" of the game may easily be pulled
into the client if need be.


# Basic Architecture

  The basic architecture of the networking system follows Geoff's suggestions
  pretty closely. It's pretty clear that we want to have basic networking off 
  the ground as soon as possible so that we can focus on other things.

  Network communications will be divided into two parts: the server and the 
  client. The server will run a fixed-interval loop where it receives all
  player input, processes it, and returns the delta of the game world to each
  client. The client then applies the delta to its own game world and renders
  it.

  The server loop will look as-follows:

  ```
  +---------------+
  | Create empty  |
  | list of world |
  |    changes    |
  +---------------+
          |
          V
  +---------------+            +------------+
  | Is time limit | ---YES---> | print warn |
  |   exceeded?   |            +------------+
  +---------------+                 |
         |                          |
         |NO                        |
         V                          |
   +----------------+               |
   | received input | <-------------+----------------------------------------+
   |    from all    |                                                        |
   |    players?    | --YES---> +--------------------+                       |
   +----------------+           | send list of world |                       |
         |                      | changes to each    |                       |
         |NO                    |   player           |                       |
         V                      +--------------------+                       |
   +-----------------------+                                                 |
 +>| received input from a |                                                 |
 | |      single player?   |----YES----> +--------------------------------+  |
 | +-----------------------+             | Process input - add changes to |--+
 |      |                                |      list of world changes     |
 |      |NO                              |     (network order resolves)   |
 |      V                                +--------------------------------+
 | +----------------------+
 | | has timeout elapsed? |---YES--->+------------------------+
 | +----------------------+          | d/c remaining players  |
 |       |                           +------------------------+
 |       | NO 
 |       V
 | +---------+
 +-|  wait   |
   +---------+
  ```


# Technology

  To start, we will use a single-threaded server using TCP sockets. Later on,
  we may switch to a multi-threaded model or a fast UDP implementation. 

  sockets should be fairly cross-platform. There may be some specific BSD Socks
  vs WinSock issues that need to be worked out.. but for the most part, we
  should be OK.

# File Structure

  For the networking architecture, we should have some layers of separation.

  * All game world simulation should be done in a network independent class (such
    that it may be later merged with the client if need be).

  * Sockets-level communication should be wrapped into an abstract C++ style
    class. Perhaps a "SocketConnection" class?

  * Server specific functionality should be in a "Server" class which extends
    the "SocketConnection" class.

  * Client specific functionality should be in a "Client" class which extends
    the "SocketConnection" class.
