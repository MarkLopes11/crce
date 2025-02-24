#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Structure to represent an edge in the graph
struct Edge {
    int src, dest, weight;
};

// Structure to represent a graph
struct Graph {
    int V, E; // Number of vertices and edges in the graph
    struct Edge* edge; // Array of edges
};

// Function to create a graph with V vertices and E edges
struct Graph* createGraph(int V, int E) {
    // Allocate memory for the graph structure
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    // Set the number of vertices and edges
    graph->V = V;
    graph->E = E;
    // Allocate memory for the array of edges
    graph->edge = (struct Edge*)malloc(E * sizeof(struct Edge));
    return graph;
}

// A utility function to find the set of an element i
int find(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent, parent[i]); // Path compression
}

// A function that does union of two sets of x and y
void unionSet(int parent[], int rank[], int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);

    // Attach smaller rank tree under root of high rank tree (Union by Rank)
    if (rank[xroot] < rank[yroot])
        parent[xroot] = yroot;
    else if (rank[xroot] > rank[yroot])
        parent[yroot] = xroot;
    else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

// Boruvka's algorithm to find Minimum Spanning Tree of a given graph
void boruvkaMST(struct Graph* graph) {
    int V = graph->V; // Number of vertices
    int E = graph->E; // Number of edges
    struct Edge* edge = graph->edge; // Array of edges

    // Allocate memory for parent, cheapest, and rank arrays
    int* parent = (int*)malloc(V * sizeof(int));
    int* cheapest = (int*)malloc(V * sizeof(int));
    int* rank = (int*)malloc(V * sizeof(int));

    // Initialize sets
    for (int i = 0; i < V; i++) {
        parent[i] = i;
        rank[i] = 0;
        cheapest[i] = -1;
    }

    int numTrees = V; // Number of trees initially equals the number of vertices
    int MSTweight = 0; // Weight of the minimum spanning tree

    while (numTrees > 1) {
        // Traverse through all edges and update the cheapest of every component
        for (int i = 0; i < E; i++) {
            int set1 = find(parent, edge[i].src);
            int set2 = find(parent, edge[i].dest);

            if (set1 != set2) {
                if (cheapest[set1] == -1 || edge[cheapest[set1]].weight > edge[i].weight)
                    cheapest[set1] = i;

                if (cheapest[set2] == -1 || edge[cheapest[set2]].weight > edge[i].weight)
                    cheapest[set2] = i;
            }
        }

        // Consider the above picked cheapest edges and add them to MST
        for (int i = 0; i < V; i++) {
            if (cheapest[i] != -1) {
                int set1 = find(parent, edge[cheapest[i]].src);
                int set2 = find(parent, edge[cheapest[i]].dest);

                if (set1 != set2) {
                    MSTweight += edge[cheapest[i]].weight;
                    unionSet(parent, rank, set1, set2);
                    printf("Edge %d-%d with weight %d included in MST\n", edge[cheapest[i]].src, edge[cheapest[i]].dest, edge[cheapest[i]].weight);
                    numTrees--;
                }
            }
        }

        // Reset cheapest array
        for (int i = 0; i < V; i++)
            cheapest[i] = -1;
    }

    printf("Weight of MST is %d\n", MSTweight); // Print the weight of the minimum spanning tree
}

int main() {
    // Example graph represented as a matrix
    int graph[4][4] = {
        {0, 10, 6, 5},
        {10, 0, INT_MAX, 15},
        {6, INT_MAX, 0, 4},
        {5, 15, 4, 0}
    };
    int V = 4; // Number of vertices
    int E = 5; // Total edges

    // Create a graph
    struct Graph* g = createGraph(V, E);

    // Fill the graph with edges from the matrix
    int edgeCount = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (graph[i][j] != 0 && graph[i][j] != INT_MAX) {
                g->edge[edgeCount].src = i;
                g->edge[edgeCount].dest = j;
                g->edge[edgeCount].weight = graph[i][j];
                edgeCount++;
            }
        }
    }

    // Run Boruvka's algorithm
    boruvkaMST(g);

    return 0;
}
