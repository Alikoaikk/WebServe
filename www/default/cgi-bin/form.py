#!/usr/bin/env python3
# Reads a POST body from stdin (the server pipes the request body in) and
# echoes the submitted form fields back. Works with the form in form.html.

import os
import sys

try:
    from urllib.parse import parse_qs
except ImportError:
    from urlparse import parse_qs

length = 0
try:
    length = int(os.environ.get("CONTENT_LENGTH", "0") or "0")
except ValueError:
    length = 0

raw = sys.stdin.read(length) if length > 0 else sys.stdin.read()
fields = parse_qs(raw)

items = ""
for key in sorted(fields):
    for value in fields[key]:
        items += "<li><strong>%s</strong>: %s</li>" % (key, value)
if not items:
    items = "<li>(no fields received)</li>"

body = """<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>CGI form result</title>
<style>
  body { font-family: sans-serif; max-width: 760px; margin: 24px auto; padding: 0 16px; color: #222; line-height: 1.5; }
  h1 { color: #2c5282; }
  a { color: #2b6cb0; }
  li { margin: 4px 0; }
</style>
</head>
<body>
  <h1>POST received via CGI</h1>
  <p>Method: %s, Content-Length: %d</p>
  <ul>%s</ul>
  <p><a href="/cgi-bin/form.html">try again</a> · <a href="/">home</a></p>
</body></html>
""" % (os.environ.get("REQUEST_METHOD", "?"), length, items)

sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("Content-Length: %d\r\n" % len(body.encode("utf-8")))
sys.stdout.write("\r\n")
sys.stdout.write(body)
