from Representation import *
from Line import *


class Model:
    def __init__(self,size):
        self.size = size
        self.raster = np.zeros(self.size, dtype=object)
        self.visual = Representation(self.size)

    def bresenhamAlgorithm(self, x1, x2, y1, y2, newObject):
        m_new = 2 * (y2 - y1)
        slope_error_new = m_new - (x2 - x1)
        y = y1
        for x in range(x1, x2 + 1):
            self.raster[x, y] = newObject
            # Add slope to increment angle formed
            slope_error_new = slope_error_new + m_new
            # Slope error reached limit, time to
            # increment y and update slope error.
            if (slope_error_new >= 0):
                y = y + 1
                slope_error_new = slope_error_new - 2 * (x2 - x1)

    def addLineInRaster(self, newObject: Line):
        points = sorted(newObject.points, key=lambda k: [k.X, k.Y])
        for i in range(len(points) - 1):
            x1 = points[i].X
            y1 = points[i].Y
            x2 = points[i + 1].X
            y2 = points[i + 1].Y
            if x1 == x2:  # cas ligne verticale
                for y in range(y1, y2 + 1):
                    self.raster[x1, y] = newObject
            else:
                self.bresenhamAlgorithm(x1, x2, y1, y2, newObject)

    def addPointInRaster(self, newObject):
        self.raster[newObject.X, newObject.Y] = newObject


    def __repr__(self):
        self.visual.printRepresentation()
        return ""