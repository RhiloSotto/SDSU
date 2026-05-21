def max4(a,b,c,d,f):
    r = a;
    if f(b, r):
        r = b
    if f(c, r):
        r = c
    if f(d, r):
        r = d
    return r

def compareExceptions(p,q):
    return str(p) > str(q)

def compareNumbers(p,q):
    return p > q

x1 = "abcd"
x2 = "efgh"
x3 = "ijkl"
x4 = "asdf"

z = max4(x1,x2,x3,x4,compareExceptions)
print(z)

x1 = Exception("abcd")
x2 = Exception("efgh")
x3 = Exception("ijkl")
x4 = Exception("asdf")

z = max4(x1,x2,x3,x4, compareExceptions)
print(z)

z = max4(9,3,7,5, lambda p, q: p > q)
print(z)
