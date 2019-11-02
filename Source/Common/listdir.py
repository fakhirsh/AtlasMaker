import os
import sys

if(len(sys.argv) != 2):
    print("Error: usage --> python3 lstdir.py [DIRNAME]")
    exit(0)
    
path = sys.argv[1]

files = []
# r=root, d=directories, f = files
for r, d, f in os.walk(path):
    for file in f:
        if '.png' in file:
            files.append(os.path.join(r, file))
            #files.append(file)

for f in files:
    print(f)

