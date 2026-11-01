#!/usr/bin/env python3
# CGI test: the form is saved to a file, then the script redirects (no HTML).
#
#   GET  /cgi-bin/post_redirect.py                       -> shows the form
#   POST /cgi-bin/post_redirect.py   (body: name=safa)   -> saves www/multipleServers/uploads/<name>.txt
#   GET  /cgi-bin/post_redirect.py?name=safa&city=Beirut -> same, data from the query string
#   Both answer "303 See Other" and the browser goes back to the form.

import os
import sys
from urllib.parse import parse_qs

UPLOAD_FOLDER = "../uploads"     # the server runs the script inside cgi-bin/


def read_form():
    # POST: the variables are in the body (stdin), its size is CONTENT_LENGTH.
    # GET:  the variables are in the URL after "?", the server puts them in QUERY_STRING.
    if os.environ.get("REQUEST_METHOD") == "POST":
        size = int(os.environ.get("CONTENT_LENGTH", "0"))
        text = sys.stdin.read(size)
    else:
        text = os.environ.get("QUERY_STRING", "")
    form = parse_qs(text)                        # "name=safa&city=Beirut" -> {"name": ["safa"], "city": ["Beirut"]}
    return form


def save_to_file(form):
    # File name = the "name" field, letters and digits only (no "../" tricks).
    name = form.get("name", ["form"])[0]
    name = "".join(c for c in name if c.isalnum()) or "form"
    filename = name + ".txt"

    with open(UPLOAD_FOLDER + "/" + filename, "w") as f:
        for key, values in form.items():
            f.write(key + "=" + values[0] + "\n")
    return filename


def send_redirect(filename):
    # Only headers, no body. The server turns "Status:" into the status line.
    print("Status: 303 See Other", end="\r\n")
    print("Location: /cgi-bin/post_redirect.py?saved=" + filename, end="\r\n")
    print("Content-Length: 0", end="\r\n")
    print("", end="\r\n")                        # empty line = end of headers


def show_form():
    saved = os.environ.get("QUERY_STRING", "").replace("saved=", "")
    message = ""
    if saved:
        message = "<p>Saved: www/multipleServers/uploads/" + saved + "</p>"

    html = """<!DOCTYPE html>
<html>
<body>
  <h1>POST + redirect test</h1>
  """ + message + """
  <form action="/cgi-bin/post_redirect.py" method="post">
    <p>Name: <input name="name" value="safa"></p>
    <p>City: <input name="city" value="Beirut"></p>
    <p><button>Save</button></p>
  </form>
</body>
</html>
"""
    print("Content-Type: text/html", end="\r\n")
    print("Content-Length: " + str(len(html)), end="\r\n")
    print("", end="\r\n")
    print(html, end="")


# ---- main ----
form = read_form()
if "name" in form:                  # the form was sent (by POST or by GET query string)
    filename = save_to_file(form)
    send_redirect(filename)
else:                               # no form data: show the page
    show_form()
