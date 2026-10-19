#!/usr/bin/env python3
# CGI script for hard.conf (server 3, /api): returns request info as JSON.

import json
import os
import sys

data = {
    "method": os.environ.get("REQUEST_METHOD", ""),
    "query": os.environ.get("QUERY_STRING", ""),
    "body": sys.stdin.read(),
}
body = json.dumps(data) + "\n"

sys.stdout.write("Content-Type: application/json\r\n")
sys.stdout.write("Content-Length: %d\r\n" % len(body.encode("utf-8")))
sys.stdout.write("\r\n")
sys.stdout.write(body)
