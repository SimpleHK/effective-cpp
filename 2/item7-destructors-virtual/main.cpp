#include <iostream>
#include <list>
#include <vector>

/*
条款 7：为多态基类声明虚析构函数
    当派生类对象经由一个基类指针被删除，而该基类指针带着一个非虚析构函数，其结果是未定义的，可能会无法完全销毁派生类的成员，造成内存泄漏。消除这个问题  的方法就是对基类使用虚析构函数：
    几乎可以肯定，任何具有虚函数的类（基类）都应该有虚析构函数
*/

class TimeKepper {
public:
    TimeKepper() {
        std::cout << "TimeKepper structure ing\n"; 
    }

    virtual ~TimeKepper() {
        std::cout << "TimeKepper destruction ing\n"; 
    }
};

class AtomicClock : public TimeKepper {
public:
    AtomicClock() {
        std::cout << "AtomicClock structure ing\n"; 
    }

    ~AtomicClock() {
        std::cout << "AtomicClock destruction ing\n"; 
    }
};

class WaterClock : public TimeKepper {
public:
    WaterClock() {
        std::cout << "WaterClock structure ing\n"; 
    }

    ~WaterClock() {
        std::cout << "WaterClock destruction ing\n"; 
    }
};

// class WristClock : public TimeKepper {
// public:
//     WristClock() {
//         std::cout << "WristClock structureing\n"; 
//     }

//     ~WristClock() {
//         std::cout << "WristClock destructioning\n"; 
//     }
// };

TimeKepper* GetTimeKepper(int kind)
{
    switch (kind) {
    case 1:
        return new AtomicClock();
    break;
    case 2:
        return new WaterClock();
    break;
    default:
    break;
    }

    return new TimeKepper();
}

/* 
当一个类不打算成为基类时，将析构函数设为虚函数通常不是一个好主意，
因为额外存储的虚表指针会使类的体积变大，并且需要为每个类创建虚表指针数组。
只要基类的析构函数是虚函数，那么派生类的析构函数不论是否用virtual关键字声明，都自动成为虚析构函数。
 */

class Point {
public:
    Point(int xCoord, int yCoord);
    ~Point();

private:
    // 如果int 占用32位，Point对象通常可以转入64位寄存器，并可以作为64位的数据传递给用其他语言编写的函数，如C或FORTRAN
    // 如果使用虚函数，需要额外的，名为vptr(virtural table pointer)的指针， vptr指向一个称为 vtbl(virtual table)的函数指针数组
    int x, y;
};

class VPoint {
public:
    VPoint(int xCoord, int yCoord);
    virtual ~VPoint();

private:
    // 如果int 占用32位，Point对象通常可以转入64位寄存器，并可以作为64位的数据传递给用其他语言编写的函数，如C或FORTRAN
    // 如果使用虚函数，需要额外的，名为vptr(virtural table pointer)的指针， vptr指向一个称为 vtbl(virtual table)的函数指针数组
    int x, y;
};

/*
虚析构函数的运作方式是，最深层派生的那个类的析构函数最先被调用，然后是其上的基类的析构函数被依次调用。
如果你想将基类作为抽象类使用，但手头上又没有别的虚函数，那么将它的析构函数设为纯虚函数是一个不错的想法。考虑以下情形：
*/
class Base {
public:
    virtual ~Base() = 0;
};

/*
但若此时从该基类中派生出新的类，会发生报错，这是因为编译器无法找到基类的析构函数的实现。因此，即使是纯虚析构函数，也需要一个函数体：
*/
Base::~Base() {}

/* 或者以下写法也被允许：*/
class BaseA {
public:
    virtual ~BaseA() = 0 {}
};

int main()
{
    TimeKepper* ptk = GetTimeKepper(1);
    delete ptk;

    std::cout << sizeof(int*) << std::endl;     // 8B
    std::cout << sizeof(Point) << std::endl;    // 8B
    std::cout << sizeof(VPoint) << std::endl;   // 16B，多包含了一个虚表指针，并且为其创建了虚表
}

/*
多态基类应该声明虚析构函数。如果有一个类有虚函数，它应该有一个虚析构函数。
非基类或非多态使用的类不应该声明虚析构函数。
*/
