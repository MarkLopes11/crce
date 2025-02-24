#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

int minDistance(int dist[], bool visited[], int V)
{
    int min = INT_MAX;
    int min_index;

    for (int i = 0; i < V; i++)
    {
        if (visited[i] == false && dist[i] <= min)
        {
            min = dist[i];
            min_index = i;
        }
    }
    return min_index;
}

void printSolution(int dist[], int parent[], int V, int src)
{
    printf("Vertex\t\tDustance from source\t\t Path\n");
    for (int i = 0; i < V; i++)
    {
        printf("%d\t\t\t %d \t\t\t", i + 1, dist[i]);

        int current = i;
        while (current != src)
        {
            printf("%d <- ", current + 1);
            current = parent[current];
        }
        printf("%d", src + 1);
        printf("\n");
    }
}

void dijstra(int V, int graph[V][V], int src)
{
    int dist[V];     // stortest distance from the source node to the current node
    int parent[V];   // parent of each node
    bool visited[V]; // if true then node is visited

    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;  // make all the distance INF
        visited[i] = false; // make visited false       
        parent[i] = -1;     // make parent as -1
    }
    dist[src] = 0; // dist from src to itself will be 0

    for (int count = 0; count < V - 1; count++)
    {
        int u = minDistance(dist, visited, V); // gives the shortest path's index

        visited[u] = true; // make the shortest path's index true

        for (int v = 0; v < V; v++) // check the adjacent of the shortest index
        {                           // they should not be in visited and the distance from current node to the adjacent should not be INF(dist[u][v]), then we update
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }
    printSolution(dist, parent, V, src);
}

int main()
{
    int V; // number of vertices
    printf("Enter the no of vertices of the graph: ");
    scanf("%d", &V);

    printf("Enter the values of the adjanceny matrix: ");
    int graph[V][V];

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            scanf("%d", &graph[i][j]);
        }
    }

    dijstra(V, graph, 0);
}