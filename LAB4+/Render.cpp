#include <string>
#include <format>
#include <iostream>
#include <algorithm>

#include "OpenGL.h"
#include "Camera.h"
#include "Light.h"
#include "MyShaders.h"
#include "Texture.h"

//объявлена в main.cpp, папка data где расположенны загружаемые данные программы.
extern std::string data_folder; 

//объект, в котором инкапсулирована работа с GLWF и Open GL, создается в main.
extern OpenGL* ogl_object; 


Camera camera;
Light light;


bool texture_mode = true;
bool light_mode = true;
bool alpha_mode = false;


void SwitchMode(void* sender, const KeyEventArg &a)
{
    if (a.key == GLFW_KEY_T && a.action == GLFW_PRESS)
    {
        texture_mode = !texture_mode;
        std::cout << "Textures: " << (texture_mode ? "[ON] off " : " on [OFF]") << std::endl;
    }

    if (a.key == GLFW_KEY_L && a.action == GLFW_PRESS)
    {
        light_mode = !light_mode;
        std::cout << "Lightning: " << (light_mode ? "[ON] off " : " on [OFF]") << std::endl;
    }
    if (a.key == GLFW_KEY_A && a.action == GLFW_PRESS)
    {
        alpha_mode = !alpha_mode;
        std::cout << "Alpha blending: " << (alpha_mode ? "[ON] off " : " on [OFF]") << std::endl;
    }

}

// Умножение матриц c[M1][N1] = a[M1][N1] * b[M2][N2]
template <typename T, int M1, int N1, int M2, int N2> void MatrixMultiply(const T* a, const T* b, T* c)
{
    for (int i = 0; i < M1; ++i)
    {
        for (int j = 0; j < N2; ++j)
        {
            c[i * N2 + j] = T(0);
            for (int k = 0; k < N1; ++k)
            {
                c[i * N2 + j] += a[i * N1 + k] * b[k * N2 + j];
            }
        }
    }
}


Shader cassini_sh;
Shader phong_sh;
Shader vb_sh;
Shader simple_texture_sh;

Texture stankin_tex, vb_tex, monkey_tex;

void InitRender()
{
    // Включаем нормализацию нормалей
    // чтобы glScaled не влияли на них.
    glEnable(GL_NORMALIZE);
    
    
    // Настройка шейдеров
    cassini_sh.vert_shader_filename = data_folder + "shaders/v.vert";
    cassini_sh.frag_shader_filename = data_folder + "shaders/cassini.frag";
    cassini_sh.LoadShaderFromFile();
    cassini_sh.Compile();

    phong_sh.vert_shader_filename = data_folder + "shaders/v.vert";
    phong_sh.frag_shader_filename = data_folder + "shaders/light.frag";
    phong_sh.LoadShaderFromFile();
    phong_sh.Compile();

    vb_sh.vert_shader_filename = data_folder + "shaders/v.vert";
    vb_sh.frag_shader_filename = data_folder + "shaders/vb.frag";
    vb_sh.LoadShaderFromFile();
    vb_sh.Compile();

    simple_texture_sh.vert_shader_filename = data_folder + "shaders/v.vert";
    simple_texture_sh.frag_shader_filename = data_folder + "shaders/textureShader.frag";
    simple_texture_sh.LoadShaderFromFile();
    simple_texture_sh.Compile();


    stankin_tex.LoadTexture(data_folder + "textures/stankin.png");
    vb_tex.LoadTexture(data_folder + "textures/vb.png");
    monkey_tex.LoadTexture(data_folder + "textures/monkey.png");

    //================НАСТРОЙКА КАМЕРЫ======================
    camera.caclulateCameraPos();
    ogl_object->mouseMoveEvent().reaction(&camera, &Camera::MouseMovie);
    ogl_object->wheelEvent().reaction(&camera, &Camera::Zoom);
    ogl_object->mouseButtonEvent().reaction(&camera, &Camera::MouseStartDrag);

    //==========================

    ogl_object->keyEvent().reaction(&SwitchMode);


    //======== Настройка источника света
    light.SetPosition(1, 1, 3);
    ogl_object->mouseMoveEvent().reaction(&light, &Light::MoveLight);
    ogl_object->keyEvent().reaction(&light, &Light::KeyPressed);


    std::cout << "Controls:" << std::endl;
    std::cout << "  L - Enable/disable ligtning" << std::endl;
    std::cout << "  T - Enable/disable textures" << std::endl;
    std::cout << "  A - Enable/disable alpha blending" << std::endl;
    std::cout << "  G / G+LMB - Move light source with cursor horizontal/vertical" << std::endl;
    std::cout << "  F - Move light source to camera's position" << std::endl;
    std::cout << "  F(hold) - Move light source with camera" << std::endl;

}

double _time = 0;
float view_matrix[16];
int location = 0;


void Render(double delta_time)
{
    _time += delta_time;
    Shader::DontUseShaders();

    if (ogl_object->isKeyPressed(GLFW_KEY_F))
    {
        light.SetPosition(camera.x(), camera.y(), camera.z());
    }
    
    camera.ApplyCamera(); //в старом шаблоне SetUpCamera
    // Забираем матрицу MODELVIEW сразу после установки камеры,
    // так как в ней отсутствуют трансформации glRotate
    glGetFloatv(GL_MODELVIEW_MATRIX, view_matrix);

    ogl_object->drawAxisAndCell(); //оси и сетка
  
    light.ApplyLight(); //в старом шаблоне SetUpLight

    
    if (texture_mode)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0); // Сбрасываем текущую текстуру
    }        
    else
        glDisable(GL_TEXTURE_2D);

    if (light_mode)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);

    if (alpha_mode)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }



    //=============НАСТРОЙКА МАТЕРИАЛА==============

    // Настройка материала, все что рисуется ниже будет иметь этот материал.
    // Массивы с настройками материала
    float amb[] = { 0.2f, 0.2f, 0.1f, 1.f };
    float dif[] = { 0.4f, 0.65f, 0.5f, 1.f };
    float spec[] = { 0.9f, 0.8f, 0.3f, 1.f };
    float sh = 0.2f * 256.0f;

    // Фоновая
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    // Дифузная
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
    // Зеркальная
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    // Размер блика
    glMaterialf(GL_FRONT, GL_SHININESS, sh);

    // Сглаживание освещения
    glShadeModel(GL_SMOOTH); // закраска по Гуро
    //(GL_FLAT - плоская закраска)

    //============ РИСОВАТЬ ТУТ ==============
    
    // Квадратик станкина

    // Рисуем квадратик с овалом Кассини!

    cassini_sh.UseShader();

    location = glGetUniformLocationARB(cassini_sh.program(), "Time");
    glUniform1fARB(location, _time);
    location = glGetUniformLocationARB(cassini_sh.program(), "size");
    glUniform2fARB(location, 100, 100);

    glPushMatrix();

    glTranslated(0, -1.2, 0);

    glBegin(GL_QUADS);
    glNormal3d(0, 0, 1);
    glTexCoord2d(1, 1);
    glVertex3d(0.5, 0.5, 0);
    glTexCoord2d(1, 0);
    glVertex3d(0.5, -0.5, 0);
    glTexCoord2d(0, 0);
    glVertex3d(-0.5, -0.5, 0);
    glTexCoord2d(0, 1);
    glVertex3d(-0.5, 0.5, 0);
    glEnd();

    glPopMatrix();

    // Квадратик с освещением
    phong_sh.UseShader();


    //Тут освещение реализовано самостоятельно,
    //без использования стандартного, которое есть в классическом OpenGL
    //Для расчета освещения необходимо передать в шейдер
    //Все данные по расчету (см. формулы из теоретической части в реководстве к лр2)
    //Расчет происходит в видовой системе коордиат.
    float light_pos[4] = { light.x(), light.y(), light.z(), 1 };
    float light_pos_v[4];

    // переносим координаты света в видовые координаты
    MatrixMultiply<float, 1, 4, 4, 4>(light_pos, view_matrix, light_pos_v);

    location = glGetUniformLocationARB(phong_sh.program(), "Ia");
    glUniform3fARB(location, 1.0f, 1.0f, 1.0f);
    location = glGetUniformLocationARB(phong_sh.program(), "Id");
    glUniform3fARB(location, 1.0f, 1.0f, 1.0f);
    location = glGetUniformLocationARB(phong_sh.program(), "Is");
    glUniform3fARB(location, 1.f, 1.f, 1.f);

    location = glGetUniformLocationARB(phong_sh.program(), "ma");
    glUniform3fARB(location, 0.1f, 0.1f, 0.1f);
    location = glGetUniformLocationARB(phong_sh.program(), "md");
    glUniform3fARB(location, 0.6f, 0.6f, 0.6f);
    location = glGetUniformLocationARB(phong_sh.program(), "ms");
    glUniform4fARB(location, 0.f, 1.f, 0.f, 300);

    location = glGetUniformLocationARB(phong_sh.program(), "light_pos_v");
    glUniform3fvARB(location, 1, light_pos_v);

    glPushMatrix();

    glTranslated(0, 0, 0);

    glBegin(GL_QUADS);
    glNormal3d(0, 0, 1);
    glTexCoord2d(1, 1);
    glVertex3d(0.5, 0.5, 0);
    glTexCoord2d(1, 0);
    glVertex3d(0.5, -0.5, 0);
    glTexCoord2d(0, 0);
    glVertex3d(-0.5, -0.5, 0);
    glTexCoord2d(0, 1);
    glVertex3d(-0.5, 0.5, 0);
    glEnd();

    glPopMatrix();  

    // Квадратик без освещения, все как в лр2.

    Shader::DontUseShaders();

    glBindTexture(GL_TEXTURE_2D, 0);

    glPushMatrix();

    glTranslated(1.2, 0, 0);

    glBegin(GL_QUADS);
    glNormal3d(0, 0, 1);
    glTexCoord2d(1, 1);
    glVertex3d(0.5, 0.5, 0);
    glTexCoord2d(1, 0);
    glVertex3d(0.5, -0.5, 0);
    glTexCoord2d(0, 0);
    glVertex3d(-0.5, -0.5, 0);
    glTexCoord2d(0, 1);
    glVertex3d(-0.5, 0.5, 0);
    glEnd();

    glPopMatrix();   

    // Квадратик с ВБ
   
    vb_sh.UseShader();
    
    //Для квадратика с ВБ используется два текстурных канала: 0 и 1
    //Биндим в них текстуры,
    //А сами каналы переключаем в коде шейдера
    glActiveTexture(GL_TEXTURE0);  
    stankin_tex.Bind(); //Тут вызывется glBindTexture
    glActiveTexture(GL_TEXTURE1); 
    vb_tex.Bind();   
    //В классическом Open GL количество текстурных каналов, т.е. 
    //кол-во текстур, которое одновременно можно использовать в одном шейдере
    //сильно ограничено!

    //Передаем текущее время рендра в шейдер для анимации перехода между текстурами
    location = glGetUniformLocationARB(vb_sh.program(), "time");
    glUniform1fARB(location, _time);

    //передаем в шейдер номера этих каналов - буквально, 0 и 1
    location = glGetUniformLocationARB(vb_sh.program(), "tex_stankin");
    glUniform1iARB(location, 0);
    location = glGetUniformLocationARB(vb_sh.program(), "tex_vb");
    glUniform1iARB(location, 1);  

    glPushMatrix(); 

    glTranslated(0, 1.2, 0);
    glBegin(GL_QUADS);
    glNormal3d(0, 0, 1);
    glTexCoord2d(1, 1);
    glVertex3d(0.5, 0.5, 0);
    glTexCoord2d(1, 0);
    glVertex3d(0.5, -0.5, 0);
    glTexCoord2d(0, 0);
    glVertex3d(-0.5, -0.5, 0);
    glTexCoord2d(0, 1);
    glVertex3d(-0.5, 0.5, 0);
    glEnd();

    //Очищаем текстурные каналы, чтобы они не повлияли на дальнейшее рисование.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();  

    Shader::DontUseShaders();
    light.DrawLightGizmo(); //рисуем свет. так как он рисуется поверх всего - идет последним.

}