#ifndef OMS_OMS_EVENT_H
#define OMS_OMS_EVENT_H

#include "oms/Namespace.h"
#include "oms/Uncopyable.h"

BEGIN_NAMESPACE_3(io, openmessaging, observer)

    /**
     * Observable objects can trigger a {@code OMSEvent}, which will be handled in {@link Observer#onEvent(OMSEvent)}
     *
     * @version OMS 1.0
     * @since OMS 1.0
     */
    class OMSEvent : private Uncopyable {
    public:
        virtual ~OMSEvent() {

        }

        virtual std::string type() = 0;

        virtual std::string details() = 0;
    };

    typedef NS::shared_ptr<OMSEvent> OMSEventPtr;

END_NAMESPACE_3(io, openmessaging, observer)
#endif //OMS_OMS_EVENT_H
