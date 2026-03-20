
#include <stdio.h>

#include "OpenGL.h"

   

int main(void)
{
    
    OpenGL gl;
    gl.initWindow();
    gl.Render();

    return 0;
}

//static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    printf("%f %f\n", xpos, ypos);
//}