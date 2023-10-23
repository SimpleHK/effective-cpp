// 条款3:尽可能使用const (Use const whenever possible)
#include <iostream>
#include <string>
#include <vector>

char greeting[] = "Hello";
const char* const p = greeting; // const 出现在*的左侧，指向的是常量， 右侧，指针本身是常量（常量指针）


/*
对于 STL 迭代器，分清使用const还是const_iterator:
const std::vector<int>::iterator iter = vec.begin();    // 迭代器不可修改，数据可修改
std::vector<int>::const_iterator iter = vec.begin();    // 迭代器可修改，数据不可修改
*/
void ConstSTL()
{
    // STL迭代器以指针位模型，因此迭代器的行为很像 T* 指针
    std::vector<int> vec{0, 1};
    // iter 的行为像 T* const
    const std::vector<int>::iterator iter = vec.begin();
     // ! 因为const 修饰 iter, 迭代器的行为很像 T* ，这个迭代器可以看作 => T* const iter, 常量指针，所以内容可以改变
    *iter = 10;
    std::cout << vec[0] << ',' << iter[0] << std::endl;
    // ++iter; // 错误，iter是const

    // citer 的行为像 const T* citer
    std::vector<int>::const_iterator citer = vec.begin();
    // *citer = 10; // 错误！ *citer 是const
    ++citer;
    std::cout << vec[0] << ',' << citer[0] << std::endl;
}

int main()
{
    ConstSTL();
    return 0;
}

// 总结
