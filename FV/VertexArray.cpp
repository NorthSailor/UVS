#include "VertexArray.h"
using namespace FV;

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::Bind()
{
    glBindVertexArray(m_ID);
}
