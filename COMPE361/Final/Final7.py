import socket

server = socket.socket(family = socket.AF_INET, type=socket.SOCK_STREAM)

server.bind( ("0.0.0.0", 8000) )

server.listen(10)

client,address = server.accept()
print(address)

txt = client.recv(1024).decode()

txt = "(" + txt + ")"

client.send(txt.encode())

client.close()

server.close()
