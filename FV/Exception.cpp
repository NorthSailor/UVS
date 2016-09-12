#include "Exception.h"
using namespace FV;

Exception::Exception(std::string what) :
    m_what(what)
{
}

const char *Exception::what() const {
    return m_what.c_str();
}

