class Fraction:
    p = 1
    q = 1

    def getnumerator(self):
        return self.p
    def setnumerator(self, n):
        self.p = int(n)
    
    def getdenominator(self):
        return self.q
    def setdenominator(self, m):
        m = int(m)
        if m == 0:
            print("Incorrect Value")
            raise
        else:
            self.q = m
    
    numerator = property(fget=getnumerator,fset=setnumerator)
    denominator = property(fget=getdenominator,fset=setdenominator)
    
f = Fraction()
f.numerator = 3.14
print(f.numerator)
f.denominator = 5.6
print(f.denominator)
