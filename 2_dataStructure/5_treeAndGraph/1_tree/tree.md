---
title: "B树、B+树是什么"
layout: post
author: "Marco"
header-style: text
hidden: true
tags:
  - 数据库
---

## B树、B+树是什么

> B树和B+树是数据库中重要的数据结构，能够实现快速的索引。了解这两种数据结构有助于读者更好明白数据库是如何工作的。
>
> 估计阅读耗时：

[toc]

#### (14) 介绍一下b树和它的应用场景有哪些
B树也叫做B-树,或者平衡多路树,它是每个节点最多有m个子树的**平衡树**.一个m阶的B树具有如下几个特征:
1. 根结点至少有两个子女.
2. 每个中间节点都包含至多m个子树 , 每个节点包含的元素个数是其子树个数-1(其中 m/2 <= k <= m)
3. 所有的叶子结点都位于同一层.
4. 每个节点中的元素从小到大排列,节点当中k-1个元素正好是k个子树包含的元素的值域分划.

b树主要应用于文件系统中,在数据库中(mongoDB)也有应用,与B+树相比好处应该是有时不需要访问到叶节点就可以获取数据.

查询时间复杂度是logN
#### (15) 介绍一下b+树和它的应用场景有哪些
B+树是一种特殊的B树,它把数据都存储在叶子节点,并且叶节点间有指针连接.内部只存关键字(其中叶子节点的最小值作为索引)和孩子指针,简化了内部节点.

应用场景主要是数据库的索引

查询时间复杂度也是logN
https://zhuanlan.zhihu.com/p/110202102

https://blog.csdn.net/hguisu/article/details/7786014

### 1. 基本概念

- ***B树、B+树应用场景是什么？***

  ​	B树、B+树是数据库的常用数据，其优点在于在大型数据库能够快速找到文件。

- ***为什么要用B树、B+树？或者说快速找到文件是如何体现的？***

  ​	以[1,2,3,4,5,6,7,8,9]为例，如果我们想查找5的数据，逐个遍历访问的话就是$O(N)$的时间级别复杂度，如果是二分查找的话就是$O(logN)$的时间复杂度。在$N$很大的时候，$logN<<N$，而B树、B+树就是针对二分查找特殊设计的树。

- ***B树、B+树针对二分查找的特殊设计之处在哪？***

  ​	简单来说，这个特殊之处最核心的本质就在于都是平衡树。

  ![image-20220219094545632](https://s2.loli.net/2022/02/19/taVzf2PvGQlX41q.png)

  > [图片来源](https://zhuanlan.zhihu.com/p/27700617)

  ​	不难从左图得出，左图是非常便于二分查找的，小于当前节点的就是左节点，大于当前节点的就是右节点，等于当前节点的就是父节点。根据这种数据的存储方式，就可以实现二分查找了。

  ​	但是从右图我们也不难得出树的插入顺序有可能将树的节点变为线性结构。其本质差异就在于左边的树较右边的树更平衡。所以如果要针对数据库选择数据结构，其实就是选择一种平衡树结构，怎么让其实现自平衡。

  ​	而我们常说的红黑树、AVL树实质上都是平衡二叉树；B树和B+树则是平衡多叉树。两者的区别就在于查找路径数量。二叉树的查找路径是只有左节点和右节点的，而多叉树的查找路径可以是若干个。

  ​	如果数据更多的时候，多叉树的搜索效率又优于二叉树，这是因为IO读写速度慢的影响。详情看[这里](https://www.bilibili.com/video/BV1et4y117wc?from=search&seid=14260168568370412631&spm_id_from=333.337.0.0)

- ***B树和B+树是如何实现自平衡的？两个的区别是什么***

  ​	自平衡的实现在于插入，每当插入不满足平衡条件时，树就会变形。插入、删除时的具体变形可以看[这里](https://segmentfault.com/a/1190000020416577)，[两个的区别在于这里](cs.usfca.edu/~galles/visualization)。

### 2. 平衡多叉树

#### 2.1 B树

- ***[代码实现](https://github.com/Kronuz/cpp-btree)***

#### 2.2 B+树

* ***[代码实现](https://github.com/zcbenz/BPlusTree)***


#### 堆的维护时间复杂度 
#### 求数组中最小的10个数,用堆写
#### (11) Top K问题(可以采取的方法有哪些,各自优点?)(重点)
*Top K 问题的常见形式:*
>给定10000个整数,找第K大(第K小)的数<br>
 给定10000个整数,找出最大(最小)的前K个数<br>
给定100000个单词,求前K词频的单词<br>

*解决Top K问题若干种方法*
* 使用最大最小堆.求最大的数用最小堆,求最小的数用最大堆.
* Quick Select算法.使用类似快排的思路,根据pivot划分数组.
* 使用排序方法,排序后再寻找top K元素.
* 使用选择排序的思想,对前K个元素部分排序.
* 将1000.....个数分成m组,每组寻找top K个数,得到m×K个数,在这m×k个数里面找top K个数.

1. 使用最大最小堆的思路 (以top K 最大元素为例)<br>
按顺序扫描这10000个数,先取出K个元素构建一个大小为K的最小堆.每扫描到一个元素,如果这个元素大于堆顶的元素(这个堆最小的一个数),就放入堆中,并删除堆顶的元素,同时整理堆.如果这个元素小于堆顶的元素,就直接pass.最后堆中剩下的元素就是最大的前Top K个元素,最右的叶节点就是Top 第K大的元素.

>note:最小堆的插入时间复杂度为log(n),n为堆中元素个数,在这里是K.最小堆的初始化时间复杂度是nlog(n)

C++中的最大最小堆要用标准库的priority_queue来实现.
```C++
struct Node {
    int value;
    int idx;
    Node (int v, int i): value(v), idx(i) {}
    friend bool operator < (const struct Node &n1, const struct Node &n2) ; 
};

inline bool operator < (const struct Node &n1, const struct Node &n2) {
    return n1.value < n2.value;
}

priority_queue<Node> pq; // 此时pq为最大堆
```

2. 使用Quick Select的思路(以寻找第K大的元素为例)<br>
Quick Select脱胎于快速排序,提出这两个算法的都是同一个人.算法的过程是这样的:
首先选取一个枢轴,然后将数组中小于该枢轴的数放到左边,大于该枢轴的数放到右边.
此时,如果左边的数组中的元素个数大于等于K,则第K大的数肯定在左边数组中,继续对左边数组执行相同操作;
如果左边的数组元素个数等于K-1,则第K大的数就是pivot;
如果左边的数组元素个数小于K,则第K大的数肯定在右边数组中,对右边数组执行相同操作.

这个算法与快排最大的区别是,每次划分后只处理左半边或者右半边,而快排在划分后对左右半边都继续排序.
```
//此为Java实现
public int findKthLargest(int[] nums, int k) {
  return quickSelect(nums, k, 0, nums.length - 1);
}

// quick select to find the kth-largest element
public int quickSelect(int[] arr, int k, int left, int right) {
  if (left == right) return arr[right];
  int index = partition(arr, left, right);
  if (index - left + 1 > k)
    return quickSelect(arr, k, left, index - 1);
  else if (index - left + 1 == k)
    return arr[index];
  else
    return quickSelect(arr, k - (index - left + 1), index + 1, right);

}
```

3. 使用选择排序的思想对前K个元素排序 ( 以寻找前K大个元素为例)<br>
扫描一遍数组,选出最大的一个元素,然后再扫描一遍数组,找出第二大的元素,再扫描一遍数组,找出第三大的元素.....以此类推,找K个元素,时间复杂度为O(N*K)