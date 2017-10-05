//
//  NendNativeAdLog.h
//
//  Created by F@N Communications, Inc.
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
    } NendNativeAdLogLevel;
    
    class NendNativeAdLog
    {
    public:
        static const std::string NEND_NATIVE_LOG_PREFIX_DEBUG;
        static const std::string NEND_NATIVE_LOG_PREFIX_INFO;
        static const std::string NEND_NATIVE_LOG_PREFIX_WARN;
        static const std::string NEND_NATIVE_LOG_PREFIX_ERROR;
        
        static void setNativeLogLevel(NendNativeAdLogLevel logLevel);
        static void logDebug(std::string message);
        static void logInfo(std::string message);
        static void logWarn(std::string message);
        static void logError(std::string message);
    };
}
#endif /* NendNativeAdLog_h */
