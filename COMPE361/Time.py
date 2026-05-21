class Time:
    ticks = 0
    def __init__(self, h, m, s):
        self.ticks = (h * 60 + m) * 60 + s
        
    def gethour(self):
        return self.ticks // 3600
    def getminute(self):
        return (self.ticks // 60) % 60
    def getsecond(self):
        return self.ticks % 60

    def sethour(self, h):
        self.ticks -= self.gethour() * 3600
        self.ticks += h * 3600
    def setminute(self, m):
        self.ticks -= self.getminute() * 60
        self.ticks += m * 60
    def setsecond(self, s):
        self.ticks -= self.getsecond()
        self.ticks += s
        
    def __str__(self):
        return str(self.gethour()) + ":" + str(self.getminute()) + ":" + str(self.getsecond())
    hour = property(fget=gethour, fset=sethour)
    minute = property(fget=getminute, fset=setminute)
    second = property(fget=getsecond, fset=setsecond)

# properties are like fields, but a method is called instead
# compiler calls the method
# properties need a getter function and a setter function
# getter functions must only have self as argument and return a value
# setter functions must have self, and another argument
u = Time(19,25,31);
h = u.gethour()
m = u.getminute()
s = u.getsecond()

print(h)
print(m)
print(s)

print(u)

# properties look like fields, and are accessed like fields
h = u.hour
print(h)

u.hour = 11
h = u.hour
print(h)
print(m)
print(s)

u.minute = 2
u.second = 5
print(u)
