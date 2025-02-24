class PlacedBlock:
    def __init__(self, hole_size, block_size):
        self.hole_size = hole_size
        self.block_size = block_size

    def remaining_space(self):
        return self.hole_size - self.block_size


def best_fit_allocation(holes, blocks):
    placements = []

    for block in blocks:
        best_fit_index = -1

        # Find the best fit hole for the current block
        for i, hole in enumerate(holes):
            if hole >= block:
                if best_fit_index == -1 or hole < holes[best_fit_index]:
                    best_fit_index = i

        if best_fit_index != -1:
            # Allocate the block to the best fit hole
            allocated_hole_size = holes[best_fit_index]
            placements.append(PlacedBlock(allocated_hole_size, block))
            holes[best_fit_index] -= block  # Update the hole size after allocation

    # Calculate total remaining space in all holes after allocation
    total_remaining_space = sum(hole for hole in holes)
    return placements, total_remaining_space


def main():
    # Initial setup
    holes = [100, 125, 150, 80, 50, 90]
    blocks = [90, 70, 140, 120, 20, 10]

    # Perform Best Fit allocation
    allocated_blocks, remaining_space = best_fit_allocation(holes, blocks)

    # Display results
    print("Allocated Blocks:")
    for placement in allocated_blocks:
        print(f"Block Size: {placement.block_size}, Hole Size After Allocation: {placement.remaining_space()}")

    print(f"\nTotal Remaining Space in Holes: {remaining_space}")


if __name__ == "__main__":
    main()
