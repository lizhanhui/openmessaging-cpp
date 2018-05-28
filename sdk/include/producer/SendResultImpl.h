#ifndef OMS_SEND_RESULT_IMPL_H
#define OMS_SEND_RESULT_IMPL_H

#include "oms/Namespace.h"
#include "core.h"
#include "oms/producer/SendResult.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    class SendResultImpl : public virtual SendResult {
    public:
        SendResultImpl(jobject proxy);

        virtual ~SendResultImpl();

        virtual std::string messageId();

    private:
        jobject _proxy;
        jclass classSendResult;
        jmethodID midMessageId;
    };

    typedef NS::shared_ptr<SendResultImpl> SendResultImplPtr;

END_NAMESPACE_3(io, openmessaging, producer)

#endif //OMS_SEND_RESULT_IMPL_H
