* 找出排序数组中离第k个数最近的m个数,最近的意思是相减绝对值最小
* ## 排序

| 排序算法 |      平均时间复杂度       |         最好情况          |         最坏情况          |       空间复杂度       | 排序方式  | 稳定性 |
| :------: | :-----------------------: | :-----------------------: | :-----------------------: | :--------------------: | :-------: | :----: |
| 冒泡排序 |    $ \textbf{O}(n^2) $    |     $ \textbf{O}(n) $     |    $ \textbf{O}(n^2) $    |   $ \textbf{O}(1) $    | In-place  |  稳定  |
| 选择排序 |    $ \textbf{O}(n^2) $    |    $ \textbf{O}(n^2) $    |    $ \textbf{O}(n^2) $    |   $ \textbf{O}(1) $    | In-place  | 不稳定 |
| 插入排序 |   $ \textbf{O}(n^{2}) $   |     $ \textbf{O}(n) $     |    $ \textbf{O}(n^2) $    |   $ \textbf{O}(1) $    | In-place  |  稳定  |
| 希尔排序 |  $ \textbf{O}(n\log n) $  |  $ \textbf{O}(n^{1.3}) $  |    $ \textbf{O}(n^2) $    |   $ \textbf{O}(1) $    | In-place  | 不稳定 |
| 归并排序 |  $ \textbf{O}(n\log n) $  |  $ \textbf{O}(n\log n) $  |  $ \textbf{O}(n\log n) $  |   $ \textbf{O}(n) $    | Out-place |  稳定  |
| 快速排序 |  $ \textbf{O}(n\log n) $  |  $ \textbf{O}(n\log n) $  |    $ \textbf{O}(n^2) $    | $ \textbf{O}(\log n) $ | In-place  | 不稳定 |
|  堆排序  |  $ \textbf{O}(n\log n) $  |  $ \textbf{O}(n\log n) $  |  $ \textbf{O}(n\log n) $  |   $ \textbf{O}(1) $    | In-place  | 不稳定 |
|          |                           |                           |                           |                        |           |        |
| 计数排序 |    $ \textbf{O}(n+k) $    |    $ \textbf{O}(n+k) $    |    $ \textbf{O}(n+k) $    |  $ \textbf{O}(n+k) $   | Out-place |  稳定  |
|  桶排序  |    $ \textbf{O}(n+k) $    |     $ \textbf{O}(n) $     |    $ \textbf{O}(n^2) $    |  $ \textbf{O}(n+k) $   | Out-place |  稳定  |
| 基数排序 | $ \textbf{O}(n\times k) $ | $ \textbf{O}(n\times k) $ | $ \textbf{O}(n\times k) $ |  $ \textbf{O}(n+k) $   | Out-place |  稳定  |

* 平均时间复杂度同样是$ \textbf{O}(n\log n) $,为什么快速排序要比堆排序性能好?
  * 堆排序访问数据的方式没有快速排序友好
    * **对于快速排序来说,数据是顺序访问的.而对于堆排序来说,数据是跳着访问的.**比如,堆排序中,最重要的一个操作就是数据的堆化.比如下面这个例子,对堆顶进行堆化,会依次访问数组下标是1,2,4,8的元素,而不像快速排序那样,局部顺序访问,所以,**这样对CPU缓存是不友好的**
  * 对于同样的数据,在排序过程中,堆排序算法的数据交换次数要多于快速排序
    * 对于基于比较的排序算法来说,整个排序过程是由两个基本操作组成的,比较和交换.**快速排序交换的次数不会比逆序的多.但是堆排序的第一步是建堆,建堆的过程会打乱数据原有的相对选择顺序,导致数据有序度降低.比如对于一组已经有序的数据来说,经过建堆之后,数据反而变得更无序了**


![在这里插入图片描述](https://img-blog.csdnimg.cn/4ea956ed6bbe434d826b2d5406b86909.png?x-oss-process=image/watermark,type_d3F5LXplbmhlaQ,shadow_50,text_Q1NETiBAY2FsbV9H,size_20,color_FFFFFF,t_70,g_se,x_16#pic_center)

### 1. 时间复杂度O(n^2)的排序

#### 1.1 冒泡

- ***思想***
  
  - 比较相邻的元素,把小的放前面去,大的就在后面,这样最终大的会在最后面
  
- 实现

  ```c++
  // 基本冒泡排序
  void bubbleSort(int array[], int length)
  {
  	for (int i = 0; i < length - 1; i++)
  	{
  		for (int j = 0; j < length - 1 - i; j++)
  		{
  			if (array[j] > array[j + 1])
  			{
  				swap(array[j], array[j + 1]);
  			}
  		}
  	}
  }
  
  ```

#### 1.2 选择

- ***思想***
  
  - **第一步:**首先在未排序序列中找到最小(大)元素,存放到排序序列的起始位置.
  - **第二步:**再从剩余未排序元素中继续寻找最小(大)元素,然后放到已排序序列的末尾.
  - **第三步:**重复第二步,直到所有元素均排序完毕.1.3 插入
  - **总结:**左边是拍好的,右边是没有拍好的,把第一个没有排序好地插入左边排好对应的位置中
  
- **实现**

  ```c++
  // 选择排序
  void selectionSort(int array[], int length)
  {
  	for (int i = 0; i < length - 1; i++) {
  		int minIdx = i;
  		for (int j = i + 1; j < length; j++) {
  			if (array[j] < array[minIdx]) {
  				minIdx = j;
  			}
  		}
  		if (minIdx != i) {
  			swap(array[i], array[minIdx]);
  		}
  	}
  }
  ```

### 2. 时间复杂度O(nlogn)的排序

#### 2.1 快速

- ***思想***
  
  - **第一步:**从数列中挑出一个元素,称为 "基准"(pivot);
  - **第二步:**重新排序数列,所有元素比基准值小的摆放在基准前面,所有元素比基准值大的摆在基准的后面(相同的数可以到任一边).在这个分区退出之后,该基准就处于数列的中间位置.这个称为分区(partition)操作;
  - **第三步:**递归地(recursive)把小于基准值元素的子数列和大于基准值元素的子数列排序.
  
- **实现**

  ```c++
  // 快速排序
  void QuickSort(int arr[], int start, int end)
  {
  	if (start >= end)
  		return;
  	int i = start;
  	int j = end;
  	int baseval = arr[start];	// 基准数
  	while (i < j) {
  		// 从右向左找比基准数小的数
  		while (i < j && arr[j] >= baseval) {
  			j--;
  		}
  		if (i < j) {
  			arr[i] = arr[j];
  			i++;
  		}
  		// 从左向右找比基准数大的数
  		while (i < j && arr[i] < baseval) {
  			i++;
  		}
  		if (i < j) {
  			arr[j] = arr[i];
  			j--;
  		}
  	}
  	arr[i] = baseval;	// 把基准数放到i的位置
  	// 递归
  	QuickSort(arr, start, i - 1);
  	QuickSort(arr, i + 1, end);
  }
  ```

  

#### 2.2 归并

- ***思想***
  
  - **第一步:**把长度为n的输入序列分成两个长度为n/2的子序列;
  - **第二步:**对这两个子序列分别采用归并排序;
  - **第三步:** 将两个排序好的子序列合并成一个最终的排序序列.
  
- **实现**

  ```c++
  // 归并排序
  void MergeSort(int arr[], int start, int end, int * temp) // start和end分别是左边界和右边界
  {
  	if (start >= end)
  		return;
  	int mid = (start + end) / 2;
  	MergeSort(arr, start, mid, temp);
  	MergeSort(arr, mid + 1, end, temp);
   
  	// 合并两个有序序列
  	int length = 0; // 表示辅助空间有多少个元素
  	int i_start = start;
  	int i_end = mid;
  	int j_start = mid + 1;
  	int j_end = end;
  	while (i_start <= i_end && j_start <= j_end) {
  		if (arr[i_start] < arr[j_start]) {
  			temp[length] = arr[i_start]; 
  			length++;
  			i_start++;
  		} else {
  			temp[length] = arr[j_start];
  			length++;
  			j_start++;
  		}
  	}
  	while (i_start <= i_end)  // 把剩下数的合并 {
  		temp[length] = arr[i_start];
  		i_start++;
  		length++;
  	}
  	while (j_start <= j_end) {
  		temp[length] = arr[j_start];
  		length++;
  		j_start++;
  	}
  	// 把辅助空间的数据放到原空间
  	for (int i = 0; i < length; i++) {
  		arr[start + i] = temp[i];
  	}
  }
  
  ```

### 3. 时间复杂度O(n)的排序

#### 3.1 计数/桶排序

- ***思想***
  
  - 设置一个定量的数组当作空桶;
  - 遍历输入数据,并且把数据一个一个放到对应的桶里去;
  - 对每个不是空的桶进行排序;
  - 从不是空的桶里把排好序的数据拼接起来.
  
- **实现**

  ```c++
  #include<stdio.h>
  int main() {
       int book[1001],i,j,t;
       //初始化桶数组
       for(i=0;i<=1000;i++) {
         book[i] = 0;
       }
       //输入一个数n,表示接下来有n个数
       scanf("%d",&n);
       for(i = 1;i<=n;i++) {
         //把每一个数读到变量中去
         scanf("%d",&t);
         //计数  
         book[t]++;
       }
       //从大到小输出
       for(i = 1000;i>=0;i--) {
         for(j=1;j<=book[i];j++) {
           printf("%d",i);
         }
       }
      getchar();getchar();
      //getchar()用来暂停程序,以便查看程序输出的内容
      //也可以用system("pause");来代替
      return 0;
  }
  ```
