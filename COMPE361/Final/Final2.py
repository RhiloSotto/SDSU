 
class MyTime:
    def __init__(self, hour, minute, second):
        self.hour = hour
        self.minute = minute
        self.second = second
    def __str__(self):
        return str(self.hour) + ":" + str(self.minute) + ":" + str(self.second)


u = MyTime(7, 8, 53)

txt = str(u)

#txt = u.__str__()

print(txt)

print(u)
