
## 四叉树

四叉树是一种树数据结构，其中每个内部节点恰好有四个子节点。它们通常用于通过递归将二维空间划分为四个象限或区域来进行分区。每个子节点或叶节点存储空间信息。四叉树是 [八叉树](https://en.wikipedia.org/wiki/Octree) 的二维类比，八叉树用于分区三维空间。

![quadtree](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-IV/geohashing-and-quadtrees/quadtree.png)

### 四叉树的类型

四叉树可以根据它们表示的数据类型进行分类，包括区域、点、线和曲线。以下是一些常见的四叉树类型：

- 点四叉树
- 点区域 (PR) 四叉树
- 多边形地图 (PM) 四叉树
- 压缩四叉树
- 边四叉树

### 为什么我们需要四叉树？

仅使用纬度和经度不够吗？为什么我们需要四叉树？虽然理论上使用纬度和经度我们可以通过[欧几里得距离](https://en.wikipedia.org/wiki/Euclidean_distance)确定点之间的距离，但对于实际用例来说，由于其对大数据集的 CPU 密集性，这种方法并不具有可扩展性。

![quadtree-subdivision](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-IV/geohashing-and-quadtrees/quadtree-subdivision.png)

四叉树使我们能够高效地在二维范围内搜索点，这些点定义为纬度/经度坐标或笛卡尔 (x, y) 坐标。此外，我们可以通过仅在达到某个阈值后细分节点来节省进一步的计算。通过应用诸如 [Hilbert 曲线](https://en.wikipedia.org/wiki/Hilbert_curve) 之类的映射算法，我们可以轻松提高范围查询性能。

### 用例

以下是四叉树的一些常见用例：

- 图像表示、处理和压缩。
- 空间索引和范围查询。
- 基于位置的服务，如 Google 地图、Uber 等。
- 网格生成和计算机图形学。
- 稀疏数据存储。

