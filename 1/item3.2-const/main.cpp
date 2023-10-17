// 条款3:尽可能使用const (Use const whenever possible)
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

/*
在成员函数上使用const:
    使类的接口意图更明确，知道哪些函数可以修改一个对象，哪些不能，这很重要
    使得使用const对象成为可能

C++的一个重要特性，仅在常量上不同的成员函数可以被重载。
*/
class TextBlock {
public:
    TextBlock(std::string str) {
        text = str;
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

    const char& operator[](std::size_t position) const {
        std::cout << "all const" << std::endl;
        return text[position];
    }

private:
    std::string text;
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
    // constTextBlock[1] = 'a'; // 错误，[] 返回的是const char&
    std::cout << constTextBlock[1] << std::endl;

    print(textBlock);

    return 0;
}

// 总结
