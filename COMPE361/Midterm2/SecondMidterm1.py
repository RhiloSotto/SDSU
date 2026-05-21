s = set()

p = int(input())
while not p in s:
    s.add(p)
    p = int(input())

print(s)
