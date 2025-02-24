class PlacedBlock:
    def __init__(self, hole_size, block_size):
        self.hole_size = hole_size
        self.block_size = block_size

    def remaining_space(self):
        return self.hole_size - self.block_size


def first_fit_allocation(holes, blocks):
    placements = []

    for block in blocks:
        allocated = False

        # Iterate through each hole to find the first fit
        for i, hole in enumerate(holes):
            if hole >= block:
                # Allocate the block to the current hole
                placements.append(PlacedBlock(hole, block))
                holes[i] -= block  # Update the hole size after allocation
                allocated = True
                break  # Stop searching for holes after first fit

        if not allocated:
            # If no hole can accommodate the block, mark as not allocated
            placements.append(PlacedBlock(0, block))  # Hole size of 0 indicates not allocated

    # Calculate total remaining space in all holes after allocation
    total_remaining_space = sum(holes)
    return placements, total_remaining_space


def main():
    holes = [100, 125, 150, 80, 50, 90]
    blocks = [90, 70, 140, 120, 20, 10]

    # Perform First Fit allocation
    allocated_blocks, remaining_space = first_fit_allocation(holes[:], blocks)

    # Display results
    print("Allocated Blocks:")
    for placement in allocated_blocks:
        if placement.hole_size > 0:
            print(f"Block Size: {placement.block_size}, Hole Size After Allocation: {placement.remaining_space()}")
        else:
            print(f"Block Size: {placement.block_size}, Not Allocated")

    print(f"\nTotal Remaining Space in Holes: {remaining_space}")


if __name__ == "__main__":
    main()
