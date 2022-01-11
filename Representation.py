from PIL import Image, ImageDraw
import numpy as np
class Representation:

    def __init__(self, size):
        self.size = size
        self.image = Image.new("RGB", self.size)
        self.writeImage = ImageDraw.Draw(self.image)

    def colorRepresentation(self, raster : np.array):
        for i in range(self.size[0]):
            for j in range(self.size[1]):
                if raster[i, j] == 0:
                    self.writeImage.point([i, j], (0,0,0))
                else:
                    self.writeImage.point([i, j], raster[i, j].color)

    def printRepresentation(self):
        self.image.resize((500, 500), Image.NEAREST).show()

