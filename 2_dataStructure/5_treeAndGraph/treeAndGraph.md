# 红黑树

* 定义及性质

  * 一般的,红黑树,满足以下性质,即只有满足以下全部性质的树,我们才称之为红黑树:
    * 每个结点要么是红的,要么是黑的
    * 根结点是黑的
    * 每个叶结点(叶结点即指树尾端NIL指针或NULL结点)是黑的
    * 如果一个结点是红的,那么它的俩个儿子都是黑的
    * 对于任一结点而言,其到叶结点树尾端NIL指针的每一条路径都包含相同数目的黑结点

* **红黑树的各种操作的时间复杂度都是$\textbf{O}(\log n)$**

* **红黑树相比于BST和AVL树有什么优点?**

  * 红黑树是**牺牲了严格的高度平衡的优越条件**为代价,它只要求部分地达到平衡要求,降低了对旋转的要求,从而提高了性能.红黑树能够以$\textbf{O}(\log n)$的时间复杂度进行搜索/插入/删除操作.此外,**由于它的设计,任何不平衡都会在三次旋转之内解决**.当然,还有一些更好的,但实现起来更复杂的数据结构能够做到一步旋转之内达到平衡,但红黑树能够给我们一个比较"便宜"的解决方案
  * 相比于BST,因为**红黑树可以能确保树的最长路径不大于两倍的最短路径的长度**,所以可以看出它的查找效果是有最低保证的.在最坏的情况下也可以保证$\textbf{O}(\log n)$的,这是要好于二叉查找树的.因为二叉查找树最坏情况可以让查找达到$\textbf{O}(n)$
  * 红黑树的算法时间复杂度和AVL相同,但统计性能比AVL树更高,所以在插入和删除中所做的后期维护操作肯定会比红黑树要耗时好多,但是他们的查找效率都是$\textbf{O}(\log n)$,所以红黑树应用还是高于AVL树的.**实际上插入 AVL 树和红黑树的速度取决于你所插入的数据.如果你的数据分布较好,则比较宜于采用 AVL树(例如随机产生系列数),但是如果你想处理比较杂乱的情况,则红黑树是比较快的**

* **红黑树相对于哈希表,在选择使用的时候有什么依据?**

  * 权衡三个因素: 查找速度,数据量,内存使用,可扩展性
  * 总体来说,hash查找速度会比map快,而且查找速度基本和数据量大小无关,属于常数级别;而map的查找速度是log(n)级别.并不一定常数就比log(n) 小,hash还有hash函数的耗时.如果考虑效率,特别是在元素达到一定数量级时,考虑使用hash.但若你对内存使用特别严格, 希望程序尽可能少消耗内存,那么一定要小心,hash可能会让你陷入尴尬,特别是当你的hash对象特别多时,你就更无法控制了,而且 hash的构造速度较慢

  * 红黑树并不适应所有应用树的领域.如果数据基本上是静态的,那么让他们待在他们能够插入,并且不影响平衡的地方会具有更好的性能.如果数据完全是静态的,例如,做一个哈希表,性能可能会更好一些
  * 红黑树天生有序,而对哈希表排序比较麻烦,涉及范围查找,或者排序,红黑树更优

  * 在实际的系统中,例如,需要使用动态规则的防火墙系统,使用红黑树而不是散列表被实践证明具有更好的伸缩性.Linux内核在管理vm_area_struct时就是采用了红黑树来维护内存块的

  * 红黑树通过扩展节点域可以在不改变时间复杂度的情况下得到结点的秩

# [跳表](https://zhuanlan.zhihu.com/p/54869087)

* https://www.jianshu.com/p/9d8296562806


* 图的结构有哪些?
* 图是一种非常通用的数据结构,可以用于表示任何的二元关系.以下是一些常见的使用图的场景:

- 社交网络:在社交网络中,每个人可以被表示为一个节点,如果两个人是朋友,那么他们之间就有一条边.
- 网络:在计算机网络或互联网中,每台计算机或路由器可以被表示为一个节点,如果两台设备直接相连,那么他们之间就有一条边.
- 地图:在地图中,每个地点可以被表示为一个节点,如果两个地点之间有直接的道路,那么他们之间就有一条边.

以下是一些常见的图相关的数据结构:

- 邻接矩阵:邻接矩阵是一种简单且直观的图的表示方法.它是一个二维数组,其中的元素表示相应节点之间是否存在边.
- 邻接表:邻接表是一种更为紧凑的图的表示方法.它是一个数组,数组的每个元素是一个列表,表示与该节点相连的所有节点.
- 边列表:边列表是一种直接表示图中所有边的方法.它是一个列表,列表的每个元素是一个表示边的对象,该对象包含了边的两个端点.
- 邻接多重表:邻接多重表是一种可以同时表示无向图和有向图的数据结构.它是一个数组,数组的每个元素是一个列表,表示与该节点相连的所有边,每条边都有一个标记表示它的方向.

选择哪种数据结构来表示图取决于你的具体需求,例如,如果图的节点和边的数量非常大,那么邻接表可能是一个更好的选择,因为它比邻接矩阵更为紧凑.



# 图的基本概念和术语
1. 图的定义
$$\begin{aligned}
&G=(V,E) \\
&V\text{(Vertex):顶点(数据元素)的有穷非空集合;} \\
&E\text{(Edge):边的有穷集合.}
\end{aligned}$$
2. 有向图:图中每条边都是有方向的,有向图的边也称作"弧"
3. 无向图:图中每条边都是无方向的
4. 完全图:图中任意两个顶点之间都有一条边相连.完全图分为有向完全图和无向完全图,如下图示
<img src="https://img-blog.csdnimg.cn/20201023102435499.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =500x" alt="在这里插入图片描述" style="zoom:67%;" />
若图中有$n$个顶点,无向完全图共有$C_n^2=\frac{n(n-1)}{2}$条边,而有向完全图共有$2C_n^2=n(n-1)$条边.
5. 稀疏图:有很少边或弧的图($e<n\log n$),$e$为边或弧的数目,$n$为顶点数目
6. 稠密图:有较多边或弧的图
7. 网:边/弧带权的图
8. 邻接:有边/弧相连的两个顶点之间的关系.在无向图中,存在$(V_i,V_j)$,则称$V_i$和$V_j$**互为邻接点**;同样,在有向图中,存在$<V_i,V_j>$,则称$V_i$**邻接到**$V_j$,$V_j$**邻接于**$V_i$.
9. 关联(依附):边/弧与顶点之间的关系.存在$(V_i,V_j)$/$<V_i,V_j>$,则称该边关联于$V_i$和$V_j$.
10. 顶点的度:与某顶点$v$相关联的边/弧的数目,记为TD($v$).在有向图中,顶点的度等于该顶点的**入度**和**出度**之和.顶点$v$的入度是以$v$为终点的有向边的条数,记作ID($v$);而顶点$v$的出度是以$v$为始点的有向边的条数,记作OD($v$).
11. 路径:接续的边构成的顶点序列.
12. 路径长度:路径上的边/弧数目,或权值之和.
13. 回路(环):第一个顶点和最后一个顶点相同的路径.
14. 简单路径:除路径起点和终点可以相同外,其他顶点均不相同的路径.
15. 简单回路(简单环):除路径起点和终点相同外,其余顶点均不相同的路径.
<img src="https://img-blog.csdnimg.cn/20201023233353684.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =500x" alt="在这里插入图片描述" style="zoom: 50%;" />
16. 连通图/强连通图:在无/有向图$G=(V,\{E\})$中,若对任意两个顶点$v,u$都存在从$v$到$u$的路径,则称$G$是连通图/强连通图.
<img src="https://img-blog.csdnimg.cn/20201023234146290.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =500x" alt="在这里插入图片描述" style="zoom: 50%;" />
17. 权与网:图中边或弧所具有的相关系数称为权.**带权的图称为网**.
18. 子图:设有两个图$G=(V,\{E\})$,$G_1=(V_1,\{E_1\})$,若$V_1\subseteq V$,$E_1\subseteq E$,则称$G_1$是$G$的子图.
<img src="https://img-blog.csdnimg.cn/20201023234754289.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =500x" alt="在这里插入图片描述" style="zoom:50%;" />
19. 极大连通子图:若某子图$G_1$是$G$的连通子图,若$G$的任一不在$G_1$的顶点加入$G_1$,得到的子图不再连通,则称$G_1$为$G$的极大连通子图.
20. 连通分量:无向图$G$的极大连通子图称为$G$的连通分量.
<img src="https://img-blog.csdnimg.cn/20201023235549327.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =500x" alt="在这里插入图片描述" style="zoom:50%;" />
21. 极大强连通子图:若某子图$G_1$是$G$的强连通子图,若$G$的任一不在$G_1$的顶点加入$G_1$,得到的子图不再强连通,则称$G_1$为$G$的极大连通子图.
22. 强连通分量:有向图$G$的极大强连通子图称为$G$的强连通分量.
<img src="https://img-blog.csdnimg.cn/20201024000013317.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =500x" alt="在这里插入图片描述" style="zoom:50%;" />
23. 极小连通子图:若某子图$G_1$是$G$的连通子图,若删除$G_1$的任一条边,得到的子图不再连通,则称$G_1$为$G$的极小连通子图.
24. 生成树:包含无向图$G$所有顶点的极小连通子图.
25. 生成森林:对非连通图,由各个连通分量的生成树组成的集合.
<img src="https://img-blog.csdnimg.cn/20201024000619100.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =500x" alt="在这里插入图片描述" style="zoom:50%;" />
# 图的存储结构
1. 数组表示法:邻接矩阵
2. 链表存储结构:邻接表/邻接多重表/十字链表
## 邻接矩阵表示法
表示方法:建立一个**顶点表**(一维数组),记录顶点信息,再建立一个**邻接矩阵**(二维数组)表示各个顶点之间的关系.设图$G=(V,E)$​有$n$​个顶点,则顶点表 Vtxs[$n$​ ]为


|    $i$     |  $0$  |  $1$  |  $2$  |   ...   | $n-1$ |
| :--------: | :---: | :---: | :---: | :-----: | :---: |
| Vtxs[$i$ ] | $v_1$ | $v_2$ | $v_3$ | **...** | $v_n$ |

而图的邻接矩阵arcs\[$n$​][$n$]定义为:
$$
\text{arcs}[i][j]= 
	\begin{cases}1,\quad  & 
	  \text{如果}(v_i,v_j)/<v_i,v_j>\in E,\\ 
	  0, \quad  & \text{否则}.
	\end{cases}
$$

### 无向图的邻接矩阵表示法
<img src="https://img-blog.csdnimg.cn/20201025112835482.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =550x" alt="在这里插入图片描述" style="zoom:50%;" />
	特点1:无向图的邻接矩阵是对称的.
	特点2:顶点$v_i$的度是第$i$行(列)中1的个数.
	特别地,完全图的邻接矩阵对角线元素为0,其余为1.

### 有向图的邻接矩阵表示法
<img src="https://img-blog.csdnimg.cn/20201025115318630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =550x" alt="在这里插入图片描述" style="zoom:50%;" />
在有向图的邻接矩阵中,
第$i$行的含义:以顶点$v_i$为尾的弧(即出度边);
第$i$列的含义:以顶点$v_i$为头的弧(即入度边).
特点1:有向图的邻接矩阵可能是不对称的.
特点2:顶点$v_i$的出度是第$i$行元素之和;顶点$v_i$的入度是第$i$列元素之和;所以顶点$v_i$的度=第$i$行元素之和 + 第$i$列元素之和.

### 网(即有权图)的邻接矩阵表示法
$$
	\text{arcs}[i][j]= 
	\begin{cases}w_{ij},\quad  & 
	  \text{如果}(v_i,v_j)/<v_i,v_j>\in E,\\ 
	  \infty, \quad  & \text{无边弧}.
	\end{cases} 
$$
<img src="https://img-blog.csdnimg.cn/20201025124422848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =550x" alt="在这里插入图片描述" style="zoom:50%;" />

### 邻接矩阵表示法优缺点
优点:
* 直观/简单/好理解
* 方便检查任意一对顶点之间是否存在边
* 方便找任一顶点的所有邻接点(有边直接相连的顶点)
* 方便求得任一顶点的度

缺点:
* 不利于增加和删除节点
* 浪费空间,存稀疏图时有大量无效元素
* 统计稀疏图总边数时浪费时间

## 邻接表表示法(链表)
邻接表表示方法:
* **顶点**存储:按编号顺序将顶点数据存储在一维数组中
* **关联同一顶点的边**(以顶点为尾的弧)的存储:用线性链表存储
<img src="https://img-blog.csdnimg.cn/20201026204620875.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =550x" alt="在这里插入图片描述" style="zoom:50%;" />
### 无向图邻接表表示法
<img src="https://img-blog.csdnimg.cn/20201026205100440.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =550x" alt="在这里插入图片描述" style="zoom:50%;" />
特点:

* 邻接表**不唯一**
* 若无向图有$n$个节点,$e$条边,则其邻接表需要$n$个头节点和$2e$个表节点.适合存储稀疏图.
* 无向图中节点$v_i$的**度**为第$i$个存储节点关联边的**单链表的节点数**.
* 计算节点的度容易
### 有向图邻接表表示法
有向图邻接表的表示方法与无向图的表示方法类似,但只将以对应节点为尾的弧存储在单链表中.如下图示
<img src="https://img-blog.csdnimg.cn/20201026211415152.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =400x" alt="在这里插入图片描述" style="zoom:50%;" />
特点: 

* 若无向图有$n$个节点,$e$条边,则其邻接表需要$n$个头节点和$e$个表节点.
* 顶点$v_i$的**出度**为第$i$个单链表的节点数.
* 顶点$v_i$的**入度**为全部单链表中邻接点域值为$i-1$的节点的个数
* 找出度容易,找入度难

有向图邻接表的表示方法还可以用逆邻接表法,即单链表存储的是以对应节点为头的弧.
## 邻接矩阵和邻接表的关系
* 联系
	* 邻接表中每个链表对应邻接矩阵中的一行,链表中节点个数等于邻接矩阵一行中非零元素的个数.
* 区别
	* 对于任一确定的无向图,其邻接矩阵是唯一的(行列号和顶点编号一致), 但邻接表不唯一(链接次序与顶点编号无关).
	* 邻接矩阵空间复杂度为$\Omicron(n^2)$,邻接表空间复杂度为$\Omicron(n+e)$.
	* 用途:邻接矩阵多用于稠密图,邻接表多用于稀疏图.
## 十字链表
* 十字链表(Orthogonal List)是解决有向图的邻接表存储方式中求节点的度困难的问题,它是有向图的另一种链式存储结构,可以看成是有向图的邻接表和逆邻接表的结合.
* 有向图中每一条弧对应十字链表中一个**弧节点**,同时有向图中每一个顶点在十字链表中对应有一个**顶点节点**.
<img src="https://img-blog.csdnimg.cn/20201029222043364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =500x" alt="在这里插入图片描述" style="zoom:50%;" />
## 邻接多重表
* 邻接多重表解决无向图邻接表存储方式中,每条边要存储两次的问题.
<img src="https://img-blog.csdnimg.cn/20201030131227788.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =500x" alt="在这里插入图片描述" style="zoom:50%;" />
# 图的遍历
## 定义
* 从给定的连通图中某一顶点除法,沿着一些边访遍图中所有顶点,且每个顶点仅被访问一次,就叫做图的遍历,它是图的**基本操作**.
* 遍历的实质:寻找每个顶点的邻接点的过程.
## 遍历可能存在的问题及解决思路
* 图中可能存在回路,且图的任一顶点都可能与其他顶点相通,访问完某个顶点后可能沿着某些边又回到曾经访问过的顶点.
* 解决思路:设置辅助数组visited[*n*],用来标记被访问过的顶点
	* 初始状态,辅助数组visited中所有值为0.
	* 顶点*i*被访问,则将visited[*i*]设置为1,防止被多次访问.
## 深度优先搜索(Depth First Search, DFS)
遍历方法:
* 在访问图中某一起始点$v$后,由$v$出发,访问它的任一邻接点$w_1$;
* 再从$w_1$出发,访问与$w_1$邻接但还没被访问过的顶点$w_2$;
* 然后再从$w_2$出发,重复类似的访问过程,......,如此进行下去,直到所有邻接顶点都被访问过的顶点$u$为止.
* 接着,退回一步,退到前一次刚访问过的顶点,看其是否由未被访问过的邻接点:
	* 如果有,则访问此顶点,再由此顶点出发,进行与先前类似的访问过程.
	* 如果没有,则再回退一步进行搜索.重复上述过程,直到连通图中所有顶点都被访问过为止.
	<img src="https://img-blog.csdnimg.cn/20201031233559889.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMzMjY1Nzk2,size_16,color_FFFFFF,t_70#pic_center =500x" alt="在这里插入图片描述" style="zoom:50%;" />

* 连通图的深度优先遍历的思想类似于树中先序遍历.
* 算法时间复杂度
	* 用邻接矩阵表示的图,遍历图中每一顶点都要完整扫描每一顶点所在的行,时间复杂度为$O(n^2)$.
	* 用邻接表表示的图,虽然有$2e$个表节点,但依靠辅助数组,只需扫描$e$个数组即可完成遍历,再加上访问$n$个头节点的时间,时间复杂度为$O(n+e)$.
* 空间复杂度:借用堆栈,$O(n)$.