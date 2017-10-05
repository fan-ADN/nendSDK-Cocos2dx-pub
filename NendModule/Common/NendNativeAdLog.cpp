//
//  NendNativeAdLog.cpp
//
//  Created by F@N Communications, Inc.
//
//

#include "NendNativeAdLog.h"

USING_NS_CC;
using namespace nend_module;

const std::string NendNativeAdLog::NEND_NATIVE_LOG_PREFIX_DEBUG = "nend_native_moudle_debug";
const std::string NendNativeAdLog::NEND_NATIVE_LOG_PREFIX_INFO = "nend_native_moudle_info";
const std::string NendNativeAdLog::NEND_NATIVE_LOG_PREFIX_WARN = "nend_native_moudle_warn";
const std::string NendNativeAdLog::NEND_NATIVE_LOG_PREFIX_ERROR = "nend_native_moudle_error";

static NendNativeAdLogLevel s_logLevel = NEND_NATIVE_LOG_LEVEL_NONE;

static void NendNativeLog(NendNativeAdLogLevel logLevel, const std::string prefix, const std::string message)
{
    if (logLevel >= s_logLevel) {
        log("%s:%s", prefix.c_str(), message.c_str());
    }
}

void NendNativeAdLog::setNativeLogLevel(NendNativeAdLogLevel logLevel)
{
    s_logLevel = logLevel;
}

void NendNativeAdLog::logDebug(const std::string message)
{
    NendNativeLog(NEND_NATIVE_LOG_LEVEL_DEBUG, NEND_NATIVE_LOG_PREFIX_DEBUG, message.c_str());
}

void NendNativeAdLog::logInfo(const std::string message)
{
    NendNativeLog(NEND_NATIVE_LOG_LEVEL_INFO, NEND_NATIVE_LOG_PREFIX_INFO, message.c_str());
}

void NendNativeAdLog::logWarn(const std::string message)
{
    NendNativeLog(NEND_NATIVE_LOG_LEVEL_WARN, NEND_NATIVE_LOG_PREFIX_WARN, message.c_str());
}

void NendNativeAdLog::logError(const std::string message)
{
    NendNativeLog(NEND_NATIVE_LOG_LEVEL_ERROR, NEND_NATIVE_LOG_PREFIX_ERROR, message.c_str());
}