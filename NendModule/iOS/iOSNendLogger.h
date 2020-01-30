//
//  iOSNendLogger.h
//
//  Created by FAN Communications, Inc.
//
//

#ifndef iOSNendLogger_h
#define iOSNendLogger_h

enum class NendLoggerLogLevel;

class iOSNendLogger {
public:
    static void setLogLevel(NendLoggerLogLevel logLevel);
};

#endif /* iOSNendLogger_h */
