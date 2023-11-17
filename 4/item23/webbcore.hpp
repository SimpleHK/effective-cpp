#pragma once
#include <stdio.h>

namespace WebBrowserStuff {
    class WebBrowser { };

    void coreFunl(WebBrowser& wb)
    {
        puts("coreFun1");
    }

    void coreFun2(WebBrowser& wb)
    {
        puts("coreFun2");
    }
}