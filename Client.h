#ifndef _CLIENT_BINDER_H_
#define _CLIENT_BINDER_H_

#include <stdlib.h>
#include <utils/Log.h>
#include <utils/RefBase.h>
#include <binder/TextOutput.h>
#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <binder/Parcel.h>
#include <assert.h>
#include "Interface.h"
#include <iostream>

class DeathRecipient: public BBinder::DeathRecipient {
            public:
            explicit DeathRecipient(){};
            virtual ~DeathRecipient(){};
            void binderDied(const android::wp<IBinder>& who);
};

class Client {
    public:
    android::sp<android::IServiceManager> mServiceManager;
    android::sp<android::IBinder> binder;
    android::sp<IFace> iface;
    android::sp<DeathRecipient> deathRecipient;

    public:
    Client() {}
    void init();
    void send(int32_t data);
};

#endif