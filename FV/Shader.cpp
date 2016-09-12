#include "Shader.h"
#include "Common.h"
#include <sstream>
using namespace FV;

Shader::Shader(ShaderType type, std::string source)
{
    m_ID = glCreateShader(static_cast<GLenum>(type));
    assert(m_ID > 0);
    const GLchar *cSource = source.c_str();
    glShaderSource(m_ID, 1, &cSource, NULL);
    glCompileShader(m_ID);
    GLint success;
    glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(m_ID, 512, NULL, infoLog);
        std::stringstream error;
        error << "Shader compilation failed. - Info log:\n";
        error << infoLog;
        error << "Shader code:\n--------------\n" << source <<
                 "\n--------------";
        throw Exception(error.str());
    }
}

Shader::~Shader()
{
    glDeleteShader(m_ID);
}
