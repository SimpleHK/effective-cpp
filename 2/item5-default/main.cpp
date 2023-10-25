#include <iostream>
#include <list>
#include <vector>

/*
条款5：了解C++默默编写并调用了哪些函数

拷贝构造函数和拷贝赋值运算符：编译器生成的版本只是简单的将源对象的每个非静态数据成员拷贝到目标对象（只是浅拷贝）

*/

/* 5.1 了解C++默默编写并调用了哪些函数 */
// C++ 中的空类并不是真正意义上的空类，编译器会为他预留以下内容：
class Empty {
public:
    // Empty() {}                                  // 默认构造函数(没有任何构造函数时)
    // Empty(const Empty&) {}                      // 拷贝构造函数
    // Empty(const Empty&&) {}                     // 移动构造函数 (C++ 11)
    // ~Empty() {}                                 // 析构函数

    // Empty& operator=(const Empty& empty) {}     // 拷贝赋值运算符
    // Empty& operator=(Empty&&) {}                // 移动赋值运算符  (C++ 11)

    // Empty& operator=(const Empty& empty) {      // 拷贝赋值运算符
    //     if (this != &empty) {
    //         a = empty.a;
    //     }

    //     return *this;
    // }

    int a;
};

int GenerateDefaultFunction()
{
    // ! 唯有当这些函数被调用时，他们才会真正被编译器创建出来，下面代码将造成上述每一个函数被创建
    // 测试这段代码需要注释上面自己写的函数，保持 Empty 是一个空类
    Empty e1;                   // 默认构造函数 & 析构函数
    Empty e2(e1);               // 拷贝构造函数
    Empty e3 = std::move(e2);   // 移动构造函数 (since C++11)
    e2 = e1;                    // 拷贝赋值运算符
    e3 = std::move(e1);         // 移动赋值运算符 (since C++11)

    return 0;
}

/*
5.2 了解编译器什么时候会拒绝为你的类生成相应的代码 
编译器生成的函数往往都比较简单，当有歧义时，编译器就会拒绝生成
*/

// template<class T>
// class NamedObject {
// public:
//     NamedObject(std::string& name, const T& value) : nameValue(name), objectValue(value)
//     {

//     }

//     void printIfo()
//     {
//         std::cout << "name:" << nameValue << "age:" << objectValue << std::endl;
//     }

// private:
//     std::string& nameValue;
//     const T objectValue;
// };

// std::string newDog("Persephone");
// std::string oldDog("Satch");

// NamedObject<int> p(newDog, 2);
// NamedObject<int> s(oldDog, 36);

/*
编译器生成的版本只是简单的将源对象的每个非静态数据成员拷贝到目标对象
c++ 拒绝生成默认拷贝赋值运算符，拷贝赋值运算符只有在允许存在时才会自动创建
1. 这会导致 p 和 s 中的 std::string& nameValue; 的引用都指向 oldDog，
然而，C++ 没有提供让引用指向不同对象的方法。 因为如果引用可以指向不同的对象，如果改变引用的内容，恐怕会影响其他对象的指针或引用
2. 常量不能被赋值，只能进行初始化。objectValue 是一个常量，p.objectValue = s.objectValue，对常量进行了赋值
*/
// p = s; // p中的数据成员应该如何处理？


/*
如果基类的拷贝赋值运算符声明为private，它的派生类将无法生成拷贝赋值运算符，因为无法调用基类部分的函数处理基类部分

*/

// 手动实现拷贝赋值运算符 
template<class T>
class NamedObject {
public:
    NamedObject(std::string& name, const T& value) : nameValue(name), objectValue(value)
    {

    }

    void operator=(const NamedObject& item)
    {
        std::cout << ">>>>>>>>>>" << std::endl;
        this->nameValue = item.nameValue; // newDog = oldDog, newDog会被改变
    }

    void printIfo()
    {
        std::cout << "name:" << nameValue << " age:" << objectValue << std::endl;
    }

private:
    std::string& nameValue;
    const T objectValue;
};

int main()
{
    std::string newDog("Persephone");
    std::string oldDog("Satch");

    NamedObject<int> p(newDog, 2);
    NamedObject<int> s(oldDog, 36);
    p = s;
    p.printIfo();
    s.printIfo();

    std::cout << newDog << std::endl;

    NamedObject<int> t = p; // 这样调用的是拷贝构造函数，t.nameValue 和 p.nameValue 引用都指向了 newDog
    t.printIfo();
    newDog = "abc";
    t.printIfo();
    p.printIfo();

    return 0;
}