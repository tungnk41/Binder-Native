
#ifndef _INTERFACE_BINDER_H_
#define _INTERFACE_BINDER_H_

#include <stdlib.h>
#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/Parcel.h>

using namespace android;

class IFace: public android::IInterface
{
    public:
    DECLARE_META_INTERFACE(Face);
    enum {
        CMD_SEND = android::IBinder::FIRST_CALL_TRANSACTION,
    };
    virtual int32_t send(int32_t data) = 0;
};

class BpFace : public android::BpInterface<IFace> 
{
    public:
    BpFace(const android::sp<android::IBinder>& impl) : android::BpInterface<IFace>(impl){};
    int32_t send(int32_t input);
};

#endif