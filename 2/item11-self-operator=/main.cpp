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

    Widget& operator=(const Widget& rhs) { // 返回的类型是当前类的一个引用
        delete pb;                  // 停止使用当前的 bitmap
        pb = new Bitmap(*rhs.pb);   // 开始使用 rhs 的 bitmap 的一个副本
        return *this;               // 返回左值对象
        // *this(赋值的目标)和 rhs 可以是同一个对象，导致最终指向一个已被删除的对象
    }

    Widget& operator=(int rhs) // 即使参数类型不是 Widget& 也适用
    {
        this->data = rhs;
        return *this; // 返回左值对象
    }

    Widget& operator+=(const Widget& rhs)
    {
        this->data += rhs.data;
        return *this;
    }

    Widget& operator++() // 没有参数，后++
    {
        this->data++;
        return *this;
    }

    Widget& operator++(int a) // 有参数，前++
    {
        ++this->data;
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
    ++((a = b += c = 10)++);
    std::cout << "a = " << a.getData() << " b = " << b.getData() << " c = " << c.getData() << std::endl;
    return 0;
}