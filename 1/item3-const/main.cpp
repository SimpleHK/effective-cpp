// 条款3:尽可能使用const (Use const whenever possible)
#include <iostream>
#include <string>
#include <vector>

char greeting[] = "Hello";
const char* const p = greeting; // const 出现在*的左侧，指向的是常量， 右侧，指针本身是常量（常量指针）

int main()
{
    // STL迭代器以指针位模型，因此迭代器的行为很像 T* 指针
    std::vector<int> vec;
    // iter 的行为像 T* const
    const std::vector<int>::iterator iter = vec.begin();
     // ! 因为const 修饰 iter, 迭代器的行为很像 T* ，这个迭代器可以看作 => T* const iter, 常量指针，所以内容可以改变
    *iter = 10;
    // ++iter; // 错误，iter是const

    // citer 的行为像 const T*
    std::vector<int>::const_iterator citer = vec.begin();
    // *citer = 10; // 错误！ *citer 是const
    ++citer;

    return 0;
}

// 总结
