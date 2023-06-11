
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
#include <iostream>
#include <thread>      
#include <chrono>
#include <tuple>

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

    int32_t send(int32_t input) override {
        android::Parcel data, reply;
        data.writeInterfaceToken(IFace::getInterfaceDescriptor());
        data.writeInt32(input);
        remote()->transact(CMD_SEND, data, &reply);
        return reply.readInt32();
    }
};


//Use this macro at end of cpp defination file
IMPLEMENT_META_INTERFACE(Face,"IFace");


// IFace::IFace(){};
// IFace::~IFace(){};
// const android::String16 IFace::descriptor("IFace");
// const android::String16& IFace::getInterfaceDescriptor() const {
//     return IFace::descriptor;
// }
// android::sp<IFace> IFace::asInterface(const android::sp<android::IBinder>& impl) {
//     android::sp<IFace> iface;
//         if (impl != NULL) {
//             iface = static_cast<IFace*>(impl->queryLocalInterface(IFace::descriptor).get());
//             if (iface == NULL) {
//                 iface = new BpFace(impl);
//             }
//         }
//         return iface;
// }



class Server : public android::BnInterface<IFace>
{
    public:
    int32_t send(int32_t data) override {
        std::cout<<data;
        return data;
    } 

    android::status_t onTransact(uint32_t code, const android::Parcel& data, android::Parcel* reply, uint32_t flags) {
    CHECK_INTERFACE(IFace,data,reply);
        switch(code) {
            case CMD_SEND: {
                auto inData = data.readInt32();
                 std::cout << inData << std::endl;
                //  exit(0);
                return android::NO_ERROR;
            } break;
            default:
                return android::BBinder::onTransact(code, data, reply, flags);
        }
    }
};

class DeathRecipient: public android::IBinder::DeathRecipient {
            public:
            explicit DeathRecipient(){};
            virtual ~DeathRecipient(){};

            virtual void binderDied(const android::wp<android::IBinder>& who) override{
                std::cout<< "binderDied "<<std::endl;
                std::ignore = who;
            }
};

class Client : public android::RefBase{
    public:
    android::sp<android::IServiceManager> mServiceManager;
    android::sp<android::IBinder> binder;
    android::sp<IFace> iface;
    android::sp<DeathRecipient> deathRecipient;

    public:
    Client() {}

    void init() {
         mServiceManager = android::defaultServiceManager();
        assert(mServiceManager != 0);
        binder = mServiceManager->getService(android::String16("Server"));
        assert(binder != 0);
        iface = android::interface_cast<IFace>(binder);
        assert(iface != 0);

        deathRecipient = new DeathRecipient();
        if (binder->linkToDeath(deathRecipient) != android::NO_ERROR) {
            std::cout << "Failed to register DeathRecipient" << std::endl;
        }
        else {
            std::cout << "register DeathRecipient" << std::endl;
        }
    }

    void send(int32_t data) {
        iface->send(data);
    }

    void binderDied(const android::wp<android::IBinder>& who) {
        if(binder == who){
            std::cout<<"Client Binder disconnected " <<std::endl;
            iface = nullptr;
        }
        else {
            std::cout<<"Else Client Binder disconnected " <<std::endl;
        }
    }
};



int main(int argc, char **argv) {
    std::ignore = argv;
    if(argc == 1) {
        //client
        Client* client = new Client();
        client->init();
        while(true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout <<"From client: " << client->binder->pingBinder()<<std::endl;
            client->send(10);
        }
    }
    else {
        android::defaultServiceManager()->addService(android::String16("Server"), new Server());
        android::ProcessState::self()->startThreadPool();
        android::IPCThreadState::self()->joinThreadPool();

    }

    return 0;
}