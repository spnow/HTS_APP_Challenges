#! /usr/bin/env python
magic = '1M953'

for i in range(300, 3000):
    if sum ([i ^ ord(c) for c in magic]) == 3530:
        print i
        break;

# i=753
# aaaaaaa@ + '\n' you pressed
