#ifndef OMS_CONSUMER_INTERCEPTOR_H
#define OMS_CONSUMER_INTERCEPTOR_H

#include "oms/Portable.h"
#include "oms/Namespace.h"
#include "oms/Uncopyable.h"
#include "oms/Message.h"

BEGIN_NAMESPACE_3(io, openmessaging, interceptor)

    /**
     * A {@code PushConsumerInterceptor} is used to intercept consume operations of push consumer.
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class ConsumerInterceptor : private Uncopyable {
    public:
        virtual ~ConsumerInterceptor() {

        }

        virtual std::string name() const = 0;

        virtual void preReceive(const MessagePtr &message, const KeyValuePtr &attributes = kv_nullptr) = 0;

        virtual void postReceive(const MessagePtr &message, const KeyValuePtr &attributes = kv_nullptr) = 0;
    };

    typedef NS::shared_ptr<ConsumerInterceptor> ConsumerInterceptorPtr;

END_NAMESPACE_3(io, openmessaging, interceptor)

#endif //OMS_CONSUMER_INTERCEPTOR_H
