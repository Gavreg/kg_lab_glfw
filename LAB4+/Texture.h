#pragma once

#include<GLFW/glfw3.h>
#include <string>

class Texture
{
    GLuint texId = 0;

  public:
    Texture(){};
    ~Texture();

    void LoadTexture(const std::string& texture_file_name);
    void Bind();
};
