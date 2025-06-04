#include <iostream>
#include <queue>
#include <vector>

class Graph {
  private:
    int numVertices;
    std::vector<std::vector<int>> adjLists;

  public:
    explicit Graph(int vertices) : numVertices(vertices) {
        adjLists.resize(vertices);
    }

    void addEdge(int src, int dest) {
        adjLists[src].push_back(dest);
        adjLists[dest].push_back(src);
    }

    void BFS(int startVertex) {
        std::vector<bool> visited(numVertices, false);
        std::queue<int> queue;

        visited[startVertex] = true;
        queue.push(startVertex);

        while (!queue.empty()) {
            int currVertex = queue.front();
            std::cout << currVertex << " ";
            queue.pop();

            for (int adjVertex : adjLists[currVertex]) {
                if (!visited[adjVertex]) {
                    visited[adjVertex] = true;
                    queue.push(adjVertex);
                }
            }
        }
    }
};

int main() {
    Graph graph(6);

    graph.addEdge(0, 1);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(2, 4);
    graph.addEdge(3, 4);
    graph.addEdge(3, 5);
    graph.addEdge(4, 5);

    std::cout << "BFS遍历结果: ";
    graph.BFS(0);

    return 0;
}
