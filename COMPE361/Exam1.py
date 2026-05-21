p = int(input())

if p > 100:
    print("Incorrect grade")
elif p >= 94:
    print("A")
elif p >= 90:
    print("A-")
elif p >= 87:
    print("B+")
elif p >= 84:
    print("B")
elif p >= 80:
    print("B-")
elif p >= 77:
    print("C+")
elif p >= 74:
    print("C")
elif p >= 70:
    print("C-")
elif p >= 0:
    print("Not passed")
else:
    print("Incorrect grade")


def MaxMin4(a,b,c,d):
    max4 = a
    min4 = a
    if b > max4:
        max4 = b
    elif b < min4:
        min4 = b
    if c > max4:
        max4 = c
    elif c < min4:
        min4 = c
    if d > max4:
        max4 = d
    elif d < min4:
        min4 = d
    return max4 - min4

r = MaxMin4(7,5,9,3)
print(r)


class Country:
    def __init__(self, name, population, area):
        self.name = name
        self.population = population
        self.area = area
    def density(self):
        return self.population / self.area

c = Country("USA", 300000000, 9000000)
d = c.density()
print(d)


class Vehicle:
    name = "Mercedes"
    speed = 300
    weight = 2500
    def __init__(self, name, speed, weight):
        self.name = name
        self.speed = speed
        self.weight = weight
    def echo(self):
        print(self.name)
        print(self.speed)
        print(self.weight)

class Bus(Vehicle):
    NumberOfPassengers = 1
    def __init__(self, name, speed, weight, passengers):
        super().__init__(name, speed, weight)
        self.NumberOfPassengers = passengers
    def echo(self):
        super().echo()
        print(self.NumberOfPassengers)
        
v = Vehicle("Cadillac", 250, 3500)
v.echo()

b = Bus("Man", 150, 4000, 100)
b.echo()


class Date:
    year = 1
    month = 1
    day = 1
    def __init__(self, y, m, d):
        if y == 0:
            raise Exception("Year cannot be zero")
        if m < 1 or m > 12:
            raise Exception("Month must be between 1 and 12")
        if d < 1 or d > 31:
            raise Exception("Day must be between 1 and 31")
        self.year = y
        self.month = m
        self.day = d
    def __str__(self):
        return str(self.year) + '-' + str(self.month) + '-' + str(self.day)

try:
    u = Date(2024, 1, 3)
    print(u)
except BaseException as ex:
    print(ex)


w = int(input("Please enter weekday number"))
match w:
    case 1: print("Sunday")
    case 2: print("Monday")
    case 3: print("Tuesday")
    case 4: print("Wednesday")
    case 5: print("Thursday")
    case 6: print("Friday")
    case 7: print("Saturday")
    case _: print("Incorrect Day Number")
