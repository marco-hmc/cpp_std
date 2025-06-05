#include <iostream>
#include <vector>

struct Edge {
    int src, dest;
};

class Graph {
  public:
    Graph(const std::vector<Edge> &edges, int numVertices) {
        this->numVertices = numVertices;

        adjList.resize(numVertices);
        for (const auto &edge : edges) {
            int src = edge.src;
            int dest = edge.dest;

            adjList[src].push_back(dest);
            adjList[dest].push_back(src);
        }
    }

    void printGraph() {
        for (int i = 0; i < numVertices; i++) {
            std::cout << "顶点 " << i << " 的邻接顶点: ";
            for (const auto &vertex : adjList[i]) {
                std::cout << vertex << " ";
            }
            std::cout << std::endl;
        }
    }

  private:
    int numVertices;
    std::vector<std::vector<int>> adjList;
};

int main() {
    std::vector<Edge> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}};
    Graph graph(edges, 5);
    graph.printGraph();

    return 0;
}