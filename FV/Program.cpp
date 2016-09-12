#include "Program.h"
#include "Common.h"
#include <assert.h>
#include <fstream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"
using namespace FV;

std::string Program::s_shaderPath = "Resources/Shaders/";

Program::Program(std::string vertexSource, std::string fragmentSource)
{
    auto vertexShader = std::make_shared<Shader>(Shader::VERTEX, vertexSource);
    auto fragmentShader = std::make_shared<Shader>(Shader::FRAGMENT,
                                                   fragmentSource);

    m_ID = glCreateProgram();
    assert(m_ID > 0);

    glAttachShader(m_ID, vertexShader->GetID());
    glAttachShader(m_ID, fragmentShader->GetID());
    glLinkProgram(m_ID);

    GLint success;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::string text = "Program linking failed - Info Log:\n";
        text.append(infoLog);
        throw Exception(text);
    }
}

Program::~Program()
{
    glDeleteProgram(m_ID);
}

void Program::Use()
{
    glUseProgram(m_ID);
}

void Program::Release()
{
    glUseProgram(0);
}

Program::Uniform::Uniform(GLint id) :
    m_ID(id)
{
    assert(m_ID > -1);
}

Program::Uniform::Uniform() : m_ID(-1)
{
    // Default constructor, not to be used.
}

Program::Uniform Program::GetUniform(std::string name) const
{
    return Uniform(glGetUniformLocation(m_ID, name.c_str()));
}

/// @note For OpenGL 4 we should transition to glProgramUniform{...}()
void Program::Uniform::Set(float v) const { glUniform1f(m_ID, v); }
void Program::Uniform::Set(double v) const { glUniform1d(m_ID, v); }
void Program::Uniform::Set(int v) const { glUniform1i(m_ID, v); }
void Program::Uniform::Set(glm::vec2 v) const { glUniform2f(m_ID, v.x, v.y); }
void Program::Uniform::Set(glm::dvec2 v) const { glUniform2d(m_ID, v.x, v.y); }
void Program::Uniform::Set(glm::ivec2 v) const { glUniform2i(m_ID, v.x, v.y); }

void Program::Uniform::Set(glm::vec3 v) const {
    glUniform3f(m_ID, v.x, v.y, v.z);
}

void Program::Uniform::Set(glm::dvec3 v) const {
    glUniform3d(m_ID, v.x, v.y, v.z);
}

void Program::Uniform::Set(glm::ivec3 v) const {
    glUniform3i(m_ID, v.x, v.y, v.z);
}

void Program::Uniform::Set(glm::vec4 v) const {
    glUniform4f(m_ID, v.x, v.y, v.z, v.w);
}

void Program::Uniform::Set(glm::dvec4 v) const {
    glUniform4d(m_ID, v.x, v.y, v.z, v.w);
}

void Program::Uniform::Set(glm::ivec4 v) const {
    glUniform4i(m_ID, v.x, v.y, v.z, v.w);
}

void Program::Uniform::Set(glm::mat2 m) const {
    glUniformMatrix2fv(m_ID, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::Uniform::Set(glm::dmat2 m) const {
    glUniformMatrix2dv(m_ID, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::Uniform::Set(glm::mat3 m) const {
    glUniformMatrix3fv(m_ID, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::Uniform::Set(glm::dmat3 m) const {
    glUniformMatrix3dv(m_ID, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::Uniform::Set(glm::mat4 m) const {
    glUniformMatrix4fv(m_ID, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::Uniform::Set(glm::dmat4 m) const {
    glUniformMatrix4dv(m_ID, 1, GL_FALSE, glm::value_ptr(m));
}

void Program::Uniform::Set(std::shared_ptr<Texture> texture, int unit) const
{
    texture->Bind(unit);
    glUniform1i(m_ID, unit);
}
