class Point:
    def __init__(self, x, y):
        self.x = x 
        self.y = y


l = list()
for r in range(3000):
    for c in range(3000):
        p = Point(r, c)
        l.append(p)

