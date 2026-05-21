# type casting
print("TYPE CASTING")
p = 2.71
q = int(p)
r = p % 1
print(type(q), "hello", q)
print(type(r), r)

h = "1234"
r = int(h)
print(type(r), r)

h = "3.14"
r = float(h) + 1
print(type(r), r)
if r == 4.14:
    print("Yes")
else:
    print("No")


# Bitwise
print("BITWISE")
p = 75
q = 49

r = p & q
print(r)

r = p | q
print(r)

r = p ^ q
print(r)

r = ~p
print(r)

r = p << 1
print(r)

r = p >> 1
print(r)


# Logical Expressions
print("LOGICAL")
p = 2500
q = 1500

r = p > q
print(r)

r = p == q
print(r)

r = p > 1000 and p < 3000
print(r)

r = p == 1000 or p == 2000
print(r)

r = not p == 1000
print(r)

