u = (3,4,5)
# read only list
print(u)
print(u[0])
print(u[1])
print(u[2])

q = list(u)

q[1] = 7
print(q)
r = tuple(q)
print(r)
