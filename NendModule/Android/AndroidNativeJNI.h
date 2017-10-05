//
//  AndroidNativeJNI.h
//
//  Created by F@N Communications, Inc.
//
//

#ifndef AndroidNativeJNI_h
#define AndroidNativeJNI_h

#include <jni.h>

USING_NS_CC;

namespace nend_module
{
    namespace internal
    {
        class AndroidNativeJNI
        {
        protected:
            void callJNI(const std::function<void (JNIEnv* env)> &callback);
        public:
            AndroidNativeJNI();
            virtual ~AndroidNativeJNI();
        };
    }
}
#endif /* AndroidNativeJNI_h */
