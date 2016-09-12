#ifndef FV_LOG_H
#define FV_LOG_H

namespace FV {

//! Log severity categories.
//! The developer should then be able to select which types of log messages
//! should be ignored and which ones should be shown.
enum LogSeverity {
    INFO, //! Does not disrupt the execution of the program.
    WARNING, //! Slight disruption of the execution but not a fatal error.
    ERROR //! The program has encountered a critical error and needs to terminate.
};

/**
 * @brief Adds a message to the program log.
 * @param severity The severity of the message. @see LogSeverity
 * @param messageFmt Format string for the message.
 */
void Log(LogSeverity severity, const char *messageFmt, ...);

}

#endif // FV_LOG_H
