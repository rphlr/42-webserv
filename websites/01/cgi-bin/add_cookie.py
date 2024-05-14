import os
from http import cookies
# Import modules for CGI handling 
import cgi, cgitb 

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
key = form.getvalue('key')
value  = form.getvalue('value')

# Check if both key and value are received
if key is not None and value is not None:
    cookie = cookies.SimpleCookie()
    cookie[key] = value
    print("HTTP/1.1 204 OK")
    print(cookie.output())
    print("\r\n")
else:
    print("HTTP/1.1 400 Bad Request")
    print("Content-Type: text/plain\r\n")
    print("Missing key or value in the request.\r\n")
