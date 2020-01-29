
import sys

if (len(sys.argv) != 2):
    print("expected filename")
    exit(1)

bytes = bytearray(open(sys.argv[1], "rb").read())

for i in range(0x17):
    sys.stdout.write(chr(bytes[i*17 + 5]))
sys.stdout.write('\n')
