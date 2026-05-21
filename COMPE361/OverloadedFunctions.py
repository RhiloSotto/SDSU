class Fraction:
    p = 0
    q = 1
    def __init__(self, p, q):
        if q == 0:
            q = 1
        self.p = p
        self.q = q


    def Display(self):
        print("How are you")

    # static function
    # must be called without an object
    # use the class.function
    @staticmethod
    def SumOfFractions(u,z):
        m = u.p * z.q + u.q * z.p
        n = u.q * z.q
        return Fraction(m,n)
    # overloaded add(+) operator
    def __add__(self, f):
        m = self.p * f.q + self.q * f.p
        n = self.q * f.q
        g = Fraction(m,n)
        return g
    # overloaded str() operator
    def __str__(self):
        return str(self.p) + '/' + str(self.q)



a = Fraction(2,3)
b = Fraction(1,4)

print(Fraction.SumOfFractions(a, b))

a.Display()

c = a + b

print(c.p)
print(c.q)

print(c)
