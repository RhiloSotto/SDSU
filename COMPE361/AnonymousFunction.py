# a function with no name
f = lambda x: x*x
# the function is lost
f = None


h = 50
f = lambda x: x*x
z = f(h)
print(z)
print(f(2))


f = lambda p, q: (p+q) /q
print(f(h, h))
