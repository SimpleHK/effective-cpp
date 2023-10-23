// 条款3:尽可能使用const (Use const whenever possible)
#define use _CRT_SECURE_NO_WARNINGS
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
/*
在成员函数上使用const:
    使类的接口意图更明确，知道哪些函数可以修改一个对象，哪些不能，这很重要
    使得使用const对象成为可能

C++的一个重要特性，仅在常量上不同的成员函数可以被重载。

当成员函数是指针时，即使创建了const对象，并使用了const成员函数，指针指向的内容仍然可能会发生改变，这可能不是我们的本意
mutable可以让非静态的成员变量从按位常量中解放出来，即使是const对象，const成员函数, mutable成员变量被改变，编译器也不会报错
*/
class TextBlock {
public:
    TextBlock(std::string str) {
        text = str;
        pText = new char[100];
        std::strcpy(pText, str.data());
    }

    char& operator[](std::size_t position) {
        std::cout << "no const" << std::endl;
        return text[position];
    }

    // ! 返回类型不能作为重载条件
    // const char& operator[](std::size_t position) {
    //     std::cout << "no const" << std::endl;
    //     return text[position];
    // }

    // ! 将带char类型的引用绑定到 const char将会丢弃const
    // char& operator[](std::size_t position) const {
    //     std::cout << "no const" << std::endl;
    //     return text[position];
    // }

    // const char& operator[](std::size_t position) const {
    //     std::cout << "all const" << std::endl;
    //     return text[position];
    // }

    /*
    ! 即使使用 const 创建了常量对象，并调用了const成员函数，仍然改变了pText的值，这有些不对劲
    因为c++使用的是位常量，char* 指针的指向并没有发生改变，就没有违反cons成员函数的语义，
    但是char*指向的内容却被改变了，这在大多数时候不是我们的本意
    */
    char& operator[](std::size_t position) const
    {
        std::cout << "all const" << std::endl;
        return pText[position];
    }

private:
    std::string text;
    char* pText;
};

void print(const TextBlock& ctb)
{
    std::cout << "printf:" << ctb[0] << std::endl; // 调用const版本, 因为形参是const
}

int main()
{
    TextBlock textBlock("abc");
    textBlock[1] = 'a'; // 优先选择限制最少，功能更强的函数
    std::cout << textBlock[1] << std::endl;

    const TextBlock constTextBlock("abc");
    constTextBlock[1] = 'a'; // 如果用string会报错
    std::cout << constTextBlock[1] << std::endl;

    print(textBlock);

    return 0;
}

// 总结
