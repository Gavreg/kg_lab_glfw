#pragma once

#include "OpenGL.h"

#ifdef _WIN32
#include "glext.h" 
#endif
#include <string>

extern PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
extern PFNGLGETHANDLEARBPROC glGetHandleARB;
extern PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
extern PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;

extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;

extern PFNGLUNIFORM1FARBPROC glUniform1fARB;
extern PFNGLUNIFORM2FARBPROC glUniform2fARB;
extern PFNGLUNIFORM3FARBPROC glUniform3fARB;
extern PFNGLUNIFORM4FARBPROC glUniform4fARB;

extern PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC glUniform4fvARB;

extern PFNGLUNIFORM1IARBPROC glUniform1iARB;

extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;

#if _WIN32
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
#endif

void initShadersFunctions();
int loadShader(const char* filename, char** shaderSrc, int* programLength);

// Вызываем эту функцию перед рендером для настройки шейдеров
void initShadersFunctions();

class Shader
{
  private:
    
      GLhandleARB m_shader_program = 0;
      GLhandleARB m_vertex_shader = 0;
      GLhandleARB m_frag_shader = 0;
 
  public:

    std::string vert_shader_filename;
    std::string frag_shader_filename;

    std::string frag_shader_code;
    std::string vert_shader_code;

    const GLhandleARB& program()
    {
        return m_shader_program;
    }

    Shader() = default;

    ~Shader();

    void LoadShaderFromFile();

    void Compile();

    void UseShader();

    static void DontUseShaders();
};

// Загружает код шейдера из файла
//************************************
// Parameter: char * filename      Имя файла
// Parameter: char * * shaderSrc   Указатель на массив символов, в который загрузится код шейдера. После того, как
//   станет ненужным, надо освободить память!
// Parameter: int * programLength  Указатель на длину массива символов.
//************************************
int loadShader(const char* filename, char** shaderSrc, int* programLength);
