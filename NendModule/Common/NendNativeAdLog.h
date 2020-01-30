//
//  NendNativeAdLog.h
//
//  Created by FAN Communications, Inc.
//
//

#ifndef NendNativeAdLog_h
#define NendNativeAdLog_h

#include "cocos2d.h"

namespace nend_module
{
    typedef enum {
        NEND_NATIVE_LOG_LEVEL_DEBUG = 1,
        NEND_NATIVE_LOG_LEVEL_INFO = 2,
        NEND_NATIVE_LOG_LEVEL_WARN = 3,
        NEND_NATIVE_LOG_LEVEL_ERROR = 4,
        NEND_NATIVE_LOG_LEVEL_NONE = INT_MAX
    } NendNativeAdLogLevel __attribute__((deprecated("will be removed.")));

    class NendNativeAdLog
    {
    public:
        static const std::string NEND_NATIVE_LOG_PREFIX_DEBUG __attribute__((deprecated("will be removed.")));
        static const std::string NEND_NATIVE_LOG_PREFIX_INFO __attribute__((deprecated("will be removed.")));
        static const std::string NEND_NATIVE_LOG_PREFIX_WARN __attribute__((deprecated("will be removed.")));
        static const std::string NEND_NATIVE_LOG_PREFIX_ERROR __attribute__((deprecated("will be removed.")));

        static void setNativeLogLevel(NendNativeAdLogLevel logLevel) __attribute__((deprecated("Now deprecated: please use `static void setLogLevel(NendLoggerLogLevel logLevel);` of NendLogger.")));
        static void logDebug(const std::string& message)__attribute__((deprecated("Now deprecated: please use `static void logDebug(std::string message);` of NendLogger.")));
        static void logInfo(const std::string& message)__attribute__((deprecated("Now deprecated: please use `static void logInfo(std::string message);` of NendLogger.")));
        static void logWarn(const std::string& message)__attribute__((deprecated("Now deprecated: please use `static void logWarn(std::string message);` of NendLogger.")));
        static void logError(const std::string& message)__attribute__((deprecated("Now deprecated: please use `static void logError(std::string message);` of NendLogger.")));
    };
}
#endif /* NendNativeAdLog_h */
