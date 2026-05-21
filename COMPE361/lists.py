u = [None] * 5
print(u)
print(len(u))

k = 0
while k < 5:
    a = int(input())
    u[k] = a
    k = k + 1

print(len(u))

k = 4
while k >= 0:
    print(u[k])
    k = k - 1
