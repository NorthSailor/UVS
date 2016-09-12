#include "Log.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

void FV::Log(FV::LogSeverity severity, const char *messageFmt, ...)
{
    /// @note In the future, the severity can be used as a filter
    ///       for log messages.
    switch (severity) {
    case FV::INFO:
        printf("[Info ");
        break;
    case FV::WARNING:
        printf("[Warning ");
        break;
    case FV::ERROR:
        printf("[Error ");
        break;
    }

    char msg[50];
    time_t currentTime = time(NULL);
    strftime(msg, 50, "%T %Z", localtime(&currentTime));
    printf("@ %s] : ", msg);

    va_list args;
    va_start(args, messageFmt);
    vprintf(messageFmt, args);
    printf("\n");
}
