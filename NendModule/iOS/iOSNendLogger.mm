//
//  iOSNendLogger.m
//
//  Created by F@N Communications, Inc.
//
//

#include "iOSNendLogger.h"
#include "NendLogger.h"
#import <NendAd/NADLogger.h>

void iOSNendLogger::setLogLevel (NendLoggerLogLevel logLevel) {
    NADLogLevel setLogLevelValue;
    switch (logLevel) {
        case NendLoggerLogLevel::DEBUG:
            setLogLevelValue = NADLogLevelDebug;
            break;
        case NendLoggerLogLevel::INFO:
            setLogLevelValue = NADLogLevelInfo;
            break;
        case NendLoggerLogLevel::WARN:
            setLogLevelValue = NADLogLevelWarn;
            break;
        case NendLoggerLogLevel::ERROR:
            setLogLevelValue = NADLogLevelError;
            break;
        default:
            setLogLevelValue = NADLogLevelOff;
            break;
    }
    [NADLogger setLogLevel:setLogLevelValue];
}
