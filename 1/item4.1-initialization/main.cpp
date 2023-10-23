#include <iostream>
#include <list>
#include <vector>

/*
条款4：确保对象在使用之前被初始化

1.不要混淆赋值和初始化。初始化发生在更早的时候--他们的默认构造函数在进入构造函数体之前被自动调用，
内置类型的对象成员（c部分）不能保证在赋值之前就行了初始化，对于内置类型的非对象成员，需要手动完成
*/

int initializer()
{
    // 最佳的方法是始终在使用对象之前初始化他们。
    // 对于内置类型的非成员对象，需要手动完成。如
    int x = 0;
    const char* text = "A C-style string";
    double d;
    std::cin >> d; // 定义的时候虽然没有初始化，但是马上就会赋值，这种情况也是允许的
    return 0; 
}

// 对于类中的成员变量而言，我们有两种建议的方法完成初始化工作，一种是直接在定义处赋初值（since C++11）：
class CTextBlock {
private:
    /*
    1. 使用 {} 进行初始化：大括号初始化是更推荐的方式，因为它提供了更强大的类型安全性和一致性。
    大括号初始化（也称为初始化列表或统一初始化）是C++11引入的新特性，它提供了更强大和一致的初始化语法。使用 {} 初始化有以下特点：
    1.1 不允许窄化（narrowing）转换： 大括号初始化不允许隐式的类型转换，它可以帮助防止潜在的精度损失。如果尝试进行窄化转换，编译器会产生错误。
    1.2 一致性： 无论是内置类型、自定义类型、数组、容器还是其他类型，大括号初始化的语法都是一致的。这提高了代码的可读性和可维护性。
    1.3 初始化列表： 大括号内可以包含一个初始化列表，用逗号分隔多个值。这允许初始化数组、容器和自定义类等。
    */
    std::size_t textLength{ 0 };
    long long a{ 6 };
    int b{ static_cast<int>(a) }; // 显示窄化
    int x = { 42 };
    int y{ x };
    bool lengthIsValid{ false };
    std::vector<int> vec = {1, 2, 3};
};

// 另一种是使用构造函数成员初始化列表：
class PhoneNumber {};
class ABEntry {
public:
    ABEntry(const std::string& name, const std::string& address, const std::list<PhoneNumber>& phones);

private:
    std::string theName;
    std::string theAddress;
    std::list<PhoneNumber> thePhones;
    int numTimesConsulted;
};

// ABEntry::ABEntry(const std::string& name, const std::string& address, const std::list<PhoneNumber>& phones)
// {
//     // 这些都是赋值，而不是初始化，因为在进入构造函数之前，都调用了他们的默认构造函数
//     theName = name;
//     theAddress = address;
//     thePhones = phones;
//     numTimesConsulted = 0; // 因为这是内置类型，没有默认构造函数，所以可以这样，更建议使用 {} 进行初始化
// }

// 使用初始化列表进行初始化
// ! 数据成员按照声明的顺序初始化。与初始化列表中的顺序无关，但最好保持顺序的一致性
ABEntry::ABEntry(const std::string& name, const std::string& address, const std::list<PhoneNumber>& phones) :
theName(name), theAddress(address), thePhones(phones), numTimesConsulted(0) 
{
    // 现在构造函数的函数体是空的 
}

/*
4.1 静态对象初始化：
    C++ 对于定义于不同编译单元内的非局部静态对象（全局静态）的初始化相对次序并无明确定义，因此，以下代码可能会出现使用未初始化静态对象的情况
    编译单元：产生单个目标文件的源代码
    非局部静态对象：函数内部的静态对象称为局部静态对象（因为他们在函数中是局部的），其他类型的静态对象称为非局部静态对象。

    解决办法：这个问题的一个有效解决方案是采用 Meyers’ singleton，将全局静态对象转化为局部静态对象
    将每个非局部静态对象移动到自己的函数中，并将其声明为static。这些函数返回他们包含的对象的引用。客户调用函数，而不是引用对象。
    ! 依据：C++保证，函数内的局部对象会在调用该函数时、第一次遇到该对象的定义时被初始化
*/

// // File 1
// class FileSystem {
// public:
//     std::size_t numDisks() const;
// };

// FileSystem tfs; // 定义了一个非局部静态对象

// // File 2
// extern FileSystem tfs;
// class Directory {
// public:
//     Directory() {
//         // 除非tfs在tempDir之前初始化，否则tempDir的构造函数将尝试在tfs初始化之前使用它。但他们初始化的顺序无法确定
//         FileSystem disk = tfs; // 使用 tfs 对象，非局部静态对象在使用时存在未初始化的风险
//     }
// };
// Directory tempDir;

// File 1
class FileSystem {
public:
    std::size_t numDisks() const;
};

FileSystem& tfs()
{
    static FileSystem fs; // 定义了一个非局部静态对象
    return fs;
}


// File 2
class Directory {
public:
    Directory() {
        // 除非tfs在tempDir之前初始化，否则tempDir的构造函数将尝试在tfs初始化之前使用它。但他们初始化的顺序无法确定
        FileSystem disk = tfs(); // 使用 tfs 对象，非局部静态对象在使用时存在未初始化的风险
        std::size_t disks = tfs().numDisks(); // 在多线程中，最好还是在单线程启动阶段手动调用函数完成初始化
    }
};
Directory tempDir;

int main()
{
    return 0; 
}

/*
总结：
手动初始化内置类型的对象，因为c++不确保会初始化他们。
在构造函数中，优先使用成员初始化列表，而不是在构造函数体内赋值。以类中声明的顺序在初始化列表中列出数据成员。
通过将非局部静态对象
*/