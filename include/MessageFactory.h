#ifndef OMS_MESSAGE_FACTORY_H
#define OMS_MESSAGE_FACTORY_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "ByteMessage.h"
#include "Namespace.h"

BEGIN_NAMESPACE_2(io, openmessaging)

    class MessageFactory {
    public:
        virtual ~MessageFactory() {
        }

        virtual boost::shared_ptr<ByteMessage>
        createByteMessageToTopic(std::string &topic, std::vector<char> &body) = 0;

        virtual boost::shared_ptr<ByteMessage>
        createByteMessageToQueue(std::string &topic, std::vector<char> &body) = 0;
    };

END_NAMESPACE_2(io, openmessaging)

#endif // OMS_MESSAGE_FACTORY_H
