import numpy as np
class Line:
    def __init__(self, listPoints):
        self.X = np.mean([c.X for c in listPoints])
        self.Y = np.mean([c.Y for c in listPoints])
        self.witdh = 2
        vector = [listPoints[-1].X - listPoints[0].X,
                listPoints[-1].Y - listPoints[0].Y]
        self.length = vector / np.linalg.norm(vector)
        self.points = listPoints
        self.color = (255,0,0)