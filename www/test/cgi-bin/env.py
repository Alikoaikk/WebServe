#!/usr/bin/env python3
# Shows the CGI environment the server passed in, plus the parsed query string.
# Try: /cgi-bin/env.py?name=webserv&x=1

import os
import sys

try:
    from urllib.parse import parse_qs
except ImportError:
    from urlparse import parse_qs

qs = os.environ.get("QUERY_STRING", "")
params = parse_qs(qs)

rows = ""
for key in ("REQUEST_METHOD", "QUERY_STRING", "CONTENT_LENGTH",
            "SCRIPT_FILENAME", "GATEWAY_INTERFACE", "SERVER_PROTOCOL"):
    rows += "<tr><td>%s</td><td>%s</td></tr>" % (key, os.environ.get(key, ""))

name = params.get("name", ["stranger"])[0]

body = """<!DOCTYPE html>
<html><head><meta charset="utf-8"><title>CGI env</title></head>
<body>
  <h1>Hello, %s!</h1>
  <h2>CGI environment</h2>
  <table border="1" cellpadding="6">%s</table>
  <p><a href="/">back home</a></p>
</body></html>
""" % (name, rows)

sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("Content-Length: %d\r\n" % len(body.encode("utf-8")))
sys.stdout.write("\r\n")
sys.stdout.write(body)
