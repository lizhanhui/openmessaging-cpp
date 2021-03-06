#include <plog/Log.h>

#include "BuiltinKeys.h"
#include "OMS.h"
#include "consumer/PullConsumer.h"
#include "MessagingAccessPoint.h"


using namespace std;
using namespace io::openmessaging;

int main(int argc, char *argv[]) {

    // Define access point
    const string accessPointUrl = "oms:rocketmq://ons3.dev:9876/default:default";

    load_library(accessPointUrl);

    // Acquire messaging access point instance through factory method
    MessagingAccessPointPtr accessPoint(getMessagingAccessPoint(accessPointUrl));

    std::string queueName("TopicTest");

    KeyValuePtr subKV(newKeyValue());
    const std::string consumer_group_value = "OMS_CONSUMER";
    subKV->put(CONSUMER_ID, consumer_group_value);


    consumer::PullConsumerPtr pullConsumer = accessPoint->createPullConsumer(subKV);
    pullConsumer->attachQueue(queueName);

    pullConsumer->startup();

    while (true) {
        MessagePtr msg = pullConsumer->receive();
        if (msg) {
            KeyValuePtr sysHeaders = msg->sysHeaders();
            std::string msgId = sysHeaders->getString(MESSAGE_ID);
            LOG_INFO << "Receive a new message. MsgId: " << msgId;
            pullConsumer->ack(msgId);
            LOG_INFO << "Acknowledging " << msgId << " OK";
        } else {
            LOG_INFO << "No new messages available. Sleep 5 seconds";
            sleep(5);
        }
    }

}