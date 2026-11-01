*This project has been created as part of the 42 curriculum by akoaik, msafa.*

# Webserv

## Description

Webserv is an HTTP/1.1 server written from scratch in C++98.

The goal is to understand how a web server works: sockets, non-blocking I/O, the HTTP protocol, and CGI.
The server is configured with a file inspired by nginx.

Main features:

- One `poll()` loop for all sockets (non-blocking, never blocks on a client).
- Methods `GET`, `POST` and `DELETE`.
- Several servers on different ports, each one with its own config.
- Static files, index files, directory listing (autoindex).
- File upload (raw body and `multipart/form-data`).
- HTTP redirects (`return 301 /path;`).
- Custom error pages and a body size limit (`client_max_body_size`).
- CGI (Python and PHP), with a timeout for scripts that hang.
- Keep-alive connections.

## Instructions

### Build

```bash
make        # build ./webserv
make clean  # remove object files
make fclean # remove object files and the binary
make re     # rebuild everything
```

### Run

Run the server from the root of the repository (the paths in the config files are relative to it).

```bash
./webserv default.conf          # one server on port 8080
./webserv multipleServers.conf  # three servers on ports 8080, 8081 and 9000
```

Both configs use port 8080, so run only one at a time.
Stop the server with `Ctrl+C`.

### Project layout

| Path | Content |
|---|---|
| `src/` | Source code (`server/`, `http/`, `CGI/`, `parsing/`) |
| `includes/classes/` | Headers that define classes and structs |
| `includes/functions/` | Headers that declare functions |
| `errors/` | Default error pages |
| `www/default/` | Website of `default.conf` |
| `www/multipleServers/` | Websites of `multipleServers.conf` |

## Testing tools

**telnet**
telnet opens a TCP connection to a host and a port.
You type the request by hand. The server sends the response to your screen.
Use it to send one request slowly, line by line.
Push `Enter` two times after the headers to end the request.

```bash
telnet localhost 8080
```

**nc (netcat)**
nc also opens a TCP connection.
It sends the data that you give to it, exactly as it is.
Use it with `printf` to send a full request in one command.
Use it to send bad requests that a browser cannot send.

```bash
printf 'GET / HTTP/1.1\r\nHost: localhost\r\n\r\n' | nc localhost 8080
```

**siege**
siege is a load test tool.
It sends many requests at the same time to the server.
Use it to make sure that the server stays available under load.
The availability must be 100%.

```bash
siege -b -c 50 -t 30S http://localhost:8080/
```

## Requests for the evaluation

Start the server with `./webserv default.conf`, then copy and paste the commands below.
`-i` shows the status line and the headers.

### GET

```bash
# static page
curl -i http://localhost:8080/

# directory listing (autoindex on)
curl -i http://localhost:8080/files/

# file not found -> 404 custom page
curl -i http://localhost:8080/nope.html

# the same request with nc
printf 'GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n' | nc localhost 8080
```

### POST (raw body)

The file name comes from the URL.

```bash
curl -i -X POST --data-binary 'hello from raw post' http://localhost:8080/upload/raw.txt

# the same request with nc
printf 'POST /upload/raw_nc.txt HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\nConnection: close\r\n\r\nhello' | nc localhost 8080
```

### POST (multipart/form-data)

The file name comes from the form. Send the request to the upload folder.

```bash
echo 'hello from multipart' > /tmp/multi.txt
curl -i -F "file=@/tmp/multi.txt" http://localhost:8080/upload

# two files in one request
echo 'second file' > /tmp/multi2.txt
curl -i -F "file1=@/tmp/multi.txt" -F "file2=@/tmp/multi2.txt" http://localhost:8080/upload
```

### DELETE

```bash
# delete a file -> 204
curl -i -X DELETE http://localhost:8080/upload/raw.txt

# delete a file that does not exist -> 404
curl -i -X DELETE http://localhost:8080/upload/raw.txt
```

### CGI

```bash
# GET
curl -i http://localhost:8080/cgi-bin/hello.py

# POST with a body (form data)
curl -i -X POST -d 'name=safa&city=Beirut' http://localhost:8080/cgi-bin/form.py
```

### Redirect

```bash
curl -i http://localhost:8080/old
```

### Errors

```bash
# method not allowed -> 405
curl -i -X DELETE http://localhost:8080/

# body too large -> 413 (limit is 10M)
head -c 11000000 /dev/zero | curl -i -X POST --data-binary @- http://localhost:8080/upload/big.bin

# POST without Content-Length -> 411
printf 'POST /upload/x.txt HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n' | nc localhost 8080

# bad request (no Host header) -> 400
printf 'GET / HTTP/1.1\r\n\r\n' | nc localhost 8080
```

## Resources

### References

- [RFC 9110: HTTP Semantics](https://www.rfc-editor.org/rfc/rfc9110): methods, status codes, headers.
- [RFC 9112: HTTP/1.1](https://www.rfc-editor.org/rfc/rfc9112): message format, keep-alive, chunked encoding.
- [RFC 1945: HTTP/1.0](https://www.rfc-editor.org/rfc/rfc1945) and [RFC 2616: HTTP/1.1 (old version)](https://www.rfc-editor.org/rfc/rfc2616).
- [RFC 3875: CGI 1.1](https://www.rfc-editor.org/rfc/rfc3875): environment variables of CGI scripts.
- [nginx documentation](https://nginx.org/en/docs/): reference for the config file directives.
- [nginx source code on GitHub](https://github.com/nginx/nginx).
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/): sockets, `poll()`.

### Our notes and visualizations

- [Notion dashboard: WebServe](https://www.notion.so/3b67871ea4c18011912ac10dea47b7b8): our notes, the request flow, and the final notes for the evaluation.
- [Request Wheel](https://claude.ai/artifact/2tgMssax3RueRK7Syj9Voe): an interactive page that shows the life of a request inside the server.

### How we used AI

- **Understanding the concepts:** we used AI to make visualizations, dashboards and interactive websites
  that explain the concepts (the `poll()` loop, the request flow, CGI). Example: the Request Wheel above.
- **HTML and CSS:** AI wrote the HTML and CSS of the test websites in `www/` and of the error pages in `errors/`,
  so we did not lose time on parts that are not the goal of the project.
