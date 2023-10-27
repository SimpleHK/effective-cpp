#include <iostream>

/*
条款 8：别让异常逃离析构函数
在析构函数中发生异常的处理策略：
1. 中止程序
2. 吞下异常，并在日志中记录异常
    这两种策略，异常都没有被真正解决，是没有办法的办法
3. 将异常交给调用者处理，以便其使用者有机会对可能出现的问题作出反应
*/

// 为了实现 RAII（资源的获取就是初始化）思想，我们通常会将对象的销毁方法封装在析构函数中，如下例子：

class DBConnection {
public:
    void close() {
        // close connection
    }
};

class DBConn {
public:
    // ~DBConn() {
    //     db.close();    // 该函数可能会抛出异常
    // }

    ~DBConn();

    void close();

private:
    DBConnection db;
    bool closed;
};

// // 第一种：杀死程序：
// DBConn::~DBConn() {
//     try {
//         db.close();
//     }
//     catch (...) {
//         // 记录运行日志，以便调试
//         std::abort();
//     }
// }

// // 第二种：直接吞下异常不做处理，但这种做法不被建议：
// DBConn::~DBConn() {
//     try {
//         db.close();
//     }
//     catch (...) {
//         // 记录运行日志，以便调试
//     }
// }

// 第三种：重新设计接口，将异常的处理交给客户端完成：
void DBConn::close() {
    db.close();
    closed = true;
}

// 若客户忘记手动调用，析构函数才会自动调用close函
DBConn::~DBConn() {
    if (closed == false) {
        try {
            db.close();
        }
        catch(...) {
            // 处理异常
        }
    }
}

/*
在这个新设计的接口中，我们提供了close函数供客户手动调用，这样客户也可以根据自己的意愿处理异常；若客户忘记手动调用，析构函数才会自动调用close函数。

当一个操作可能会抛出需要客户处理的异常时，将其暴露在普通函数而非析构函数中是一个更好的选择。
*/

int main()
{
    return 0;
}

/*
总结
析构函数永远不应该吐出异常。如果在析构函数中调用的函数可能抛出异常，则析构函数应该捕获任何异常，然后将其吞下或终止程序。
如果类的调用者需要能够对操作期间抛出的异常做出反应，则类应该提供执行改操作的普通（即非析构函数）函数。
*/