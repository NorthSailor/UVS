#ifndef FV_EXCEPTION_H
#define FV_EXCEPTION_H

#include <exception>
#include <string>

namespace FV {

/**
 * An simple exception class.
 */
class Exception
{
public:
    /**
     * Creates a new exception.
     * @param what A brief (or extended), user-friendly
     * 		  description of the problem.
     */
    Exception(std::string what);

    /**
     * The user friendly exception description.
     * @return A null-terminated const C string containing the description.
     */
    const char *what() const;

protected:
    //! The exception description.
    std::string m_what;
};

}

#endif // FV_EXCEPTION_H
