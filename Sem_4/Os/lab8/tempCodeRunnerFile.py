holes = [100, 125, 150, 80, 50, 90]
blocks = [90, 70, 140, 120, 20, 10]


class Placed:
    def __init__(self, hole, block):
        self.hole = hole
        self.block = block

    def sizeLeft(self):
        return self.hole - self.block


placements = []
for block in blocks:
    for i, hole in enumerate(holes):
        if hole >= block:
            placements.append(Placed(holes[i], block))
            holes[i] -= block
            break

sumi = sum([placement.sizeLeft() for placement in placements])
print(f"Space left is {sumi}")
