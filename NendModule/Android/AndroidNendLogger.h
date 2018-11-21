//
//  AndroidNendLogger.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef AndroidNendLogger_h
#define AndroidNendLogger_h

enum class NendLoggerLogLevel;

class AndroidNendLogger {
public:
    static void setLogLevel(NendLoggerLogLevel logLevel);
};

#endif /* AndroidNendLogger_h */
