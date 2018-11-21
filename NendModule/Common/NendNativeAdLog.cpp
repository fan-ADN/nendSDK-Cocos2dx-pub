//
//  NendNativeAdLog.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "NendNativeAdLog.h"
#include "NendLogger.h"

using namespace nend_module;

const std::string NendNativeAdLog::NEND_NATIVE_LOG_PREFIX_DEBUG = "nend_native_module_debug";
const std::string NendNativeAdLog::NEND_NATIVE_LOG_PREFIX_INFO = "nend_native_module_info";
const std::string NendNativeAdLog::NEND_NATIVE_LOG_PREFIX_WARN = "nend_native_module_warn";
const std::string NendNativeAdLog::NEND_NATIVE_LOG_PREFIX_ERROR = "nend_native_module_error";

void NendNativeAdLog::setNativeLogLevel(NendNativeAdLogLevel logLevel)
{
    switch (logLevel) {
        case NEND_NATIVE_LOG_LEVEL_DEBUG:
            NendLogger::setLogLevel(NendLoggerLogLevel::DEBUG);
            break;
        case NEND_NATIVE_LOG_LEVEL_INFO:
            NendLogger::setLogLevel(NendLoggerLogLevel::INFO);
            break;
        case NEND_NATIVE_LOG_LEVEL_WARN:
            NendLogger::setLogLevel(NendLoggerLogLevel::WARN);
            break;
        case NEND_NATIVE_LOG_LEVEL_ERROR:
            NendLogger::setLogLevel(NendLoggerLogLevel::ERROR);
            break;
        default:
            NendLogger::setLogLevel(NendLoggerLogLevel::OFF);
            break;
    }
}

void NendNativeAdLog::logDebug(const std::string& message)
{
    NendLogger::logDebug(message);
}

void NendNativeAdLog::logInfo(const std::string& message)
{
    NendLogger::logInfo(message);
}

void NendNativeAdLog::logWarn(const std::string& message)
{
    NendLogger::logWarn(message);
}

void NendNativeAdLog::logError(const std::string& message)
{
    NendLogger::logError(message);
}
