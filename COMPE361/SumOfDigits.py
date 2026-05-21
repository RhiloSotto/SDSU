p = int(input())

u = 0
while p > 0:
    u += p % 10
    p //= 10

print(u)
