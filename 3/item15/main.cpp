#include <iostream>
#include <string>
/*
条款15: 在资源管理类中提供对原始资源的访问
原因：许多API直接引用资源（要求直接传入原始资源，而不是RAII对象），
所以有时候你将不得不绕过资源管理对象，处理原始资源。

解决办法：将RAII对象转换为API需要的类型, 显示转换和隐式转换
*/

int main()
{

    return 0;
}