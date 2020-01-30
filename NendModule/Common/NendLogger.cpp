//
//  NendLogger.cpp
//
//  Created by FAN Communications, Inc.
//
//

#include "NendLogger.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iOSNendLogger.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "AndroidNendLogger.h"
#endif

const std::string NendLogger::NEND_LOG_PREFIX_DEBUG = "nend_module_debug";
const std::string NendLogger::NEND_LOG_PREFIX_INFO = "nend_module_info";
const std::string NendLogger::NEND_LOG_PREFIX_WARN = "nend_module_warn";
const std::string NendLogger::NEND_LOG_PREFIX_ERROR = "nend_module_error";

static NendLoggerLogLevel s_logLevel = NendLoggerLogLevel::OFF;

static void NendLog(NendLoggerLogLevel logLevel, const std::string& prefix, const std::string& message)
{
    if (logLevel >= s_logLevel) {
        cocos2d::log("%s:%s", prefix.c_str(), message.c_str());
    }
}

void NendLogger::setLogLevel (NendLoggerLogLevel logLevel) {
    s_logLevel = logLevel;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    iOSNendLogger::setLogLevel(logLevel);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    AndroidNendLogger::setLogLevel(logLevel);
#endif

}

void NendLogger::logDebug(const std::string& message) {
    NendLog(NendLoggerLogLevel::DEBUG, NEND_LOG_PREFIX_DEBUG, message);
}

void NendLogger::logInfo(const std::string& message) {
    NendLog(NendLoggerLogLevel::INFO, NEND_LOG_PREFIX_INFO, message);
}

void NendLogger::logWarn(const std::string& message) {
    NendLog(NendLoggerLogLevel::WARN, NEND_LOG_PREFIX_WARN, message);
}

void NendLogger::logError(const std::string& message) {
    NendLog(NendLoggerLogLevel::ERROR, NEND_LOG_PREFIX_ERROR, message);
}
