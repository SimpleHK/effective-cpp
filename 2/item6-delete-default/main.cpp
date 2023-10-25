#include <iostream>
#include <list>
#include <vector>

/*
条款 6：若不想使用编译器自动生成的函数，就该明确拒绝
*/

// 原书中使用的做法是将不想使用的函数声明为private，但在 C++11 后我们有了更好的做法：使用 delete 关键字

class Base {
public:
    // virtual ~Base() = 0;
    // virtual void PrintInfo() = 0;
};

class Uncopyable : public Base {
public:
    Uncopyable() = default;
    Uncopyable(const Uncopyable&) = delete;
    // void PrintInfo() override; // only virtual
    // void PrintInfo() final;    // only virtual
    Uncopyable& operator=(const Uncopyable&) = delete;
};

int main()
{
    Uncopyable u1;
    Uncopyable u2;
    // u2 = u1; // 不能调用，因为拷贝赋值被删除了
}
