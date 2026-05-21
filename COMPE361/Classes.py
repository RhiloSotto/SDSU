class Rectangle:
    length = 1
    width = 1
    def area(self):
        a = self.length * self.width
        return a
    def __init__(self, l, w):
        if l <= 0 or w <= 0 or l < w:
            print("Incorrect arguments for constructor")
            #raise - terminate/exception but will learn later
        else:
            self.length = l
            self.width = w

# cuboid inherits from rectangles
class Cuboid(Rectangle):
    height = 1
    def volume(self):
        v = self.area() * self.height
        return v
    def __init__(self, l, w, h):
        if h < 0 or h > w:
            print("Incorrect arguments for constructor")
        else:
            self.height = h
        super().__init__(l,w)
    def area(self):
        sa = 2*((self.length * self.width) + (self.length * self.height) + (self.width * self.height))
        return sa

c = Cuboid(7, 3, 2)

z = c.area()
print(z)
z = c.volume()
print(z)
    
r = Rectangle(5,4)
#r.length = 7
#r.width = 5

z = r.area()
print(z)

'''
g = Rectangle(5,9)
g.a = 4
g.b = 3

z = g.area()
print(z)
'''
