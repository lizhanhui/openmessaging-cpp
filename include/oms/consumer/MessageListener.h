#ifndef OMS_MESSAGE_LISTENER_H
#define OMS_MESSAGE_LISTENER_H

#include "oms/Portable.h"
#include "oms/consumer/Context.h"
#include "oms/Namespace.h"
#include "oms/Message.h"
#include "oms/Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, consumer)

    /**
     * The message listener interface. A message listener must implement this {@code MessageListener} interface and register
     * itself to a consumer instance to asynchronously receive messages.
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class MessageListener : private Uncopyable {
    public:
        virtual ~MessageListener() {

        }

        virtual void onMessage(MessagePtr &message, ContextPtr &context) = 0;
    };

    typedef NS::shared_ptr<MessageListener> MessageListenerPtr;

END_NAMESPACE_3(io, openmessaging, consumer)
#endif //OMS_MESSAGE_LISTENER_H
