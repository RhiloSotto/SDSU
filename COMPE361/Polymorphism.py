from abc import ABC, abstractmethod


class Piece(ABC):
    x = 0
    y = 0

    def __init__(self, x, y):
        self.x = x
        self.y = y
    def move(self, x, y):
        if self.verify(x,y):
            self.x = x
            self.y = y
    def display(self):
        print(self.x, end='')
        print(',', end='')
        print(self.y)
    @abstractmethod
    def verify(self, x, y):
        pass


class Rook(Piece):
    def verify(self, x, y):
        return self.x == x or self.y == y
    
class Bishop(Piece):
    def verify(self, x, y):
        return abs(x - self.x) == abs(y - self.y)

p = Bishop(3,1) # in case of piece it will not work
p.display()
p.move(5,4)
p.display()
