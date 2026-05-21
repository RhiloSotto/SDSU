u = set()
q = [1,2,7,4,3,2,1,7,4]
for j in q:
    u.add(j)
for j in u:
    print(j)

z = {3,2,7,9,4,1,5,2}
print(z)

if 9 in z:
    print("Yes")

a = {1,3,4}
b = {2,4}
c= a.union(b)
print(c)
