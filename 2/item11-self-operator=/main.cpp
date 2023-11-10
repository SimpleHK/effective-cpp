#include <iostream>

/*
条款11： 在 operator= 中处理“自我赋值”
自我赋值是合法的操作，但在一些情况下可能会导致意外的错误，例如在复制堆上的资源时：
*/

class Widget;

void Self()
{
    Widget w(2);
    w = w; // 给自己赋值，这看起来很傻，但它是合法的，所以不要认定客户不会这么做。

    int a[5];
    uint8_t i, j;
    a[i] = a[j]; // 潜在的自我赋值，如果 i == j

    int *px, *py;
    *px = *py;  // 潜在的自我赋值。 如果都指向同一片内存空间
}

/*
如果两个对象来自同一个继承结构，甚至不需要将他们声明为同一类型，
因为基类引用或指针可以引用或指向派生类类型的对象
*/
class Base {};
class Derived : public Base {};
void doSomething(const Base& rb, Derived* pd); // rb 和 *pd 可能是同一个对象

/*
如果试图自己管理资源，可能会陷入这样的陷阱：在使用完资源之前不小心释放了资源。
*/
class Bitmap {};

// 自定义的类, 实现赋值操作符时应该遵循这个约定，尽量与内置类型保持一致
class Widget {
public:
    Widget(int cnt) : data(cnt) {

    }

    // 直接这么写不具备自我赋值安全
    Widget& operator=(const Widget& rhs) { // 返回的类型是当前类的一个引用
        delete pb;                  // 停止使用当前的 bitmap
        pb = new Bitmap(*rhs.pb);   // 开始使用 rhs 的 bitmap 的一个副本
        return *this;               // 返回左值对象
        // *this(赋值的目标)和 rhs 可以是同一个对象，导致最终指向一个已被删除的对象
    }

// 如何实现自我赋值安全
    // 1. 在 operator= 的顶部检查是否对自己赋值
    Widget& operator=(const Widget& rhs) { // 返回的类型是当前类的一个引用
        if (this == &rhs)
        {
            return *this; // 如果是自我赋值，什么也不做
        }
        delete pb;                  // 停止使用当前的 bitmap
        pb = new Bitmap(*rhs.pb);   // 开始使用 rhs 的 bitmap 的一个副本
        return *this;               // 返回左值对象
        // *this(赋值的目标)和 rhs 可以是同一个对象，导致最终指向一个已被删除的对象
    }
// 它还有异常安全性的问题：如果“new Bitmap”表达式产生异常（因为没有足够的内存用于分配，
// 或者因为Bitmap的拷贝构造函数抛出异常），并最终指向自己删除的Bitmap对象。

// 幸运的是，使 operator= 具有异常安全性，通常也会具有自我赋值安全性。
    // 2 另一个常见的做法是只关注异常安全性，而不关注是否自我赋值：
    // 仅仅是适当安排语句的顺序，就可以做到使整个过程具有异常安全性。
    Widget& Widget::operator=(const Widget& rhs)
    {
        Bitmap* pOrig = pb;
        pb = new Bitmap(*rhs.pb); // 如果 new Bitmap 抛出异常，pb(以及它所在的Widget)保持不变
        delete pOrig;
        return *this;
    }
    // 即使没有自我赋值测试，上面的代码也具有自我赋值安全性，因为创建呢原始Bitmap的一个副本，删除了原始Bitmap，然后指向创建的副本。

    // 3 使用拷贝和交换
    // copy and swap 技术，这种技术聪明地利用了栈空间会自动释放的特性，这样就可以通过析构函数来实现资源的释放：
    Widget& operator=(const Widget& rhs)
    {
        Widget temp(rhs);
        std::swap(*this, temp);
        return *this;
    }

    // 上述做法还可以写的更加巧妙，就是利用按值传参，自动调用构造函数：
    Widget& operator=(Widget rhs) // 按值传递某项内容会生成该实参的副本，按值传参相当于做了拷贝，并且在函数调用完后会自动释放
    {
        std::swap(*this, rhs);
        return *this;
    }

    int getData()
    {
        return data;
    }

private:
    int data;
    Bitmap* pb; // 指向一个从堆中分配的对象
};


int main()
{
    Widget a = Widget(1); 
    Widget b = Widget(2); 
    Widget c = Widget(5);

    // a = b += c = 10; // c = 10 调用 Widget& operator=(int rhs)
    std::cout << "a = " << a.getData() << " b = " << b.getData() << " c = " << c.getData() << std::endl;
    return 0;
}

// 总结：
// 当对象被赋值给自身时，确保 operator= 表现良好。
// 技术途径包括：比较源对象和目标对象的地址、仔细的语句排序、copy-and-swap
// 确保在两个和多个对象相同（是同一个对象）的情况下，函数的行为仍然正确。