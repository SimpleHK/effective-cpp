#include <iostream>

/*
条款10： 令opetator= 返回一个指向 *this的引用
虽然并不强制执行此条款，但为了实现连锁赋值，大部分时候应该这样做：
这个约定同样适用于 +=, -=, *=等
*/

void ChainAssignment()
{
    // 赋值的一个有趣之处在于，可以将他们串在一起
    int x, y, z;
    x = y = z = 15;
}
// 自定义的类, 实现赋值操作符时应该遵循这个约定，尽量与内置类型保持一致
class Widget {
public:
    Widget(int cnt) : data(cnt) {

    }

    Widget& operator=(const Widget& rhs) { // 返回的类型是当前类的一个引用
        this->data = rhs.data;
        return *this; // 返回左值对象
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