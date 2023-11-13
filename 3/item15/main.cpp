#include <iostream>
#include <string>
/*
条款15: 在资源管理类中提供对原始资源的访问
原因：许多API直接引用资源（要求直接传入原始资源，而不是RAII对象），
所以有时候你将不得不绕过资源管理对象，处理原始资源。

解决办法：将RAII对象转换为API需要的类型, 显示转换和隐式转换
*/

struct FontHandle {
    int ID = 0;
    bool isoccupied = false;
    int size = 8;
};

FontHandle getFont(FontHandle fh)
{
    fh.isoccupied = true;
    std::cout << "获取FontHandle对象, ID为" << fh.ID << std::endl;

    return fh;
}

void releaseFont(FontHandle fh)
{
    fh.isoccupied = false;
    std::cout << "释放FontHandle对象, ID为" << fh.ID << std::endl;
}

void ChangeFontSize(FontHandle fh, int newSize)
{
    fh.size = newSize;
    std::cout << "ID为" << fh.ID << "的size改变为" << fh.size << std::endl;
}

class Font {
public:
    explicit Font(FontHandle fh) : f(fh)
    {
    }

    ~Font()
    {
        releaseFont(f);
    }

    FontHandle get() const
    {
        return f; // 显示转换
    }

    /*
    operator FontHandle() const { return handle; }: 这是一个隐式的类型转换函数，
    它允许 Font 对象在需要 FontHandle 类型的地方被隐式地转换为 FontHandle。
    这意味着编译器会在需要时自动调用这个转换函数。
    */
    operator FontHandle() const // 隐式转换函数
    {
        std::cout << "隐式转换" << std::endl;
        return f;
    }

private:
    FontHandle f;
};

int main()
{
    FontHandle fh;
    Font f(getFont(fh));

    ChangeFontSize(f.get(), 9);     // 明确地将Font转换为FontHandle
    ChangeFontSize(f, 10);          // 隐式地将Font转换为FontHandle

    Font f1(getFont(fh));
    // 假如本想复制一个 font，但是不小心将类型写为fonHandle，因为有隐式转换，没有报错
    // 但当f1被摧毁时，fh2将变为悬吊的
    FontHandle fh2 = f1;

    return 0;
}

/*
经常有API需要访问原始资源，因此每个RAII类都应该提供方法来获取它管理的原始资源。
访问可以通过显示转换和隐式转换。一般来说，显示转换更安全，而隐式转换对客户端来说更方便。
*/