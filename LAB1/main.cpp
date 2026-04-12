
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

#include "OpenGL.h"

int test(); 

int main(int arg_c, char * args[])
{
    
    if (arg_c > 1)
    {
        for (int i = 1; i < arg_c; ++i)
        {
            std::string s(args[i]);
            if (s == "--test")
            {
                return test();
            }
        }
    }

    OpenGL gl;
    gl.initWindow();
    gl.Render();

    return 0;
}




int test()
{
    try
    {
       OpenGL gl;
       gl.initWindow();
       gl.Render(100);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
    
}