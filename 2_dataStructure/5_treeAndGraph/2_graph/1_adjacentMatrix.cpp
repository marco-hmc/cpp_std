#include <iostream>
#include <vector>

class Graph {
  private:
    int numVertices;
    std::vector<std::vector<int>> adjMatrix;

  public:
    Graph(int numVertices) {
        this->numVertices = numVertices;
        adjMatrix.resize(numVertices, std::vector<int>(numVertices, 0));
    }

    void addEdge(int src, int dest) {
        adjMatrix[src][dest] = 1;
        adjMatrix[dest][src] = 1;
    }

    void printGraph() {
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++) {
                std::cout << adjMatrix[i][j] << " ";
            }
            std::cout << '\n';
        }
    }
};

int main() {
    Graph graph(5);

    graph.addEdge(0, 1);
    graph.addEdge(0, 4);
    graph.addEdge(1, 2);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);

    graph.printGraph();

    return 0;
}