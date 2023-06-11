#ifndef _SERVER_BINDER_H_
#define _SERVER_BINDER_H_

#include <stdlib.h>
#include <binder/TextOutput.h>
#include <binder/IBinder.h>
#include <binder/Parcel.h>
#include "Interface.h"
#include <iostream>

class Server : public android::BnInterface<IFace>
{
    public:
    int32_t send(int32_t data); 
    android::status_t onTransact(uint32_t code, const android::Parcel& data, android::Parcel* reply, uint32_t flags);
};

#endif