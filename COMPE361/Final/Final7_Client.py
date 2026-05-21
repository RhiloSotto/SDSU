import socket

client = socket.socket(family = socket.AF_INET, type = socket.SOCK_STREAM)

client.connect( ("146.244.39.154", 8000) )

txt = input()

client.send(txt.encode())

info = client.recv(1024).decode()

client.close()

print(info)
