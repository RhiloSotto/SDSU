

f = open("C:\\SWSetup\\Info.txt", "r+b")
byte = f.read(1)

while byte:
    if byte == b';':
        f.seek(-1, 1)
        f.write(b',')
    byte = f.read(1)

f.close()
