#include "MyShaders.h"
#include <fstream>
#include <sstream>

#include <stdexcept>
#include <iostream>
#include <format>

PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
PFNGLGETHANDLEARBPROC glGetHandleARB;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB;

PFNGLUNIFORM1FARBPROC glUniform1fARB;
PFNGLUNIFORM2FARBPROC glUniform2fARB;
PFNGLUNIFORM3FARBPROC glUniform3fARB;
PFNGLUNIFORM4FARBPROC glUniform4fARB;

PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
PFNGLUNIFORM4FVARBPROC glUniform4fvARB;

PFNGLUNIFORM1IARBPROC glUniform1iARB;

PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;

#if _WIN32
PFNGLACTIVETEXTUREPROC glActiveTexture;
#endif

bool init = false;
template <typename T>
T glfwGetProcAddress_wraper(const char* name)
{
    T proc_addr = reinterpret_cast<T>(glfwGetProcAddress(name));
    if (!proc_addr)
#ifdef SHADERS_THROW_EXCEPTION
        throw std::runtime_error(std::format("Failed to load the {} function", name));
#else
        std::cerr << std::format("Failed to load the {} function", name) << std::endl;
#endif
    return proc_addr;
}

void initShadersFunctions()
{
    if (init) return;

    // Наполняем делегаты адресами на функции
    glCreateShaderObjectARB = glfwGetProcAddress_wraper<PFNGLCREATESHADEROBJECTARBPROC>("glCreateShaderObjectARB");
    glCreateProgramObjectARB = glfwGetProcAddress_wraper<PFNGLCREATEPROGRAMOBJECTARBPROC>("glCreateProgramObjectARB");
    glAttachObjectARB = glfwGetProcAddress_wraper<PFNGLATTACHOBJECTARBPROC>("glAttachObjectARB");
    glDetachObjectARB = glfwGetProcAddress_wraper<PFNGLDETACHOBJECTARBPROC>("glDetachObjectARB");
    glDeleteObjectARB = glfwGetProcAddress_wraper<PFNGLDELETEOBJECTARBPROC>("glDeleteObjectARB");
    glShaderSourceARB = glfwGetProcAddress_wraper<PFNGLSHADERSOURCEARBPROC>("glShaderSourceARB");
    glCompileShaderARB = glfwGetProcAddress_wraper<PFNGLCOMPILESHADERARBPROC>("glCompileShaderARB");
    glLinkProgramARB = glfwGetProcAddress_wraper<PFNGLLINKPROGRAMARBPROC>("glLinkProgramARB");
    glValidateProgramARB = glfwGetProcAddress_wraper<PFNGLVALIDATEPROGRAMARBPROC>("glValidateProgramARB");
    glUseProgramObjectARB = glfwGetProcAddress_wraper<PFNGLUSEPROGRAMOBJECTARBPROC>("glUseProgramObjectARB");
    glGetObjectParameterivARB = glfwGetProcAddress_wraper<PFNGLGETOBJECTPARAMETERIVARBPROC>("glGetObjectParameterivARB");
    glGetInfoLogARB = glfwGetProcAddress_wraper<PFNGLGETINFOLOGARBPROC>("glGetInfoLogARB");

    glGetUniformLocationARB = glfwGetProcAddress_wraper<PFNGLGETUNIFORMLOCATIONARBPROC>("glGetUniformLocationARB");

    // Функции передачи float-ов в шейдеры
    glUniform1fARB = glfwGetProcAddress_wraper<PFNGLUNIFORM1FARBPROC>("glUniform1fARB");
    glUniform2fARB = glfwGetProcAddress_wraper<PFNGLUNIFORM2FARBPROC>("glUniform2fARB");
    glUniform3fARB = glfwGetProcAddress_wraper<PFNGLUNIFORM3FARBPROC>("glUniform3fARB");
    glUniform4fARB = glfwGetProcAddress_wraper<PFNGLUNIFORM4FARBPROC>("glUniform4fARB");

    glUniform1fvARB = glfwGetProcAddress_wraper<PFNGLUNIFORM4FVARBPROC>("glUniform1fvARB");
    glUniform2fvARB = glfwGetProcAddress_wraper<PFNGLUNIFORM4FVARBPROC>("glUniform2fvARB");
    glUniform3fvARB = glfwGetProcAddress_wraper<PFNGLUNIFORM4FVARBPROC>("glUniform3fvARB");
    glUniform4fvARB = glfwGetProcAddress_wraper<PFNGLUNIFORM4FVARBPROC>("glUniform4fvARB");

    // Функуции передачи int-ов c itqlthfvb
    glUniform1iARB = glfwGetProcAddress_wraper<PFNGLUNIFORM1IARBPROC>("glUniform1iARB");

#if _WIN32
    glActiveTexture = glfwGetProcAddress_wraper<PFNGLACTIVETEXTUREPROC>("glActiveTexture");
#endif

    init = true;
}

Shader::~Shader()
{    
    if (m_shader_program != 0)
        glDeleteObjectARB(m_shader_program);
    if (m_vertex_shader != 0)
        glDeleteObjectARB(m_vertex_shader);
    if (m_frag_shader != 0)
        glDeleteObjectARB(m_frag_shader);
}

void Shader::LoadShaderFromFile()
{
    if (!init)
        initShadersFunctions();


    m_shader_program = glCreateProgramObjectARB();
    m_vertex_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    m_frag_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    std::ifstream stream_f(frag_shader_filename);
    std::ifstream stream_v(vert_shader_filename);

    try {
        std::stringstream ss;
        ss << stream_v.rdbuf();
        vert_shader_code = ss.str();
    }
    catch (std::exception &ex)
    {
        std::cerr << std::format("Can't loaf vert. shader file {}", vert_shader_filename) << std::endl;
        std::cerr << ex.what() << std::endl;
    }
    
    try {
        std::stringstream ss;
        ss << stream_f.rdbuf();
        frag_shader_code = ss.str();
    }
    catch (std::exception& ex)
    {
        std::cerr << std::format("Can't loaf frag. shader file {}", vert_shader_filename) << std::endl;
        std::cerr << ex.what() << std::endl;
    }

    auto frag_code = frag_shader_code.c_str();
    auto vert_code = vert_shader_code.c_str();

    int frag_length = frag_shader_code.length();
    int vert_length = vert_shader_code.length();

    glShaderSourceARB(m_frag_shader, 1, &frag_code, &frag_length);
    glShaderSourceARB(m_vertex_shader, 1, &vert_code, &vert_length);

}

void Shader::Compile()
{
    // Компиляция вершинного шейдера
    glCompileShaderARB(m_vertex_shader);
    {
        int compiled = 0;
        int length = 0;
        int laux = 0;
        // Получение сообщения о ошибках компиляции
        glGetObjectParameterivARB(m_vertex_shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
        glGetObjectParameterivARB(m_vertex_shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);

        if (!compiled && length>0)
        {
            char* log = new char[length];
            glGetInfoLogARB(m_vertex_shader, length, &laux, log);

            std::cerr << "Vertex Shader compile error!" << std::endl;
            std::cerr << log << std::endl;

            delete log;
        }
        
    }

    // Компиляция фрагментного шейдера
    glCompileShaderARB(m_frag_shader);
    {
        // Получение сообщения о ошибках компиляции
        int compiled = 0;
        int length = 0;
        int laux = 0;
        glGetObjectParameterivARB(m_frag_shader, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
        glGetObjectParameterivARB(m_frag_shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);

        if (!compiled && length>0)
        {
            char* log = new char[length];
            glGetInfoLogARB(m_frag_shader, length, &laux, log);

            std::cerr << "Fragment Shader compile error!" << std::endl;
            std::cerr << log << std::endl;

            delete log;
        }

        
    }

    // Привязывем шейдер к программному объекту
    glAttachObjectARB(m_shader_program, m_frag_shader);
    glAttachObjectARB(m_shader_program, m_vertex_shader);

    // Линкуем
    glLinkProgramARB(m_shader_program);
    {
        int result = 0;
        int length = 0;
        int laux = 0;
        glGetObjectParameterivARB(m_shader_program, GL_OBJECT_LINK_STATUS_ARB, &result);
        glGetObjectParameterivARB(m_shader_program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);

        if (!result && length>0)
        {
            char* log = new char[length];
            glGetInfoLogARB(m_shader_program, length, &laux, log);

            std::cerr << "Shader programm link error!" << std::endl;
            std::cerr << log << std::endl;

            delete log;
        }
        
    }
}

void Shader::UseShader()
{
    glUseProgramObjectARB(m_shader_program);
}

void Shader::DontUseShaders()
{
    glUseProgramObjectARB(0);
}
