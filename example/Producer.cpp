#include <plog/Log.h>
#include <cstdlib>
#include <oms/OpenMessaging.h>

using namespace std;
using namespace io::openmessaging;
using namespace io::openmessaging::producer;

int main(int argc, char *argv[]) {

    int count = 128;

    if (argc > 1) {
        char* end;
        count = static_cast<int>(std::strtol(argv[1], &end, 10));
    }

    // Define access point
    const string accessPointUrl = "oms:rocketmq://ons3.dev:9876/default:default";

    try {
        // Acquire messaging access point instance through factory method
        MessagingAccessPointPtr accessPoint = getMessagingAccessPoint(accessPointUrl);

        // Create a producer instance via MessageAccessPoint instance
        ProducerPtr producer = accessPoint->createProducer();

        // Start producer and hereafter be prepared to send message to brokers
        producer->startup();

        // Now that producers are ready to deliver, we are about to create messages
        const char *slogan = "OpenMessagingService is the NEXT generation of messaging system";
        const MessageBody message_body((signed char*)slogan, strlen(slogan));

        // Create message by producer, which also plays the role of message factory
        ByteMessagePtr message = producer->createBytesMessage("TopicTest", message_body);

        for (int i = 0; i < count; ++i) {
            // Send message
            SendResultPtr sendResult = producer->send(message);

            // SendResult instance holds meta data including message id which may be employed to query various info on messaging
            // console
            LOG_INFO << "Sending Message OK. Message ID: " << sendResult->messageId();
        }


        // Shutdown producer if it's no longer used. Typically is called when your application is about to quit.
        producer->shutdown();
    } catch (OMSException &e) {
        cout << e.what() << endl;
    }
}