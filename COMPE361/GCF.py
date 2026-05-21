p = int(input())
q = int(input())

while p != q:
    if p > q:
        p -= q
    else:
        q -= p

print(p)
