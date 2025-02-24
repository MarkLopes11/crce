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
    worst_fit_index = -1
    for i, hole in enumerate(holes):
        if hole >= block:
            if worst_fit_index == -1 or hole > holes[worst_fit_index]:
                worst_fit_index = i
    if worst_fit_index != -1:
        placements.append(Placed(holes[worst_fit_index], block))
        holes[worst_fit_index] -= block

sumi = sum([placement.sizeLeft() for placement in placements])
print(f"Space left is {sumi}")
