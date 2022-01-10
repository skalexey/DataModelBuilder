import os
import subprocess

status = 0

if os.name == 'nt':
    print("Run build.bat")
    status = os.system("build.bat")
else:
    print("Run build.sh")
    status = os.system("./build.sh")

exit(status)

