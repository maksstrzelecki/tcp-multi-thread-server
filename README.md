# TCP / HTTP Server Project Description

This project is a beginner-friendly C++ server implementation built to explore low-level network programming, TCP communication, POSIX sockets, multithreading, and basic HTTP request parsing.

The project started as a minimal TCP server and gradually evolved into a simple multithreaded HTTP-style server. It currently accepts client connections, reads incoming HTTP requests, parses the request line and headers, and sends back a plain text HTTP response containing basic request information such as method, path, version, and host.

## Project Evolution

### 2026-07-08 - Project Initialization

The repository was initialized with the basic project structure. Empty starter files were created, including `README.md`, `include/main.hpp`, and `src/main.cpp`.

This commit established the initial workspace for the server project.

Commit: `648e981`  
Message: `project initialization`

### 2026-07-08 - Basic TCP Server With POSIX Sockets

The first working server implementation was added using the POSIX sockets API.

At this stage, the project introduced the core networking flow:

- creating a socket
- binding it to an address and port
- listening for incoming connections
- accepting a client connection
- exchanging simple data between server and client

This version focused on understanding raw TCP server-client communication before adding HTTP-specific behavior.

Commit: `fbd5806`  
Message: `Starter implementation of server-client communication using posix sockets api`

### 2026-07-09 - Thread Pool and Concurrent Server Handling

The server was extended with a thread pool implementation.

Instead of handling only one client at a time in a purely sequential way, the project started moving toward concurrent client handling. A queue of tasks, worker threads, mutexes, and a condition variable were introduced.

This was an important architectural step because it separated connection acceptance from request processing. The main thread could continue accepting new clients while worker threads handled existing client work.

Commit: `c6eb0a4`  
Message: `Added thread pool implementation and starting concurrent version of TCP`

### 2026-08-11 - Basic HTTP Parser and Response Logic

The server evolved from a raw TCP server into a basic HTTP-aware server.

This version added simple HTTP request parsing and response generation. The server began reading HTTP-like requests, extracting request information, and returning a valid HTTP response with headers such as `Content-Type`, `Content-Length`, and `Connection`.

This marked the transition from general socket communication toward an educational HTTP server.

Commit: `9073b2e`  
Message: `Basic http parser implemented + response logic`

### 2026-08-22 - Improved HTTP Request Parsing

The HTTP request parsing logic was improved.

The changes made request parsing more reliable and prepared the server for cleaner extraction of HTTP data. This step refined the earlier parsing approach and made the server better at handling real HTTP request structure.

Commit: `4366de1`  
Message: `Improved http request parsing`

### 2026-08-23 - Dedicated HTTP Request Parser Class

The latest version introduced a dedicated `HttpRequest` class.

This separated HTTP request parsing from the main server loop, making the code easier to understand and maintain. The request parser now stores parsed values such as:

- HTTP method
- request path
- HTTP version
- headers
- body placeholder

The project also cleaned up repository contents by removing a compiled binary from source tracking and adding README notes about HTTP request structure and request termination.

Commit: `2118474`  
Message: `Defined basic http parser class`

## Current State

The current project is a simple multithreaded HTTP server written in C++.

It uses POSIX sockets for networking and a custom thread pool for concurrent request handling. When a client connects, the server receives the request, parses the HTTP request line and headers, then sends back a plain text HTTP response.

The project is mainly educational. It demonstrates how web servers work at a lower level, before using higher-level frameworks or libraries.

## Main Concepts Demonstrated

- TCP sockets
- POSIX networking API
- server socket setup
- binding and listening on a port
- accepting client connections
- multithreading
- thread pools
- task queues
- mutexes and condition variables
- basic HTTP request parsing
- HTTP response formatting
- separating parsing logic into a class

## Possible Future Improvements

Possible next steps for the project include:

- returning `400 Bad Request` for invalid requests
- supporting request bodies using `Content-Length`
- avoiding assumptions about required headers such as `Host`
- improving error handling around `send()` and `recv()`
- supporting different routes
- moving classes into separate header and source files
- adding a build system such as CMake
- adding tests for the HTTP parser
- supporting graceful server shutdown