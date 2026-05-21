p = int(input())

j = 2
while j < p:
    if p % j == 0:
        break
    else:
        j += 1

if j < p:
    print(p, "is composite")
else:
    print(p, "is prime")
