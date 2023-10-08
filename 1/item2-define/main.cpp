#include <iostream>
#include <string>

// // #define ASPECT_RATIO 1.653
// // 优先使用编译器，而不是预处理器
// constexpr double aspectRation = 1.653;

// int main()
// {
//     // std::cout << "纵横比" << ASPECT_RATIO << std::endl;
//     std::cout << "常量纵横比" << aspectRation << std::endl;
//     return 0;
// }

// #define AUTHOR_NAME "Scott Meyers"
// // const char * authorName = "Scott Meyers"; // 指向仍然可以被改变
// // const char *const authorName = "Scott Meyers";
// const std::string authorName = "Scott Meyers";

// int main()
// {
//     int i = 5;
//     int a[i];
    
//     std::cout << "作者" << sizeof(a) << std::endl;
//     return 0;
// }

// class GamePlayer {
// private:
//     #define NUM_TURNS 5
//     static const int NumTurns = 5;
//     int scores[NumTurns];
// public:
//     int numTurns() {
//         return NumTurns;
//     }
// };

// int main()
// {
//     GamePlayer player;
    
//     std::cout << "NumTurns" << player.numTurns() << std::endl;
//     return 0;
// }

int f(int num)
{
    return num;
}

// 通过使用内联函数模板，可以获得宏函数的所有效率(减少函数调用开销)，以及普通函数的所有可预测行为和类型安全
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))

template<typename T>
inline T CallWithMax(const T& a, const T& b) // 引用传递性能更好，并且const不会改变原始数据
{
    return f((a) > (b) ? (a) : (b));
}

int main()
{
    int a = 5, b = 0;
    std::cout << CALL_WITH_MAX(++a, b) << std::endl; // ++a在这里执行了两次，我们只希望执行一次
    std::cout << CallWithMax(++a, b) << std::endl;
    return 0;
}

// 总结
// 对于简单常量，首选const对象或枚举，而不是#define
// 对于类似函数的宏，优先使用内联函数
