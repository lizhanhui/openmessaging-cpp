#include <boost/unordered_map.hpp>

#include "core.h"
#include "MessagingAccessPointImpl.h"
#include "KeyValueImpl.h"
#include "producer/ProducerImpl.h"
#include "consumer/PushConsumerImpl.h"
#include "consumer/PullConsumerImpl.h"

using namespace io::openmessaging;
using namespace io::openmessaging::producer;
using namespace io::openmessaging::consumer;
using namespace io::openmessaging::observer;

BEGIN_NAMESPACE_2(io, openmessaging)
<<<<<<< HEAD
    extern boost::mutex service_mtx;
    extern std::vector<NS::shared_ptr<ServiceLifecycle> > services;
    extern volatile boost::atomic_bool stopped;

    void signal_handler(int sig_num) {
        if (SIGINT == sig_num) {
            LOG_INFO << "Received SIGINT signal";
        }

        if (SIGTERM == sig_num) {
            LOG_INFO << "Received SIGTERM signal";
        }

        stopped.exchange(true);

        {
            boost::lock_guard<boost::mutex> lk(service_mtx);
            LOG_INFO << services.size() << " services to shutdown";
            // Avoid using of auto to make the code C++98 compatible
            for(std::vector<NS::shared_ptr<ServiceLifecycle> >::iterator it = services.begin();
                it < services.end(); it++) {
                NS::shared_ptr<ServiceLifecycle> item = *it;
                item->shutdown();
                services.erase(it);
                LOG_INFO << "Shutdown [OK]";
            }
        }

        ShutdownVM();
    }


=======
    extern boost::unordered_map<std::string, MessagingAccessPointPtr> access_points;
    extern boost::recursive_mutex access_point_mtx;
>>>>>>> a3519cb... shutdown gracefully
END_NAMESPACE_2(io, openmessaging)

MessagingAccessPointImpl::MessagingAccessPointImpl(const std::string &url,
                                                   const KeyValuePtr &props,
                                                   jobject proxy) :
        _url(url), _properties(props), ServiceLifecycleImpl(proxy) {

    CurrentEnv current;
    const char *klassMessagingAccessPoint = "io/openmessaging/MessagingAccessPoint";
    classMessagingAccessPoint = current.findClass(klassMessagingAccessPoint);

    midImplVersion = current.getMethodId(classMessagingAccessPoint, "implVersion",
                                         buildSignature(Types::String_, 0));

    midCreateProducer = current.getMethodId(classMessagingAccessPoint, "createProducer",
                                            buildSignature(Types::Producer_, 0));

    midCreateProducer2 = current.getMethodId(classMessagingAccessPoint, "createProducer",
                                             buildSignature(Types::Producer_, 1, Types::KeyValue_));

    midCreatePushConsumer = current.getMethodId(classMessagingAccessPoint, "createPushConsumer",
                                      buildSignature(Types::PushConsumer_, 0));

    midCreatePushConsumer2 = current.getMethodId(classMessagingAccessPoint, "createPushConsumer",
                                       buildSignature(Types::PushConsumer_, 1, Types::KeyValue_));

    midCreatePullConsumer = current.getMethodId(classMessagingAccessPoint, "createPullConsumer",
                                      buildSignature(Types::PullConsumer_, 0));

    midCreatePullConsumer2 = current.getMethodId(classMessagingAccessPoint, "createPullConsumer",
                                       buildSignature(Types::PullConsumer_, 1, Types::KeyValue_));
}

KeyValuePtr MessagingAccessPointImpl::attributes() {
    return _properties;
}

std::string MessagingAccessPointImpl::implVersion() {
    CurrentEnv current;
    jstring version =
            reinterpret_cast<jstring>(current.callObjectMethod(_proxy, midImplVersion));
    const char *pVersion = current.env->GetStringUTFChars(version, NULL);
    std::string result = pVersion;
    current.env->ReleaseStringUTFChars(version, pVersion);
    return result;
}

ProducerPtr MessagingAccessPointImpl::createProducer(const KeyValuePtr &props) {
    CurrentEnv current;
    jobject producerLocal;
    if (props) {
        jobject kv = (dynamic_cast<KeyValueImpl*>(props.get()))->getProxy();
        producerLocal = current.callObjectMethod(_proxy, midCreateProducer2, kv);
    } else {
        producerLocal = current.callObjectMethod(_proxy, midCreateProducer);
    }

    jobject producer = current.newGlobalRef(producerLocal);

    NS::shared_ptr<Producer> ret = NS::make_shared<ProducerImpl>(producer, props);
    {
        boost::lock_guard<boost::mutex> lk(service_mtx);
        services.push_back(ret);
    }

    return ret;
}

consumer::PushConsumerPtr MessagingAccessPointImpl::createPushConsumer(const KeyValuePtr &props) {
    CurrentEnv current;
    jobject pushConsumerLocal;
    if (props) {
        jobject kv = (dynamic_cast<KeyValueImpl*>(props.get()))->getProxy();
        pushConsumerLocal = current.callObjectMethod(_proxy, midCreatePushConsumer2, kv);
    } else {
        pushConsumerLocal = current.callObjectMethod(_proxy, midCreatePushConsumer);
    }

    jobject pushConsumer = current.newGlobalRef(pushConsumerLocal);
    NS::shared_ptr<PushConsumer> ret = NS::make_shared<PushConsumerImpl>(pushConsumer);
    {
        boost::lock_guard<boost::mutex> lk(service_mtx);
        services.push_back(ret);
    }

    return ret;
}

consumer::PullConsumerPtr MessagingAccessPointImpl::createPullConsumer(const KeyValuePtr &props) {
    CurrentEnv current;
    jobject pullConsumerLocal;
    if (props) {
        jobject kv = (dynamic_cast<KeyValueImpl*>(props.get()))->getProxy();
        pullConsumerLocal = current.callObjectMethod(_proxy, midCreatePullConsumer2, kv);
    } else {
        pullConsumerLocal = current.callObjectMethod(_proxy, midCreatePullConsumer);
    }

    jobject pullConsumer = current.newGlobalRef(pullConsumerLocal);
    NS::shared_ptr<PullConsumer> ret = NS::make_shared<PullConsumerImpl>(pullConsumer);
    {
        boost::lock_guard<boost::mutex> lk(service_mtx);
        services.push_back(ret);
    }

    return ret;
}

consumer::StreamingConsumerPtr MessagingAccessPointImpl::createStreamingConsumer(const KeyValuePtr &props) {
    throw OMSException("Not Implemented");
}

ResourceManagerPtr MessagingAccessPointImpl::resourceManager() {
    throw OMSException("Not Implemented");
}

jobject MessagingAccessPointImpl::getProxy() {
    return _proxy;
}

<<<<<<< HEAD
void MessagingAccessPointImpl::startup() {
    ServiceLifecycleImpl::startup();
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
=======
void MessagingAccessPointImpl::shutdown() {

    {
        boost::lock_guard<boost::recursive_mutex> lk(access_point_mtx);
        boost::unordered_map<std::string, MessagingAccessPointPtr>::iterator found =
                access_points.find(_url);
        if (found != access_points.end()) {
            access_points.erase(_url);
            LOG_DEBUG << "Remove access point[" << _url << "] Done";
        } else {
            LOG_WARNING << "Access point not found for url: " << _url;
        }
    }

    ServiceLifecycleImpl::shutdown();
>>>>>>> a3519cb... shutdown gracefully
}

MessagingAccessPointImpl::~MessagingAccessPointImpl() {
    CurrentEnv context;
    context.deleteRef(classMessagingAccessPoint);
}
