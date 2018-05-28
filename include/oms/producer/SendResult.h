#ifndef OMS_SEND_RESULT_H
#define OMS_SEND_RESULT_H

#include <string>

#include "oms/Portable.h"
#include "oms/Namespace.h"
#include "oms/Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, producer)

    /**
     * The result of sending a OMS message to server
     * with the message id and some properties.
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class SendResult : private Uncopyable {
    public:
        virtual ~SendResult() {

        }

        virtual std::string messageId() = 0;
    };

    typedef NS::shared_ptr<SendResult> SendResultPtr;

END_NAMESPACE_3(io, openmessaging, producer)

#endif // OMS_SEND_RESULT_H
