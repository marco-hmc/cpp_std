#include <climits>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// 要在n个城市之间铺设光缆,主要目标是要使这
// n个城市的任意两个之间都可以通信,但铺设光缆的费用很高,且各个城市之间铺设光缆的费用不同,
// 因此另一个目标是要使铺设光缆的总费用最低.这就需要找到带权的最小生成树.

// 定义图的边
struct Edge {
    int src, dest, weight;
};

// 定义比较函数用于最小堆
struct Compare {
    bool operator()(const Edge &e1, const Edge &e2) {
        return e1.weight > e2.weight;
    }
};

// Prim算法实现最小生成树
void primMST(vector<vector<int>> &graph) {
    int V = graph.size();  // 图的顶点数

    // 创建一个最小堆来存储边
    priority_queue<Edge, vector<Edge>, Compare> minHeap;

    // 创建一个数组来存储最小生成树的顶点
    vector<int> parent(V, -1);

    // 创建一个数组来存储顶点到最小生成树的距离
    vector<int> key(V, INT_MAX);

    // 创建一个数组来标记顶点是否已经包含在最小生成树中
    vector<bool> inMST(V, false);

    // 将第一个顶点加入最小生成树
    key[0] = 0;
    minHeap.push({0, 0, 0});

    while (!minHeap.empty()) {
        // 从最小堆中取出权重最小的边
        Edge curr = minHeap.top();
        minHeap.pop();

        int u = curr.dest;

        // 如果顶点已经包含在最小生成树中,则跳过
        if (inMST[u]) continue;

        inMST[u] = true;

        // 遍历与当前顶点相邻的顶点
        for (int v = 0; v < V; v++) {
            // 如果v与u相邻且v未包含在最小生成树中,并且权重小于当前距离,则更新最小生成树
            if (graph[u][v] && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
                minHeap.push({u, v, graph[u][v]});
            }
        }
    }

    // 输出最小生成树的边
    cout << "最小生成树的边:" << endl;
    for (int i = 1; i < V; i++) {
        cout << parent[i] << " - " << i << endl;
    }
}

int main() {
    // 创建一个图的邻接矩阵表示
    vector<vector<int>> graph = {{0, 2, 0, 6, 0},
                                 {2, 0, 3, 8, 5},
                                 {0, 3, 0, 0, 7},
                                 {6, 8, 0, 0, 9},
                                 {0, 5, 7, 9, 0}};

    // 调用Prim算法找到最小生成树
    primMST(graph);

    return 0;
}
