import os
import subprocess

if os.name == 'nt':
    print("Run build.bat")
    os.system("build.bat")
else:
    print("Run build.sh")
    os.system("./build.sh")

