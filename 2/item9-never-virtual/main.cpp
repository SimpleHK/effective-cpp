#include <iostream>

/*
条款 9: 绝不在构造和析构过程中调用虚函数
在创建派生类对象时，基类的构造函数永远会早于派生类的构造函数被调用，而类的析构函数永远会晚于派生类的析构函数被调用。

基类构造 -> 派生类构造 -> 派生类析构 -> 基类析构

! 在派生类对象的基类构造和析构期间，对象的类型是基类而非派生类，
! 在运行基类构造函数时，派生类的数据成员没有初始化
因此此时调用虚函数会被编译器解析至基类的虚函数版本，通常不会得到我们想要的结果。

间接调用虚函数是一个难以发现的危险行为，需要尽量避免
*/

// class Transaction {
// public:
//     Transaction () {
//         std::cout << "正在构造 Transaction" << std::endl;
//         logTransaction();

//         Init(); // 间接调用了虚函数, 运行报错
//     }

//     virtual void logTransaction() const = 0 {
//         std::cout << "记录了一条Transaction类型的记录" << std::endl;
//     }

//     void Init() { // 此处间接调用了虚函数
//         logTransaction();
//     }
// };

// class BuyTransaction : public Transaction {
// public:
//     BuyTransaction() {
//         std::cout << "正在构造 BuyTransaction" << std::endl;
//     }

//     virtual void logTransaction() const
//     {
//         std::cout << "记录了一条BuyTransaction类型的记录" << std::endl;
//     }
// };

// 如果想要基类在构造时就得知派生类的构造信息，推荐的做法是在派生类的构造函数中将必要的信息向上传递给基类的构造函数
class Transaction {
public:
    explicit Transaction (const std::string& logInfo) {
        std::cout << "正在构造 Transaction" << std::endl;
        logTransaction(logInfo); // 更改为了非虚函数调用
    }

    void logTransaction(const std::string& logInfo ) const {
        std::cout << logInfo << std::endl;
    }
};

class BuyTransaction : public Transaction {
public:
    BuyTransaction(const std::string& logInfo) : Transaction(CreateLogString(logInfo)) {
        std::cout << "正在构造 BuyTransaction" << std::endl;
    }

private:
    // ! 注意此处的CreateLogString是一个静态成员函数，这是很重要的，因为静态成员函数可以确保不会使用未完成初始化的成员函数
    static std::string CreateLogString(const std::string& logInfo) {
        return logInfo;
    }

};


int main()
{
    /*
    派生类对象的基类部分在派生类部分之前构造。
    在构造基类时，虚函数永远不会进入派生类。

    在运行基类构造函数时，派生类的数据成员没有初始化。
        而派生类函数几乎肯定会引用局部数据成员。
    在派生类构造函数开始执行之前，对象被视为基类对象。
    同样的道理也使用于析构函数。
     */
    // BuyTransaction b; // 会调用 Transaction 的 logTransaction()，而不是 BuyTransaction 的

    BuyTransaction b("记录了一条BuyTransaction类型的记录");
    return 0;
}
