#include "oms/OpenMessaging.h"
#include "Connect.h"

using namespace io::openmessaging;
using namespace std;

struct OmsAccessPoint *createMessagingAccessPoint(const char *url, const char **attributes) {

    if (NULL != attributes) {
        KeyValuePtr kv = newKeyValueImpl();
        for (int i = 0;; i++) {
            if (NULL == *attributes) {
                break;
            }

            string entry(*attributes);
            string::size_type pos = entry.find('=');
            if (pos != string::npos) {
                kv->put(entry.substr(0, pos), entry.substr(pos + 1));
            }
        }

        MessagingAccessPointPtr ptr = getMessagingAccessPoint(string(url), kv);
         ptr.get();

    }

    return NULL;
}