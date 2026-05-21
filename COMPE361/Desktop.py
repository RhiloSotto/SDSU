def DisplayDashes(n):
    for j in range(0,n):
        print("-", end='')
    print('')

def GreatestCommonDivisor(p, q = 1000):
    while p != q:
        if p > q:
            p = p - q
        else:
            q = q - p
    return p

def LeastCommonMultiple(p, q):
    return (p * q) // GreatestCommonDivisor(p,q)


def fun(p,q):
    return p - q

r = fun(75, 35)


DisplayDashes(7)
print("Hello World")
DisplayDashes(3)
print("Good Bye")
DisplayDashes(5)


GreatestCommonDivisor(2000000, 12)
print(LeastCommonMultiple(23,46))
