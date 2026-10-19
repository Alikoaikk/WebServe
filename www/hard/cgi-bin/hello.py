#!/usr/bin/env python3
# Minimal CGI script for hard.conf (server 1, /cgi-bin).

import os
import sys

body = """<!DOCTYPE html>
<html>
<head><meta charset="utf-8"><title>Python CGI</title></head>
<body>
  <h1>Hello from Python CGI</h1>
  <p>Method: %s</p>
  <p>Query: %s</p>
  <p><a href="/">back home</a></p>
</body>
</html>
""" % (os.environ.get("REQUEST_METHOD", ""), os.environ.get("QUERY_STRING", ""))

sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("Content-Length: %d\r\n" % len(body.encode("utf-8")))
sys.stdout.write("\r\n")
sys.stdout.write(body)
