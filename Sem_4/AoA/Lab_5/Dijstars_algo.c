#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

// A utility function to find the vertex with minimum distance value,
// from the set of vertices not yet included in the shortest path tree
int minDistance(int dist[], bool visited[], int V) {
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (visited[v] == false && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// A utility function to print the constructed distance array along with paths
void printSolution(int dist[], int parent[], int V, int src) {
    printf("Vertex \t\t Distance from Source \t Path\n");
    for (int i = 0; i < V; i++) {
        printf("%d \t\t\t %d \t\t\t ", i + 1, dist[i]);
        
        // Print the path
        int current = i;
        while (current != src) {
            printf("%d <- ", current + 1);
            current = parent[current];
        }
        printf("%d", src + 1);
        printf("\n");
    }
}

// Function that implements Dijkstra's single source shortest path algorithm
// for a graph represented using adjacency matrix representation
void dijkstra(int V, int graph[V][V], int src) {
    int dist[V];    // The output array. dist[i] will hold the shortest distance from src to i
    int parent[V];  // Array to store the parent of each vertex in the shortest path
    bool visited[V]; // visited[i] will be true if vertex i is included in the shortest
                    // path tree or shortest distance from src to i is finalized

    // Initialize all distances as INFINITE, visited[] as false, and parent[] as -1
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
        parent[i] = -1;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not yet processed
        int u = minDistance(dist, visited, V);

        // Mark the picked vertex as processed
        visited[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex
        for (int v = 0; v < V; v++) {
            // Update dist[v] only if it is not in visited, there is an edge from u to v,
            // and the total weight of the path from src to v through u is smaller than the current value of dist[v]
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    // Print the constructed distance array along with paths
    printSolution(dist, parent, V, src);
}

// Driver's code
int main() {
    int V;
    printf("Enter the number of vertices in the graph: ");
    scanf("%d", &V);

    printf("Enter the values of the adjacency matrix:\n");
    int graph[V][V];
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            scanf("%d", &graph[i][j]);
        }
    }

    // Function call
    dijkstra(V, graph, 0);

    return 0;
}
