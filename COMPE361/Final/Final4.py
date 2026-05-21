 
f = open("C:\\SWSetup\\Info.txt", "r+b")
bt = f.read(1)
while bt:
    if bt == b'@':
        f.seek(-1,1)
        f.write(b'$')
    bt = f.read(1)

f.close()
    

