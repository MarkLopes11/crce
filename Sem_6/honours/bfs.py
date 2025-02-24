import heapq

def best_first_search(graph, heuristic, start, goal):
    # Priority queue to select the node with the lowest heuristic value
    open_list = []
    heapq.heappush(open_list, (heuristic[start], start))
    
    # Dictionary to keep track of the path
    came_from = {}
    came_from[start] = None
    
    # Set to keep track of visited nodes
    visited = set()

    while open_list:
        # Get the node with the lowest heuristic value
        current_cost, current_node = heapq.heappop(open_list)
        
        # If goal is reached, construct the path
        if current_node == goal:
            path = []
            while current_node:
                path.append(current_node)
                current_node = came_from[current_node]
            path.reverse()
            return path
        
        visited.add(current_node)
        
        # Explore neighbors
        for neighbor in graph[current_node]:
            if neighbor not in visited:
                came_from[neighbor] = current_node
                heapq.heappush(open_list, (heuristic[neighbor], neighbor))
                
    return None  # Return None if no path is found

# Example graph as an adjacency list
graph = {
    'S': ['A', 'B'],
    'B': ['C'],
    'B': ['D'],
    'C': ['G'],
    'D': ['G'],
    'G':[]
    }

# heuristic function
heuristic = {
    'S':11.18,
    'A': 8.06,
    'B': 6.4,
    'C': 4.12,
    'D': 3.61,
    'G': 0  # Goal node has a heuristic 0
}

# Define start and goal nodes
start_node = 'S'
goal_node = 'G'

# Call the Best-First Search function
path = best_first_search(graph, heuristic, start_node, goal_node)

# Display the result
if path:
    print(f"Path from {start_node} to {goal_node}: {' -> '.join(path)}")
else:
    print(f"No path found from {start_node} to {goal_node}")
