 # LRU

* LRU是Least Recently Used的缩写,即最近最少使用,是一种常用的页面置换算法,**选择最近最久未使用的页面予以淘汰**.该算法赋予每个页面一个访问字段,用来记录一个页面自上次被访问以来所经历的时间 t,当须淘汰一个页面时,选择现有页面中其 t 值最大的,即最近最少使用的页面予以淘汰

  > 设计LRU缓存结构,该结构在构造时确定大小,假设大小为K,并有如下两个功能
  >
  > - set(key, value):将记录(key, value)插入该结构
  > - get(key):返回key对应的value值
  >
  > [要求]
  >
  > 1. set和get方法的时间复杂度为O(1)
  > 2. 某个key的set或get操作一旦发生,认为这个key的记录成了最常使用的.
  > 3. 当缓存的大小超过K时,移除最不经常使用的记录,即set或get最久远的.
  >
  > 若opt=1,接下来两个整数x, y,表示set(x, y)
  > 若opt=2,接下来一个整数x,表示get(x),若x未出现过或已被移除,则返回-1
  > 对于每个操作2,输出一个答案

* 代码实现,使用哈希表和链表实现,哈希表记录某个key值对应的链表节点,以常数时间复杂度操作对应链表节点.每次set或get,将对应节点移动到链表头,表示最近使用,每次淘汰将链表尾节点淘汰,表示最久未使用

  ```c++
  class LRUCache {
  public:
      LRUCache(int capacity): cap(capacity) {
  
      }
      
      int get(int key) {
          if(map.count(key)) {
              mv2front(key);
              return map[key]->second;
          }
          return -1;
      }
      
      void put(int key, int value) {
          if(map.count(key)) {
              map[key]->second = value;
              mv2front(key);
          }
          else{
              cache.push_front({key, value});
              map.insert({key, cache.begin()});
              if(map.size() > cap) {
                  map.erase(cache.back().first);
                  cache.pop_back();
              }
              
          }
      }
      
  private:
      void mv2front(int key) {
          cache.splice(cache.begin(), cache, map[key]);
      }
      
  private:
      int cap;
      unordered_map<int, list<pair<int, int> >::iterator> map;
      list<pair<int, int> > cache;
  };
  ```

# LFU

* LFU(least frequently used (LFU) page-replacement algorithm),即最不经常使用页置换算法,要求在页置换时置换引用计数最小的页,因为经常使用的页应该有一个较大的引用次数

  > 一个缓存结构需要实现如下功能.
  >
  > - set(key, value):将记录(key, value)插入该结构
  > - get(key):返回key对应的value值
  >
  > 但是缓存结构中最多放K条记录,如果新的第K+1条记录要加入,就需要根据策略删掉一条记录,然后才能把新记录加入.这个策略为:在缓存结构的K条记录中,哪一个key从进入缓存结构的时刻开始,被调用set或者get的次数最少,就删掉这个key的记录;
  >
  > 如果调用次数最少的key有多个,上次调用发生最早的key被删除
  >
  > 这就是LFU缓存替换算法.实现这个结构,K作为参数给出
  >
  > [要求]
  >
  > set和get方法的时间复杂度为O(1)
  >
  > 若opt=1,接下来两个整数x, y,表示set(x, y)
  > 若opt=2,接下来一个整数x,表示get(x),若x未出现过或已被移除,则返回-1

* 代码实现,使用两个哈希表实现.将每个访问频率的记录存储到一个链表中,每个节点记录为(key,val),一个哈希表建立访问频率到对应链表首节点的映射,另一个哈希表建立记录链表中记录的key值到该节点的映射,实现常数时间复杂度访问.每次get或set,将该记录访问频率加1,并添加到对应访问频率链表头.每次淘汰最小访问频率对应链表的尾节点

  ```c++
  
  class LFUImpl {
  public:
      vector<int> LFU(vector<vector<int> >& operators, int k) {
          vector<int>　ret;
          for(auto &op: operators) {
              if(op[0] == 1){
                  set(op[1], op[2], k);
              }
              else {
                  ret.push_back(get(op[1]));
              }
          }
          return ret;
      }
      
      int get(int key ){
          if(key2nodeit.count(key)) {
              update(key);
              return key2nodeit[key]->val;
          }
          return -1;
      }
      
      void set(int key, int val, int k) {
          if(key2nodeit.count(key)>0){
              update(key);
              key2nodeit[key]->val = val;
          }
          else {
              if(int(key2nodeit.size()) == k){
                  remove();
              }
              minFreq = 1;
              freq2node[minFreq].push_front({key, val, minFreq});
              key2nodeit[key] = freq2node[minFreq].begin();
          }
      }
  private:
      void update(int key) {
          int val = key2nodeit[key]->val;
          int freq = key2nodeit[key]->freq;
          freq2node[freq].erase(key2nodeit[key]);
          if(freq2node[freq].empty()) {
              freq2node.erase(freq);
              if(minFreq == freq) {
                  minFreq++;
              }
          }
          freq2node[freq + 1].push_front(node(key, val, freq + 1));
          key2nodeit[key] = freq2node[freq + 1].begin();
      }
   
      void remove() {
          int key = freq2node[minFreq].back().key;
          key2nodeit.erase(key);
          freq2node[minFreq].pop_back();
          if(freq2node[minFreq].empty()) {
              freq2node.erase(minFreq);
          }
      }
  private:
      struct node{
          int key, val, freq;
          node(int k, int v, int f) : key(k), val(v), freq(f){}
      };
      unordered_map<int, list<node>::iterator > key2nodeit;	// 每个键对应一个list迭代器,用于快速查询
      unordered_map<int, list<node> > freq2node; // 每个频率对应一个list
      int minFreq = 1;  // 记录最小频率
  };
  ```
