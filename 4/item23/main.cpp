#include "webbdisplay.hpp"
#include "webbcore.hpp"
#include <iostream>
// #include "wbbclear.hpp" 不使用将不会参与编译

using namespace WebBrowserStuff;

int main()
{
    WebBrowser webb;

    coreFunl(webb);
    coreFun2(webb);
    
    displayFun1(webb);
}