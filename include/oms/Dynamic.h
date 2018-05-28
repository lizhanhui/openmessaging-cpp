#ifndef OMS_OMS_H
#define OMS_OMS_H

#include <dlfcn.h>

#include <cstring>
#include <iostream>

#include "Portable.h"
#include "Namespace.h"
#include "Uncopyable.h"
#include "OMSException.h"
#include "KeyValue.h"

// forward declaration
BEGIN_NAMESPACE_2(io, openmessaging)
    class MessagingAccessPoint;
    typedef NS::shared_ptr<MessagingAccessPoint> MessagingAccessPointPtr;
END_NAMESPACE_2(io, openmessaging)

#ifdef __cplusplus
    extern "C" {
#endif

        static void *handle = NULL;

        static void load_library(const std::string &url) {
            std::string::size_type begin = url.find(":");
            std::string::size_type end = url.find(":", begin + 1);
            std::string driver = url.substr(begin + 1, end - begin - 1);

        #ifdef __APPLE__
            std::string extension = ".dylib";
        #endif
        #ifdef __linux__
            std::string extension = ".so";
        #endif
            std::string shared_library_name = "liboms_" + driver + extension;
            // clean previous error
            dlerror();
            handle = dlopen(shared_library_name.c_str(), RTLD_LAZY);
            if (!handle) {
                char * error = dlerror();
                std::cerr << "Failed to load shared library: " << shared_library_name
                          << "; Error Message:" << error << std::endl;
                std::string default_library = "liboms_jni";
                shared_library_name = default_library + extension;
                handle = dlopen(shared_library_name.c_str(), RTLD_LAZY);

                if (!handle) {
                    std::string msg = "Failed to dlopen shared library: ";
                    msg += shared_library_name;
                    msg += ". Reason: ";
                    msg += dlerror();
                    std::cout << msg << std::endl;
                    throw io::openmessaging::OMSException(msg);
                }
            }
        }

        static io::openmessaging::KeyValuePtr newKeyValue() {
            if (NULL == handle) {
                throw io::openmessaging::OMSException("Please call load_library first");
            }

            typedef io::openmessaging::KeyValuePtr (*Fn)();
            Fn fn;
            fn = (Fn)dlsym(handle, "newKeyValueImpl");
            return fn();
        }

        static io::openmessaging::MessagingAccessPointPtr
        getMessagingAccessPoint(const std::string &url,
                                const io::openmessaging::KeyValuePtr &props = io::openmessaging::KeyValuePtr()) {

            typedef io::openmessaging::MessagingAccessPointPtr (*Fn)(const std::string&, const io::openmessaging::KeyValuePtr &);

            if (NULL == handle) {
                load_library(url);
            }

            Fn fn;

            fn = (Fn)dlsym(handle, "getMessagingAccessPointImpl");

            return fn(url, props);
        }

        io::openmessaging::MessagingAccessPointPtr
        getMessagingAccessPointImpl(const std::string &url,
                                    const io::openmessaging::KeyValuePtr &props = io::openmessaging::KeyValuePtr());


        io::openmessaging::KeyValuePtr newKeyValueImpl();

#ifdef __cplusplus
    }
#endif



#endif // OMS_OMS_H
