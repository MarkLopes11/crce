class PlacedBlock:
    def __init__(self, hole_size, block_size):
        self.hole_size = hole_size
        self.block_size = block_size

    def remaining_space(self):
        return self.hole_size - self.block_size


def worst_fit_allocation(holes, blocks):
    placements = []

    for block in blocks:
        worst_fit_index = -1

        # Find the largest hole that can accommodate the block
        for i, hole in enumerate(holes):
            if hole >= block:
                if worst_fit_index == -1 or hole > holes[worst_fit_index]:
                    worst_fit_index = i

        if worst_fit_index != -1:
            # Allocate the block to the largest hole found
            placements.append(PlacedBlock(holes[worst_fit_index], block))
            holes[worst_fit_index] -= block  # Update the hole size after allocation

    # Calculate total remaining space in all holes after allocation
    total_remaining_space = sum(holes)
    return placements, total_remaining_space


def main():
    holes = [100, 125, 150, 80, 50, 90]
    blocks = [90, 70, 140, 120, 20, 10]

    # Perform Worst Fit allocation
    allocated_blocks, remaining_space = worst_fit_allocation(holes[:], blocks)

    # Display results
    print("Allocated Blocks:")
    for placement in allocated_blocks:
        print(f"Block Size: {placement.block_size}, Hole Size After Allocation: {placement.remaining_space()}")

    print(f"\nTotal Remaining Space in Holes: {remaining_space}")



if __name__ == "__main__":
    main()
