import sys
import os

for line in sys.stdin:
    if 'Exit' == line.rstrip():
        break
    filename = line[9:]

path = os.path.abspath(os.getcwd())
path += "/uploads/"
path += filename
if os.path.exists(path):
  os.remove(path)
  message = "success"
else:
  message = "The file does not exist"


print("""\
Content-Type: text/html\r\n\r\n
<html>
<body>
   <p>%s</p>
</body>
</html>
""" % (message))