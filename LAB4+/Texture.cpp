#include "Texture.h"

#include <algorithm>
#include <iostream>
#include <format>

// Библиотека для разгрузки изображений
// https://github.com/nothings/stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::~Texture()
{
    if (texId != 0)
        glDeleteTextures(1, &texId);
}

void Texture::LoadTexture(const std::string& texture_file_name)
{
    if (texId != 0)
        glDeleteTextures(1, &texId);

    // Генерируем ID текстуры
    glGenTextures(1, &texId);

    int curent_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &curent_texture);

    // Делаем текущую текстуру активной
    glBindTexture(GL_TEXTURE_2D, texId);


    // x - ширина изображения
    // y - высота изображения
    // n - количество каналов
    // 4 - нужное нам количество каналов
    // Пиксели будут хранится в памяти [R-G-B-A]-[R-G-B-A]-[.....
    //  по 4 байта на пиксель - по байту на канал
    // Пустые каналы будут равны 255
    int x, y, n;

    // Загружаем картинку
    // см. #include "stb_image.h"
    unsigned char* data = stbi_load(texture_file_name.c_str(), &x, &y, &n, 4);

    if (data)
    {
        // Картинка хранится в памяти перевернутой,
        // так как ее начало в левом верхнем углу;
        // поэтому мы ее переворачиваем -
        // меняем первую строку с последней,
        // вторую с предпоследней, и.т.д.

        for (int i = 0; i < y / 2; ++i)
        {
            unsigned char* from = data + i * x * 4;
            unsigned char* to = data + (y - 1 - i) * x * 4;
            std::swap_ranges(from, from + x*4, to);
        }


        // Загрузка изображения в видеопамять
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // Выгрузка изображения из опперативной памяти
        stbi_image_free(data);

    }
    else
    {
        std::cerr << std::format("Error loading texture {}", texture_file_name) << std::endl;
    }

    
    // Настройка режима наложения текстур
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // GL_REPLACE -- полная замена политога текстурой
    // Настройка тайлинга
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Настройка фильтрации
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //======================================================

    glBindTexture(GL_TEXTURE_2D, curent_texture);
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, texId);
}
