class First:
    a = 1
    b = 1
    def firstmethod(self, a, b):
        self.a = a
        self.b = b
    def __init__(self, a, b):
        self.a = a
        self.b = b
    def area(self):
        return self.a * self.b * 10000000

class Second:
    b = 2
    c = 2
    def secondmethod(self, b, c):
        self.b = b
        self.c = c
    def __init__(self, b, c):
        self.b = b
        self.c = c
    def area(self):
        return self.b * self.c * 10

# multiple inheritance
# common child of First and Second
# concurrency will occur
class Derived(First, Second):
    def Display(self):
        print(self.a)
        print(self.b)
        print(self.c)


d = Derived(3,3)

# the b field of First is transferred
# first inheriting class called (one, two)
# one takes priority
d.Display()

# First's area function is used
# since it is inherited from first
q = d.area()
print(q)

d.firstmethod(7,9)
d.secondmethod(5,4)
d.Display()
