## 智能指针

### 1. concepts

1. 为什么要有智能指针？智能指针（Smart Pointer）是 C++中的一种对象，它像原始指针一样可以指向堆上的对象，但是它们有一个额外的特性：当没有任何指针指向一个对象时，该对象会被自动删除。这样可以避免内存泄漏，使得内存管理更加方便，因此被称为“智能”指针。简单来说，智能指针就是通过 RAII 保证作用域外自动被析构的。

2. 有什么智能指针，分别怎么用？在 C++中，有三种主要的智能指针：`std::unique_ptr`，`std::shared_ptr`和`std::weak_ptr`。

   1. `std::unique_ptr`：这是一种独占所有权的智能指针。
   2. `std::shared_ptr`：这是一种共享所有权的智能指针。
   3. `std::weak_ptr`：这是一种不拥有所有权的智能指针，它指向一个由`shared_ptr`管理的对象。
   4. 为什么要有 weak_ptr？ `weak_ptr`是一种特殊的智能指针，它可以指向`shared_ptr`所管理的对象，但是它不会增加该对象的引用计数。这意味着`weak_ptr`不会阻止对象被删除。`weak_ptr`要理解为不拥有所有权，不增加引用计数的一种指针。主要用于解决 std::shared_ptr 可能导致的循环引用问题，以及在不影响对象生命周期的情况下观察对象。

   - 解决`shared_ptr`的循环引用问题。例如，如果两个`shared_ptr`对象互相引用，那么它们都不会被删除，这会导致内存泄漏。通过使用`weak_ptr`，我们可以打破这种循环引用，避免内存泄漏；
   - 我某一个业务缓存了一份数据，业务这边不负责这个数据的生命周期，有就用，没有就重新创建，这个时候用`weak_ptr`也是很合适的。（怎么理解这个所有权？这个所有权的意思是谁拥有的意思，拥有这个对象的指针需要负责去析构，谁拥有谁析构，反过来其实也是谁析构谁拥有。）

### 2. unique_ptr

#### 2.1 unique_ptr 是怎么保证所有权唯一的？

`std::unique_ptr`通过以下几种方式保证所有权的唯一性：

1. **禁止复制**：`std::unique_ptr`禁止复制构造和复制赋值，这意味着你不能将一个`unique_ptr`直接赋值给另一个`unique_ptr`。这样可以防止有两个`unique_ptr`同时拥有同一个对象的所有权。
2. **允许移动**：虽然`unique_ptr`禁止复制，但是它允许移动。这意味着你可以将一个`unique_ptr`的所有权转移给另一个`unique_ptr`。在所有权转移之后，原始的`unique_ptr`不再拥有任何对象，这样可以确保任何时候都只有一个`unique_ptr`拥有对象的所有权。

通过这两种方式，`std::unique_ptr`可以保证所有权的唯一性，然后通过 RAII 避免内存泄漏和悬挂指针等问题。

### 3. shared_ptr

#### 3.1 shared_ptr 是怎么控制所有权的？

`shared_ptr`是 C++中的一种智能指针，它通过引用计数来控制所有权。每当一个新的`shared_ptr`指向某个对象，这个对象的引用计数就会增加。当`shared_ptr`被销毁或者重新指向其他对象时，原对象的引用计数就会减少。当引用计数变为 0 时，`shared_ptr`就会自动删除所指向的对象。

#### 3.2 多线程下的 shared_ptr 需要注意什么？

需要在复制构造函数和赋值运算符中正确地增加引用计数。需要在析构函数中正确地减少引用计数，并在引用计数为 0 时删除原始指针。需要处理自我赋值的情况。如果一个对象赋值给它自己，那么在减少引用计数并删除原始指针之前，必须先增加引用计数。

#### 3.3 如何实现一个引用计数指针,以及其中要注意的点?

1. 需要在复制构造函数和赋值运算符中正确地增加引用计数。
2. 需要在析构函数中正确地减少引用计数，并在引用计数为 0 时删除原始指针。
3. 需要处理自我赋值的情况。如果一个对象赋值给它自己，那么在减少引用计数并删除原始指针之前，必须先增加引用计数。
4. 需要使用互斥锁或原子操作来保证线程安全，因为有可能在多线程环境使用。

#### 3.4 `std::enable_share_from_this`

- 为什么要用 `enable_shared_from_this`？

  - **需要在类对象的内部中获得一个指向当前对象的 shared_ptr 对象**
  - 如果在一个程序中，对象内存的生命周期全部由智能指针来管理。在这种情况下，**要在一个类的成员函数中，对外部返回 this 指针就成了一个很棘手的问题**

- 什么时候用？

  - 当一个类被 `share_ptr` 管理，且在**类的成员函数里需要把当前类对象作为参数传给其他函数时**，这时就需要传递一个指向自身的 `share_ptr`

- 如何安全地将 this 指针返回给调用者?
  - 一般来说，我们不能直接将 this 指针返回。如果函数将 this 指针返回到外部某个变量保存，然后这个对象自身已经析构了，但外部变量并不知道，此时如果外部变量再使用这个指针，就会使得程序崩溃>

### 4. weak_ptr

#### 4.1 weak_ptr 是怎么解决循环引用计数问题的？

循环引用指两个或更多个 `std::shared_ptr` 实例相互引用形成闭环。在此情况下，即便没有外部 `std::shared_ptr` 指向这些对象，它们的引用计数也不会降为 0，导致对象永远不会被删除，进而引发内存泄漏。以下通过一个示例说明：

```c++
#include <iostream>
#include <memory>

class B;

class A {
public:
    std::shared_ptr<B> b_ptr;
    ~A() { std::cout << "A destroyed\n"; }
};

class B {
public:
    std::shared_ptr<A> a_ptr;
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {

    auto a = std::make_shared<A>();  // a.use_count() == 1
    auto b = std::make_shared<B>();  // b.use_count() == 1
    a->b_ptr = b;  // b.use_count() == 2
    b->a_ptr = a;  // a.use_count() == 2
    return 0;
}
```

在上述代码中，栈上有两个 `std::shared_ptr` 类型的变量 `a` 和 `b`，它们指向堆上的对象。。要理解`std::share_ptr`内部是存放着一个堆上指针，和与这个堆上指针关联的引用计数。当和这个堆上指针关联的引用技术为 0 时，才会把这个堆上指针给释放。这里栈上的两个`std::shared_ptr`被干掉后，引用技术都还是 1，因此就出现了循环引用的问题。

`std::weak_ptr` 能打破这种循环。它属于弱引用，不会增加对象引用计数，可视为安全的观察者，能观察由 `std::shared_ptr` 管理的对象，但不妨碍对象被删除。`std::weak_ptr` 是一种不控制资源生命周期的智能指针，它对对象进行弱引用，仅提供对其管理资源的访问手段，引入它旨在协助 `std::shared_ptr` 工作。当需要使用 `std::weak_ptr` 所观察的对象时，可调用 `std::weak_ptr::lock` 方法。若对象存在，此方法返回一个新的 `std::shared_ptr` 实例；若对象已删除，`lock` 方法返回一个空的 `std::shared_ptr`。以下是使用 `std::weak_ptr` 解决循环引用的示例：

```c++
#include <iostream>
#include <memory>

class B;

class A {
  public:
    std::weak_ptr<B> b_ptr;
    ~A() { std::cout << "A destroyed\n"; }
};

class B {
  public:
    std::weak_ptr<A> a_ptr;
    ~B() { std::cout << "B destroyed\n"; }
};

int main() {
    auto a = std::make_shared<A>();  // a.use_count() equals = 1
    auto b = std::make_shared<B>();  // b.use_count() equals = 1
    a->b_ptr = b;                    // b.use_count() equals = 2
    b->a_ptr = a;                    // a.use_count() equals = 1

    if (auto shared_a = b->a_ptr.lock()) {
        std::cout << "A对象仍然存在\n";
    }
    return 0;
}
```

在这个示例中，当 `a` 离开作用域时，其引用计数减为 0，从而析构 `A` 对象。`A` 对象析构时，其成员 `b_ptr` 析构不会影响 `b` 的引用计数。随后 `b` 离开作用域，其引用计数减为 0，从而析构 `B` 对象，避免了内存泄漏。`std::weak_ptr` 可以从一个 `std::shared_ptr` 或另一个 `std::weak_ptr` 对象构造。`std::shared_ptr` 可直接赋值给 `std::weak_ptr`，也可通过 `std::weak_ptr` 的 `lock()` 函数来获得 `std::shared_ptr`。其构造和析构不会引起引用计数的增加或减少。通过这些特性，`std::weak_ptr` 可有效解决 `std::shared_ptr` 相互引用时的死锁问题（即两个 `std::shared_ptr` 相互引用，导致引用计数永远不可能下降为 0，资源无法释放的情况）。

虽然 `std::weak_ptr` 常用于解决 `std::shared_ptr` 的循环引用问题。但我觉得这种例子非常不好，正常人谁会这样子写代码，这个代码表明的意图是非常模糊的

在实际应用中，`std::weak_ptr` 更适用于表明一种不占有所有权、观察对象生命周期且不遵循 RAII（Resource Acquisition Is Initialization）原则的指针场景。例如，某业务缓存了一份数据，业务本身不负责该数据的生命周期，数据存在时直接使用，不存在时重新获取，这种情况下使用 `weak_ptr` 存储该数据就较为合适。

### 5. 释放器

1. **背景介绍**：默认情况下，智能指针对象在析构时仅释放其持有的堆内存（调用 `delete` 或 `delete[]`）。然而，当堆内存代表的对象还关联其他需回收的非内存资源（如操作系统的套接字句柄、文件句柄等）时，就需要自定义智能指针的资源释放函数。

2. **`std::unique_ptr` 自定义释放器**：智能指针的释放器（Deleter）可以是一个函数或函数对象，当智能指针不再需要其所管理的资源时，会调用释放器来释放资源。在 `std::unique_ptr` 中，可自定义释放器用于管理非内存资源。例如，为 `std::unique_ptr` 定义一个用于关闭文件的释放器：

```cpp
struct FileDeleter
{
    void operator()(FILE* ptr) const
    {
        if (ptr)
        {
            fclose(ptr);
        }
    }
};

std::unique_ptr<FILE, FileDeleter> smartFile(fopen("file.txt", "r"));
```

在此例中，当 `smartFile` 离开其作用域时，会自动调用 `FileDeleter` 来关闭文件。

3. **`std::shared_ptr` 自定义释放器**：同样，在 `std::shared_ptr` 中也能自定义释放器。例如：

```cpp
std::shared_ptr<FILE> smartFile(fopen("file.txt", "r"), [](FILE* ptr){
    if (ptr)
    {
        fclose(ptr);
    }
});
```

在这个例子里，当 `std::shared_ptr` 管理的文件资源不再被引用时，会调用自定义的 lambda 表达式释放器来关闭文件。
