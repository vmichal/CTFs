import sys


bytes = bytearray(sys.stdin.buffer.read())
handlers = []

for i in range(0x17):
    handlers.append(bytearray(bytes[17*i:17*(i+1)]))

for handler in handlers:
    if handler[0] != 0x56:
        xor_val = handler[0] ^ 0x56
        for i in range(len(handler)):
            handler[i] ^= xor_val

for handler in handlers:
        sys.stdout.buffer.write(handler)
