//
//  NendLogger.h
//
//  Created by FAN Communications, Inc.
//
//

#ifndef NendLogger_h
#define NendLogger_h

#include "cocos2d.h"

enum class NendLoggerLogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    OFF
};

class NendLogger {
private:
    static const std::string NEND_LOG_PREFIX_DEBUG;
    static const std::string NEND_LOG_PREFIX_INFO;
    static const std::string NEND_LOG_PREFIX_WARN;
    static const std::string NEND_LOG_PREFIX_ERROR;
public:
    static void setLogLevel(NendLoggerLogLevel logLevel);
    static void logDebug(const std::string& message);
    static void logInfo(const std::string& message);
    static void logWarn(const std::string& message);
    static void logError(const std::string& message);
};

#endif /* NendLogger_h */
