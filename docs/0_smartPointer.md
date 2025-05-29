## 智能指针

### 1. concepts

1. 为什么要有智能指针？
   智能指针是 C++ 中用于管理动态内存的一种强大工具。它本质上是一个对象，具备与原始指针相似的功能，能够指向堆上分配的对象。然而，智能指针之所以被称为“智能”，是因为它拥有一个关键特性：当不再有任何指针指向其所管理的对象时，该对象会被自动释放。这一特性极大地简化了内存管理流程，有效避免了因手动管理内存不当而导致的内存泄漏问题。

    智能指针实现自动内存管理的原理基于资源获取即初始化（RAII，Resource Acquisition Is Initialization）机制。当一个智能指针对象被创建时，它会获取对某个资源（如堆上的对象）的控制权；当智能指针对象的生命周期结束（例如超出作用域）时，其析构函数会自动被调用，在析构函数中会释放其所管理的资源。通过这种方式，智能指针确保了资源在不再需要时能够被及时、正确地释放，使内存管理变得更加便捷和可靠。


2. 为什么 auto_ptr 被淘汰了？

    `auto_ptr` 是 C++98 标准引入的一种智能指针类型，但在实践中暴露出一些严重的问题，导致在 C++11 标准中被弃用，并被 `unique_ptr` 所取代。

    `auto_ptr` 最大的问题在于其所有权管理机制。当 `auto_ptr` 进行复制或赋值操作时，会发生所有权的转移，即原始的 `auto_ptr` 会失去对对象的所有权，转而由新的 `auto_ptr` 接管。这种行为往往不符合程序员的预期，因为在许多情况下，我们期望复制或赋值操作后，两个指针都能访问对象。例如，在将 `auto_ptr` 作为函数参数传递或者在容器中存储 `auto_ptr` 时，这种所有权的意外转移会导致难以调试的错误。

    具体来说，指针的所有权意味着拥有该指针的实体负责在适当的时候释放（`delete`）指针所指向的内存。在 `auto_ptr` 的设计中，虽然也遵循了谁拥有谁释放的原则，但由于其复制和赋值操作中的所有权转移特性，使得所有权的管理变得混乱。尤其在 C++98 标准中尚未引入正式的移动语义，`auto_ptr` 的这种所有权转移行为缺乏清晰、高效的实现方式，进一步加剧了其使用的复杂性和易错性。因此，为了提供更清晰、安全的所有权语义，`auto_ptr` 被逐渐淘汰，`unique_ptr` 应运而生。

3. 有什么智能指针，分别怎么用？
   在 C++中，有三种主要的智能指针：`std::unique_ptr`，`std::shared_ptr`和`std::weak_ptr`。

   1. `std::unique_ptr`：这是一种独占所有权的智能指针。
   2. `std::shared_ptr`：这是一种共享所有权的智能指针。
   3. `std::weak_ptr`：这是一种不拥有所有权的智能指针，它指向一个由`shared_ptr`管理的对象。
   4. 为什么要有 weak_ptr？
   `weak_ptr`是一种特殊的智能指针，它可以指向`shared_ptr`所管理的对象，但是它不会增加该对象的引用计数。这意味着`weak_ptr`不会阻止对象被删除。`weak_ptr`要理解为不拥有所有权，不增加引用计数的一种指针。主要用于解决 std::shared_ptr 可能导致的循环引用问题，以及在不影响对象生命周期的情况下观察对象。
   - 解决`shared_ptr`的循环引用问题。例如，如果两个`shared_ptr`对象互相引用，那么它们都不会被删除，这会导致内存泄漏。通过使用`weak_ptr`，我们可以打破这种循环引用，避免内存泄漏；
   - 我某一个业务缓存了一份数据，业务这边不负责这个数据的生命周期，有就用，没有就重新创建，这个时候用`weak_ptr`就是很合适的。

### 2. unique_ptr

#### 2.1 unique_ptr 是怎么保证所有权唯一的？

`std::unique_ptr`通过以下几种方式保证所有权的唯一性：

1. **禁止复制**：`std::unique_ptr`禁止复制构造和复制赋值，这意味着你不能将一个`unique_ptr`直接赋值给另一个`unique_ptr`。这样可以防止有两个`unique_ptr`同时拥有同一个对象的所有权。
2. **允许移动**：虽然`unique_ptr`禁止复制，但是它允许移动。这意味着你可以将一个`unique_ptr`的所有权转移给另一个`unique_ptr`。在所有权转移之后，原始的`unique_ptr`不再拥有任何对象，这样可以确保任何时候都只有一个`unique_ptr`拥有对象的所有权。
3. **自动删除**：当`unique_ptr`被销毁（例如离开其作用域）时，它会自动删除其所拥有的对象。这意味着你不需要手动删除对象，可以防止因忘记删除对象而导致的内存泄漏。

通过这三种方式，`std::unique_ptr`可以保证所有权的唯一性，从而避免内存泄漏和悬挂指针等问题。

### 3. shared_ptr

#### 3.1 `shared_ptr` 控制所有权的方式

`std::shared_ptr` 作为 C++ 中用于实现共享所有权的智能指针，主要通过引用计数机制来精细地控制对象的所有权。具体而言，每当有一个新的 `std::shared_ptr` 指向同一个对象时，该对象的引用计数便会递增。这意味着多个 `std::shared_ptr` 实例可以同时拥有对同一个对象的“部分所有权”，因为它们共同维护着对象的生命周期。

当某个 `std::shared_ptr` 实例被销毁（比如超出其作用域）或者重新指向其他对象时，其所指向对象的引用计数就会相应地递减。这一机制确保了对象在仍有 `std::shared_ptr` 实例引用它时不会被意外释放。而当引用计数减少到 0 时，也就表明不再有任何 `std::shared_ptr` 指向该对象，此时 `std::shared_ptr` 会自动执行内存清理操作，删除其所指向的对象，从而有效地避免了内存泄漏。

#### 3.2 多线程下 `shared_ptr` 的注意事项

在多线程环境中使用 `std::shared_ptr` 时，需要特别留意以下几个关键方面：

1. **引用计数操作的原子性**：由于多个线程可能同时对 `std::shared_ptr` 进行操作，如复制构造、赋值等操作都会涉及引用计数的增减，所以必须保证这些操作的原子性。否则，可能会出现竞态条件，导致引用计数的不准确，进而引发内存管理错误，比如对象被提前释放或者未能及时释放。在 C++ 标准库的实现中，`std::shared_ptr` 的引用计数操作通常是原子的，但在自定义实现引用计数指针时，需要特别注意确保原子性。

2. **析构函数中的操作**：在 `std::shared_ptr` 的析构函数中，不仅要正确地减少引用计数，还需要在引用计数降为 0 时，安全地删除原始指针。在多线程环境下，这一过程同样需要考虑线程安全性，以避免多个线程同时尝试删除对象或在对象已被删除后仍尝试访问的情况。

3. **自我赋值处理**：当出现自我赋值的情况，即在一个 `std::shared_ptr` 对象将自身赋值给自己时，需要格外小心。正确的做法是在减少原对象引用计数并删除原始指针之前，先增加引用计数，以防止对象在赋值过程中被意外删除。例如：
```cpp
std::shared_ptr<int> ptr = std::make_shared<int>(42);
ptr = ptr; // 自我赋值情况
```
在这种情况下，`std::shared_ptr` 的实现必须确保在处理过程中对象的引用计数和生命周期得到正确管理。

#### 3.3 实现引用计数指针及其注意要点

实现一个引用计数指针时，除了上述多线程环境下的注意事项外，还需重点关注以下方面：

1. **复制构造函数和赋值运算符**：在复制构造函数和赋值运算符中，必须正确地增加引用计数。当通过复制构造函数创建一个新的引用计数指针实例，或者通过赋值运算符将一个引用计数指针赋值给另一个时，新的实例应该与原实例共同指向同一个对象，并增加对象的引用计数。例如：

2. **析构函数**：析构函数负责在引用计数为 0 时，正确地删除原始指针。当引用计数指针实例被销毁时，析构函数应减少引用计数，并在引用计数降为 0 时，释放所指向的对象以及引用计数本身所占用的内存。

3. **自我赋值处理**：如同在多线程环境下一样，在实现引用计数指针时，必须妥善处理自我赋值的情况。确保在赋值过程中，对象的引用计数和生命周期得到正确管理，避免出现对象被提前释放或内存泄漏等问题。

4. **线程安全**：在多线程环境中，为了保证引用计数操作的线程安全性，可能需要使用互斥锁（`std::mutex`）或原子操作（`std::atomic`）。例如，可以使用互斥锁来保护引用计数的增减操作，确保在同一时间只有一个线程能够修改引用计数。如下是一个简单示例：

#### 3.4 `std::enable_shared_from_this`

1. **为什么要用 `std::enable_shared_from_this`**：
在 C++ 编程中，当对象的内存生命周期完全由智能指针管理时，在类对象的内部获取一个指向当前对象的 `std::shared_ptr` 对象就成为一个重要需求。特别是在类的成员函数中，若要将当前对象作为参数传递给其他函数，直接返回 `this` 指针会带来问题。因为直接返回 `this` 指针创建的普通指针不受智能指针的引用计数管理，可能导致对象在外部指针仍在使用时被提前析构，从而引发程序崩溃。而 `std::enable_shared_from_this` 提供了一种安全的方式，使得在类的成员函数中能够获取一个指向自身的 `std::shared_ptr`，该 `std::shared_ptr` 与管理该对象的其他 `std::shared_ptr` 共享相同的引用计数，从而保证对象生命周期的一致性。

2. **什么时候用**：
当一个类被 `std::shared_ptr` 管理，并且在类的成员函数里需要把当前类对象作为参数传给其他函数时，就需要使用 `std::enable_shared_from_this`。例如，在一个需要将自身对象传递给其他模块进行处理的类中，或者在类的成员函数需要注册自身到某个全局管理机制中时，都可以使用 `std::enable_shared_from_this` 来获取一个指向自身的 `std::shared_ptr`，以确保对象的生命周期得到正确管理。

3. **如何安全地将 `this` 指针返回给调用者**：
一般情况下，直接返回 `this` 指针是不安全的，因为如前所述，这可能导致对象生命周期管理的不一致。通过继承自 `std::enable_shared_from_this` 类，类的成员函数可以调用 `shared_from_this()` 方法来获取一个指向当前对象的 `std::shared_ptr`。这个 `std::shared_ptr` 与管理该对象的其他 `std::shared_ptr` 共享引用计数，从而安全地将当前对象以智能指针的形式返回给调用者。例如：
```cpp
#include <iostream>
#include <memory>

class MyClass : public std::enable_shared_from_this<MyClass> {
public:
    std::shared_ptr<MyClass> getSharedPtr() {
        return shared_from_this();
    }
};

int main() {
    std::shared_ptr<MyClass> obj = std::make_shared<MyClass>();
    std::shared_ptr<MyClass> sharedObj = obj->getSharedPtr();
    return 0;
}
```
在上述代码中，`MyClass` 继承自 `std::enable_shared_from_this<MyClass>`，在 `getSharedPtr` 成员函数中通过 `shared_from_this()` 获取指向自身的 `std::shared_ptr`，确保了对象生命周期的安全管理。 

### 4. weak_ptr

#### 4.1 weak_ptr 解决循环引用计数问题的原理

`std::weak_ptr`作为一种特殊的智能指针，在解决`std::shared_ptr`循环引用问题方面发挥着关键作用。

循环引用是指两个或多个`std::shared_ptr`实例之间相互引用，形成一个封闭的环。例如，假设有类`A`和类`B`，`A`中有一个`std::shared_ptr<B>`成员变量，`B`中有一个`std::shared_ptr<A>`成员变量。当创建`A`和`B`的实例，并让它们相互引用时，就会出现循环引用：
```cpp
class B;

class A {
public:
    std::shared_ptr<B> b;
};

class B {
public:
    std::shared_ptr<A> a;
};

int main() {
    std::shared_ptr<A> ptrA = std::make_shared<A>();
    std::shared_ptr<B> ptrB = std::make_shared<B>();

    ptrA->b = ptrB;
    ptrB->a = ptrA;
}
```
在上述代码中，`ptrA`和`ptrB`的引用计数由于相互引用而永远不会降为0，即使`main`函数结束，`ptrA`和`ptrB`所指向的对象也不会被释放，从而导致内存泄漏。

`std::weak_ptr`的设计目的就是打破这种循环。它本质上是一种弱引用，不会增加所指向对象的引用计数。可以将`std::weak_ptr`理解为一个安全的观察者，它能够观察由`std::shared_ptr`管理的对象，但不会对对象的生命周期产生影响，即不会阻止对象被删除。

为了解决上述循环引用问题，可以将其中一个类的成员变量类型改为`std::weak_ptr`，例如：
```cpp
class B;

class A {
public:
    std::shared_ptr<B> b;
};

class B {
public:
    std::weak_ptr<A> a;
};

int main() {
    std::shared_ptr<A> ptrA = std::make_shared<A>();
    std::shared_ptr<B> ptrB = std::make_shared<B>();

    ptrA->b = ptrB;
    ptrB->a = ptrA;
}
```
在这个修改后的代码中，`B`类中的`a`是`std::weak_ptr<A>`类型，不会增加`ptrA`的引用计数。当`main`函数结束时，`ptrA`和`ptrB`的引用计数会正常降为0，它们所指向的对象也会被正确释放，从而避免了内存泄漏。

当需要使用`std::weak_ptr`所观察的对象时，可以调用`std::weak_ptr::lock`方法。该方法会尝试获取一个指向所观察对象的`std::shared_ptr`实例。如果对象仍然存在（即对应的`std::shared_ptr`的引用计数不为0），`lock`方法会返回一个有效的`std::shared_ptr`，可以像使用其他`std::shared_ptr`一样来访问对象；如果对象已经被删除（对应的`std::shared_ptr`的引用计数为0），`lock`方法将返回一个空的`std::shared_ptr`。例如：
```cpp
class B;

class A {
public:
    std::shared_ptr<B> b;
};

class B {
public:
    std::weak_ptr<A> a;

    void useA() {
        std::shared_ptr<A> lockedA = a.lock();
        if (lockedA) {
            // 可以安全地使用lockedA指向的对象
        } else {
            // 对象已被删除
        }
    }
};
```

#### 4.2 weak_ptr 的特点

`std::weak_ptr` 具有以下显著特点：

1. **不控制资源生命周期**：`std::weak_ptr` 不负责管理其所指向对象的生命周期，它不会像 `std::shared_ptr` 那样增加对象的引用计数。这使得 `std::weak_ptr` 可以在不影响对象正常生命周期管理的前提下，提供对对象的访问途径。

2. **弱引用特性**：它是对对象的一种弱引用，仅仅提供了对由 `std::shared_ptr` 管理的资源的一个访问手段。`std::weak_ptr` 的主要作用是协助 `std::shared_ptr` 工作，特别是在处理可能出现的循环引用问题时。

3. **构造与赋值**：`std::weak_ptr` 可以从一个 `std::shared_ptr` 或另一个 `std::weak_ptr` 对象构造。`std::shared_ptr` 可以直接赋值给 `std::weak_ptr`。例如：
```cpp
std::shared_ptr<int> sharedPtr = std::make_shared<int>(42);
std::weak_ptr<int> weakPtr1(sharedPtr);
std::weak_ptr<int> weakPtr2 = weakPtr1;
```

4. **获取 `std::shared_ptr`**：可以通过 `std::weak_ptr` 的 `lock()` 函数来获得一个 `std::shared_ptr`。如前面所述，`lock()` 方法尝试锁定对象并返回一个有效的 `std::shared_ptr`，前提是对象尚未被释放。否则，返回一个空的 `std::shared_ptr`。

5. **不影响引用计数**：`std::weak_ptr` 的构造和析构不会引起所指向对象引用计数的增加或减少。这一特性使得 `std::weak_ptr` 不会干扰 `std::shared_ptr` 对对象生命周期的正常管理，同时又能在需要时提供对对象的临时访问。

6. **解决循环引用问题**：正如前面所讨论的，`std::weak_ptr` 主要用于解决 `std::shared_ptr` 相互引用时可能出现的死锁问题，即两个或多个 `std::shared_ptr` 相互引用导致引用计数永远不会下降为0，资源无法释放的情况。通过使用 `std::weak_ptr` 打破循环中的一个引用，可以确保对象能够在适当的时候被正确释放。

`std::weak_ptr` 的这些特点使其成为 C++ 内存管理中处理复杂对象关系和生命周期管理的重要工具，特别是在涉及到 `std::shared_ptr` 的场景中，能够有效地避免内存泄漏和资源管理问题。 

### 5. 释放器

在 C++ 的智能指针使用场景中，默认情况下，智能指针对象在析构时会自动调用 `delete` 或 `delete[]` 来释放其所持有的堆内存。然而，在实际编程中，堆内存所代表的对象可能还关联着其他需要回收的资源，比如操作系统的套接字句柄、文件句柄等。此时，就需要自定义智能指针的资源释放函数，以确保这些额外资源也能被正确释放。下面以 `std::unique_ptr` 和 `std::shared_ptr` 为例，详细介绍如何使用释放器来管理非内存资源。

智能指针的释放器（Deleter）本质上是一个函数或者函数对象。当智能指针完成其使命，不再需要其所管理的资源时，便会调用这个释放器来执行资源的释放操作。

以管理文件句柄为例，展示如何为 `std::unique_ptr` 定义释放器。假设我们有一个自定义的结构体 `FileDeleter`，它重载了 `()` 运算符，用于实现文件关闭操作：

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
```

在上述代码中，`FileDeleter` 结构体定义了一个可调用对象，当传入一个 `FILE*` 类型的指针时，会检查指针是否为空，若不为空则调用 `fclose` 函数关闭文件。

接下来，通过以下方式使用 `std::unique_ptr` 和 `FileDeleter` 来管理文件：

```cpp
std::unique_ptr<FILE, FileDeleter> smartFile(fopen("file.txt", "r"));
```

在这行代码中，创建了一个 `std::unique_ptr` 对象 `smartFile`，它指向通过 `fopen` 打开的文件 `file.txt`，并且使用 `FileDeleter` 作为其释放器。当 `smartFile` 离开其作用域时，会自动调用 `FileDeleter` 中的 `()` 运算符重载函数，从而确保文件被正确关闭，避免了文件句柄的泄漏。

同样，对于 `std::shared_ptr`，也可以定义释放器来管理非内存资源。例如：

```cpp
std::shared_ptr<FILE> smartFile(fopen("file.txt", "r"), [](FILE* ptr){
    if (ptr)
    {
        fclose(ptr);
    }
});
```

这里通过 `std::shared_ptr` 的构造函数，第一个参数为通过 `fopen` 打开的文件指针，第二个参数是一个 lambda 表达式，该表达式定义了释放器的行为。当最后一个指向该文件的 `std::shared_ptr` 对象销毁时，会调用这个 lambda 表达式，进而关闭文件。

通过为智能指针自定义释放器，我们能够有效地管理各种非内存资源，确保在对象生命周期结束时，所有相关资源都能得到正确释放，提高程序的稳定性和资源利用率。无论是 `std::unique_ptr` 还是 `std::shared_ptr`，这种机制都为处理复杂的资源管理场景提供了强大的支持。 

### 99. quiz

#### 1. 智能指针如何变为常量形式的？

```cpp
struct MyClass {
    int value;
};

std::shared_ptr<const MyClass> ptr = std::make_shared<MyClass>();
// 下面的代码将无法编译，因为ptr指向一个常量对象
// ptr->value = 42;
```
