import cgi, fileinput, os, sys

try:
	message = 'what?!'
	message = cgi.FieldStorage()
	fileitem = parsed['filename']
except:
	message = 'No file was uploaded'

print("""\
Content-Type: text/html\r\n\r\n
<html>
<body>
   <p>%s</p>
</body>
</html>
""" % (message))