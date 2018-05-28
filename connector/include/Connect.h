#ifndef OMS_CONNECT_H
#define OMS_CONNECT_H

#ifdef __cplusplus
extern "C" {
#endif
    struct OmsAccessPoint;
    struct OmsProducer;
    struct OmsPushConsumer;

    struct OmsAccessPoint* createMessagingAccessPoint(const char* url, const char** attributes);

#ifdef __cplusplus
};
#endif

#endif //OMS_CONNECT_H
