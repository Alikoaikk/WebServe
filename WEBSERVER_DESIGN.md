# HTTP Webserver Design (2-Person, 1 Month, C++98)

**Goal:** Build a non-blocking HTTP server in C++98 that passes the 42 webserv subject, split workload between two beginners.

**Architecture:** Person 1 handles networking layer (sockets, poll, HTTP protocol). Person 2 handles application layer (config parsing, CGI execution, file serving). They communicate through a clean Config interface.

**Tech Stack:** C++98, poll() for async I/O, standard library only (no Boost/external libs).

---

## 1. Work Split & Clear Responsibilities

### Person 1 (You - MohamadSafa): Networking & HTTP Protocol
- Socket management (listening, accepting, non-blocking I/O)
- Main event loop with poll()
- HTTP request parsing & validation
- HTTP response building & sending
- Client connection lifecycle management
- Error handling at network/protocol level

### Person 2: Config Parsing & Application Logic
- Config file parsing (servers, locations, routes, CGI rules)
- CGI execution & environment setup
- Static file serving
- File upload handling
- Directory structure validation
- Error handling at application level

### Shared Responsibility
- Integration testing
- Error pages (404, 500, etc.)
- Final testing & debugging

---

## 2. Architecture Overview

```
┌─────────────────────────────────────────────────┐
│           main.cpp (Entry Point)                │
│  1. Load Config from file (Person 2)            │
│  2. Initialize ServerManager (Person 1)         │
│  3. Run event loop (poll + accept/recv/send)    │
└──────────────────┬──────────────────────────────┘
                   │
        ┌──────────┴──────────┐
        │                     │
    ┌───▼─────────────────┐   ┌────▼──────────────┐
    │  ServerManager      │   │    Config         │
    │  (Person 1)         │   │  (Person 2)       │
    │ - Socket listening  │   │ - Parse servers   │
    │ - poll() loop       │   │ - Parse locations │
    │ - Client handling   │   │ - Query methods   │
    └───┬─────────────────┘   └────┬──────────────┘
        │                          │
        └──────────────┬───────────┘
                       │
            ┌──────────▼──────────┐
            │  ClientConnection   │
            │ (Person 1 + 2)      │
            │ - Parse request     │
            │ - Route matching    │
            │ - Serve file/CGI    │
            │ - Build response    │
            └─────────────────────┘
```

---

## 3. Key Components & Their Responsibilities

### Person 1 Provides (Networking Layer)

#### SocketManager
- Responsibility: Manage non-blocking sockets and poll loop
- Methods:
  - `createListenSocket(host, port)` → returns fd
  - `acceptConnections()` → new ClientConnection
  - `handleClientIO()` → reads/writes on ready fds
  - `addClient(ClientConnection*)` → track active clients
  - `removeClient(int fd)` → cleanup on disconnect

#### HTTPRequest
- Responsibility: Parse raw HTTP data into structured request
- Methods:
  - `parse(rawData: string)` → bool (success/fail)
  - `getMethod()` → "GET", "POST", "DELETE"
  - `getPath()` → "/index.html"
  - `getHeaders()` → map<string, string>
  - `getBody()` → string
  - `isComplete()` → bool (full request received)

#### HTTPResponse
- Responsibility: Build HTTP response to send
- Methods:
  - `setStatusCode(int code)` → void (200, 404, 500, etc.)
  - `setHeader(key, value)` → void
  - `setBody(string)` → void
  - `build()` → string (complete HTTP response)

#### ClientConnection
- Responsibility: Manage one client's lifecycle
- Fields:
  - `int fd` - client socket
  - `HTTPRequest request` - incoming
  - `HTTPResponse response` - outgoing
  - `string recvBuffer` - partial data received
  - `enum State { RECEIVING, PROCESSING, SENDING }` - state machine
- Methods:
  - `onReadReady()` → void (recv data, parse request)
  - `onWriteReady()` → void (send response)
  - `isComplete()` → bool (request + response done?)

### Person 2 Provides (Application Layer)

#### Config
- Responsibility: Parse config file and provide query interface
- Methods:
  - `loadFromFile(path)` → void
  - `getServers()` → vector<Server>
  - `findServer(port)` → const Server*
  - `findLocation(Server*, path)` → const Location*
  - `isMethodAllowed(Location*, method)` → bool
  - `getFileRoot(Location*)` → string
  - `hasCGI(Location*, extension)` → bool
  - `getCGIHandler(Location*, extension)` → string (e.g., "/usr/bin/php-cgi")
  - `getUploadDir(Location*)` → string
  - `getMaxBodySize()` → size_t

#### Server (nested in Config)
- Fields:
  - `string host` - "127.0.0.1"
  - `int port` - 8080
  - `vector<Location> locations` - routes
  - `map<int, string> errorPages` - 404 → "/404.html"

#### Location (nested in Server)
- Fields:
  - `string path` - "/api", "/uploads", etc.
  - `vector<string> allowedMethods` - ["GET", "POST"]
  - `string root` - "/var/www/html"
  - `bool dirListing` - false
  - `string defaultFile` - "index.html"
  - `string uploadPath` - where to save uploads
  - `map<string, string> cgiHandlers` - ".php" → "/usr/bin/php-cgi"

#### CGIHandler
- Responsibility: Execute CGI scripts safely
- Methods:
  - `execute(scriptPath, env, body)` → string (CGI output)
  - `setTimeout(seconds)` → void

#### FileServer
- Responsibility: Serve static files safely
- Methods:
  - `serveFile(filepath, Location*)` → pair<int, string> (status, content)
  - `listDirectory(dirpath, Location*)` → pair<int, string> (status, HTML)

---

## 4. Request → Response Flow (Integration Point)

```
Client sends HTTP request
         ↓
Person 1: recv() into buffer via poll
         ↓
Person 1: HTTPRequest::parse()
         ↓
Person 1: Query Config for matching Location
         ↓
Person 1: Check if method allowed
         ↓
    ┌────┴────┬─────────┬──────────┐
    ↓         ↓         ↓          ↓
  GET       POST     DELETE    Unknown
    ↓         ↓         ↓          ↓
  File    Upload   Delete    400 Bad
  Serve    File     File      Request
    │         │       │          │
    └─────────┴───────┴──────────┘
             ↓
    Person 1: HTTPResponse::build()
             ↓
    Person 1: send() via poll (non-blocking)
             ↓
    Person 1: close() or keep-alive
```

---

## 5. Error Handling Strategy (Strict Mode)

### Person 1 Handles (Network/Protocol Level)
| Error | Action |
|-------|--------|
| Client disconnect during recv | Gracefully close connection, remove from poll |
| Incomplete HTTP headers | Buffer data, wait for more (timeout after 30s) |
| Malformed HTTP | Return 400 Bad Request |
| recv/send fails | Log error, close connection, continue |
| poll() error | Log, continue |

### Person 2 Handles (Application Level)
| Error | Action |
|-------|--------|
| Invalid config file | Throw exception, fail on startup |
| File not found | Return 404 with error page |
| CGI crash/timeout | Return 500 Server Error |
| Permission denied on file | Return 403 Forbidden |

---

## 6. File Organization

```
WebServe/
├── includes/
│   ├── headers/
│   │   ├── imports.hpp              (All #includes)
│   │   └── Constants.hpp            (Timeouts, limits)
│   │
│   └── classes/
│       ├── Socket.hpp               (Person 1)
│       ├── HTTPRequest.hpp          (Person 1)
│       ├── HTTPResponse.hpp         (Person 1)
│       ├── ClientConnection.hpp     (Person 1)
│       ├── Config.hpp               (Person 2)
│       ├── CGIHandler.hpp           (Person 2)
│       ├── FileServer.hpp           (Person 2)
│       └── parsing.hpp              (Person 2)
│
├── src/
│   ├── server/
│   │   ├── Socket.cpp               (Person 1)
│   │   └── ClientConnection.cpp     (Person 1)
│   │
│   ├── http/
│   │   ├── HTTPRequest.cpp          (Person 1)
│   │   └── HTTPResponse.cpp         (Person 1)
│   │
│   ├── parsing/
│   │   ├── Config.cpp               (Person 2)
│   │   ├── tokenize.cpp             (Person 2)
│   │   ├── servParsing.cpp          (Person 2)
│   │   └── locationParsing.cpp      (Person 2)
│   │
│   └── cgi/
│       ├── CGIHandler.cpp           (Person 2)
│       └── FileServer.cpp           (Person 2)
│
├── main.cpp                         (Person 1: Event loop)
├── Makefile
└── config_files/
    ├── default.conf
    └── test.conf
```

---

## 7. Timeline & Milestones

### Week 1: Core Infrastructure (Both parallel)
**Person 1:**
- Socket listening on port 8080
- Accept client connection
- Basic poll() loop
- HTTPRequest parser (GET)
- HTTPResponse builder

**Person 2:**
- Config.hpp interface
- Parse basic config file
- Server/Location structures

**Integration:**
- Person 1 loads Person 2's config

### Week 2: Core Features (Still parallel)
**Person 1:**
- Handle multiple clients
- Non-blocking recv (partial requests)
- Client timeout (30s)
- HTTP error handling (400, 404, 500)

**Person 2:**
- CGIHandler structure
- FileServer structure
- Parse CGI rules

**Integration:**
- Person 1 calls FileServer → static files
- Person 1 calls CGIHandler → CGI execution

### Week 3: Advanced Features (Mostly integration)
**Person 1:**
- POST request parsing (body)
- Multiple listening ports
- Keep-alive handling

**Person 2:**
- File upload handling
- Error page serving

**Integration:**
- End-to-end: GET, POST, DELETE
- Full CGI support
- Stress testing

### Week 4: Polish & Testing
**Both:**
- Bug fixes
- Compliance check (NGINX)
- Memory/timeout checks
- Final browser testing
- README

---

## 8. Critical Dependencies

**Person 2 blocks Person 1 on:**
1. Config.hpp interface (Person 1 needs to know what methods to call)
2. Basic config parsing (Person 1 needs real config data to test with)

**Person 1 can work independently:**
1. Socket listening on hardcoded port
2. HTTP parsing & response building
3. Basic poll loop

---

## 9. Testing Strategy

**Unit Testing (Each person):**
- HTTPRequest: Parse GET, POST, DELETE, malformed
- HTTPResponse: Different status codes, headers, bodies
- Config: Parse valid/invalid configs
- FileServer: Serve files, 404, permissions
- CGIHandler: Execute script, timeout, crash

**Integration Testing (Week 3+):**
- Full request → response cycle
- Multiple clients
- File uploads
- CGI execution
- Error pages
- Stress testing

**Manual Testing:**
- telnet: `telnet localhost 8080` → GET
- curl: `curl -X POST http://localhost:8080/upload`
- Browser: `http://localhost:8080/index.html`

---

## 10. Mandatory Features

| Feature | Person | Status |
|---------|--------|--------|
| Config parsing | 2 | |
| Non-blocking socket (poll) | 1 | |
| GET method | 1 | |
| POST method | 1 | |
| DELETE method | 1 | |
| Static file serving | 2 | |
| File uploads | 2 | |
| Error pages | 2 | |
| CGI execution | 2 | |
| Multiple ports | 1 | |

---

## Summary

- **Person 1:** Networking + HTTP (sockets, poll, request/response)
- **Person 2:** Application logic (config, files, CGI)
- **Interface:** Config class with query methods
- **Timeline:** 4 weeks with Week 1-2 parallel work, Week 3-4 integration
- **Testing:** Unit tests + integration + manual testing
- **Safety:** Strict error handling, no crashes
