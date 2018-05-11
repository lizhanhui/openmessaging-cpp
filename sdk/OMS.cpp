#include <boost/unordered_map.hpp>
#include <csignal>

#include "OMS.h"
#include "core.h"
#include "KeyValueImpl.h"
#include "MessagingAccessPointImpl.h"

using namespace io::openmessaging;

<<<<<<< HEAD
MessagingAccessPoint*
=======
BEGIN_NAMESPACE_2(io, openmessaging)
    boost::unordered_map<std::string, MessagingAccessPointPtr> access_points;
    boost::recursive_mutex access_point_mtx;
    boost::atomic_bool check_signal_handler_registered(false);
END_NAMESPACE_2(io, openmessaging)

void signal_handler(int signum) {
    bool shutdown_access_points = false;

    if (SIGTERM == signum) {
        LOG_INFO << "Caught SIGTERM signal.";
        shutdown_access_points = true;
    }

    if (SIGINT == signum) {
        LOG_INFO << "Caught SIGINT signal.";
        shutdown_access_points = true;
    }

    if (shutdown_access_points) {
        LOG_INFO << "About to shutdown global access point";

        {
            boost::lock_guard<boost::recursive_mutex> lk(access_point_mtx);
            LOG_DEBUG << access_points.size() << " access points to shutdown";
            for (boost::unordered_map<std::string, MessagingAccessPointPtr>::const_iterator it = access_points.cbegin();
                    it != access_points.cend(); it++) {
                it->second->shutdown();
                access_points.erase(it);
            }
        }
//        Shutdown();
    }
}

void register_signal_handler() {
    LOG_DEBUG << "Begin to register signal handler";
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    LOG_DEBUG << "Signal handle registration [Done]";
}

MessagingAccessPointPtr
>>>>>>> a3519cb... shutdown gracefully
getMessagingAccessPointImpl(const std::string &url, const NS::shared_ptr<KeyValue> &props) {

    Initialize();
    bool to_register = false;
    if (check_signal_handler_registered.compare_exchange_strong(to_register, true)) {
        register_signal_handler();
    }

    {
        boost::lock_guard<boost::recursive_mutex> lk(access_point_mtx);
        boost::unordered_map<std::string, MessagingAccessPointPtr>::iterator found
                = access_points.find(url);
        if (found != access_points.end()) {
            return found->second;
        }

        CurrentEnv current;
        const char* klassOMS = "io/openmessaging/OMS";
        jclass classOMS = current.findClass(klassOMS);
        jmethodID midGetMessagingAccessPoint;
        bool useKV = false;
        if (props) {
            useKV = true;
            std::string sig = buildSignature(Types::MessagingAccessPoint_, 2, Types::String_, Types::KeyValue_);
            midGetMessagingAccessPoint = current.getMethodId(classOMS, "getMessagingAccessPoint", sig.c_str(), true);
        } else {
            std::string sig = buildSignature(Types::MessagingAccessPoint_, 1, Types::String_);
            midGetMessagingAccessPoint = current.getMethodId(classOMS, "getMessagingAccessPoint", sig.c_str(), true);
        }

        jstring accessUrl = current.newStringUTF(url.c_str());

<<<<<<< HEAD
    current.deleteRef(accessUrl);
    current.deleteRef(classOMS);
    jobject globalRef = current.newGlobalRef(messagingAccessPoint);
    return new MessagingAccessPointImpl(url, props, globalRef);
=======
        jobject messagingAccessPoint;
        if (useKV) {
            KeyValue* ptr = props.get();
            jobject prop = (dynamic_cast<KeyValueImpl*>(ptr))->getProxy();
            messagingAccessPoint = current.callStaticObjectMethod(classOMS,
                                                                  midGetMessagingAccessPoint,
                                                                  accessUrl,
                                                                  prop);
        } else {
            messagingAccessPoint = current.callStaticObjectMethod(classOMS,
                                                                  midGetMessagingAccessPoint,
                                                                  accessUrl);
        }

        current.deleteRef(accessUrl);
        current.deleteRef(classOMS);
        jobject globalRef = current.newGlobalRef(messagingAccessPoint);
        MessagingAccessPointPtr ptr = NS::make_shared<MessagingAccessPointImpl>(url, props, globalRef);
        access_points[url] = ptr;
        return ptr;
    }
>>>>>>> a3519cb... shutdown gracefully
}

KeyValue* newKeyValueImpl() {
    Initialize();
    CurrentEnv current;
    const char *klassDefaultKeyValue = "io/openmessaging/internal/DefaultKeyValue";
    jclass classDefaultKeyValue = current.findClass(klassDefaultKeyValue);
    jmethodID ctor = current.getMethodId(classDefaultKeyValue, "<init>", "()V");
    jobject objectDefaultKeyValue = current.newObject(classDefaultKeyValue, ctor);
    return new KeyValueImpl(objectDefaultKeyValue);
}