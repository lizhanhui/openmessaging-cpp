#ifndef OMS_BYTE_MESSAGE_IMPL_H
#define OMS_BYTE_MESSAGE_IMPL_H

#include "oms/ByteMessage.h"
#include "core.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    class ByteMessageImpl : public ByteMessage {
    public:
        ByteMessageImpl(jobject proxy);

        virtual ~ByteMessageImpl();

        virtual NS::shared_ptr<KeyValue> sysHeaders();

        virtual NS::shared_ptr<KeyValue> userHeaders();

        virtual MessageBody getBody(MessageType type);

        virtual ByteMessage& setBody(const MessageBody &body);

        virtual ByteMessageImpl& putSysHeaders(const std::string &key, int value);

        virtual ByteMessageImpl& putSysHeaders(const std::string &key, long value);

        virtual ByteMessageImpl& putSysHeaders(const std::string &key, double value);

        virtual ByteMessageImpl& putSysHeaders(const std::string &key, const std::string &value);

        virtual ByteMessageImpl& putUserHeaders(const std::string &key, int value);

        virtual ByteMessageImpl& putUserHeaders(const std::string &key, long value);

        virtual ByteMessageImpl& putUserHeaders(const std::string &key, double value);

        virtual ByteMessageImpl& putUserHeaders(const std::string &key, const std::string &value);

        jobject getProxy();

    private:
        jobject objectByteMessage;

        jclass classByteMessage;

        jmethodID midSysHeaders;

        jmethodID midUserHeaders;

        jmethodID midGetBody;

        jmethodID midSetBody;

        jmethodID midPutSysHeadersInt;
        jmethodID midPutSysHeadersLong;
        jmethodID midPutSysHeadersDouble;
        jmethodID midPutSysHeadersString;

        jmethodID midPutUserHeadersInt;
        jmethodID midPutUserHeadersLong;
        jmethodID midPutUserHeadersDouble;
        jmethodID midPutUserHeadersString;

    };

    typedef NS::shared_ptr<ByteMessageImpl> ByteMessageImplPtr;

END_NAMESPACE_2(io, openmessaging)

#endif //OMS_BYTE_MESSAGE_IMPL_H
