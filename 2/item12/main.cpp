#include <iostream>
#include <string>
/*
条款12：复制对象时勿忘记其每一个成分
当使用继承时，继承自基类的成员往往容易忘记在派生类中完成复制，
如果你的基类拥有拷贝构造函数和拷贝赋值运算符，应该记得调用他们
编译器生成的拷贝函数，会拷贝对象的所有数据，当你声明自己的拷贝函数时，就是在告诉编译器，默认实现中有你不喜欢的地方。
*/

void logCall(const std::string& funcName)
{
    std::cout << funcName << "已经记录" << std::endl;
}

class Date {
public:
    void setDay(int d)
    {
        day = d;
    }

    int getDay()
    {
        return day;
    }

private:
    int day;

};

class Customer {
public:
    Customer() {}
    Customer(const Customer& rhs) : name(rhs.name)
    {
        logCall("Customer copy constructor");
    }
    Customer& operator=(Customer rhs)
    {
        // std::swap(*this, rhs);
        name = rhs.name;
        return *this;
    }

    void setName(const std::string str)
    {
        name = str;
    }

    std::string getName()
    {
        return name;
    }

    void print()
    {
        std::cout << "name:" << name << std::endl;
    }

    void setDate(Date d)
    {
        lastTransaction = d;
    }

private:
    std::string name;
    Date lastTransaction;
};

class PriorityCustomer : public Customer {
public:
    PriorityCustomer(const PriorityCustomer& rhs);
    PriorityCustomer& operator=(const PriorityCustomer& rhs);

private:
    int priority;
};

/*
当使用继承时，继承自基类的成员往往容易忘记在派生类中完成复制，
如果你的基类拥有拷贝构造函数和拷贝赋值运算符，应该记得调用它们：
*/
PriorityCustomer::PriorityCustomer(const PriorityCustomer& rhs) :
    Customer(rhs),              // 调用基类的拷贝构造函数
    priority(rhs.priority)
{

}

PriorityCustomer& PriorityCustomer::operator=(const PriorityCustomer& rhs)
{
    Customer::operator=(rhs);   // 调用基类的拷贝赋值运算符 
    priority = rhs.priority;
    return *this;
}
/*
! 注意，不要尝试在拷贝构造函数中调用拷贝赋值运算符，或在拷贝赋值运算符的实现中调用拷贝构造函数，
一个在初始化时，一个在初始化后，他们的功用是不同的。

如果拷贝构造函数和拷贝赋值函数有很多重复的代码，可以将重复部分的代码抽离为第三个函数
*/

int main()
{
    Date d;
    d.setDay(28);

    Customer c1;
    c1.setName("客户A");
    c1.print();


    Customer c2(c1);
    Customer c3;
    c3 = c1;

    c1.print();
    c2.print();
    c3.print();

    return 0;
}

// 总结：
// 拷贝函数应该确保拷贝对象的所有数据成员和它所有的基类部分
// 不要试图用其中一个拷贝函数来实现另一个。