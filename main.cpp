
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
                std::cout<<"Receiver #### "<<inData<<std::endl;
                return android::NO_ERROR;
            } break;
            default:
                return android::BBinder::onTransact(code, data, reply, flags);
        }
    }
};



class Client {
    public:
        class DeathRecipient: public android::IBinder::DeathRecipient {
            private: Client& mParent;
            public:
            explicit DeathRecipient(Client& parent): mParent(parent){};
            virtual ~DeathRecipient(){};

            void binderDied(const android::wp<android::IBinder>& who) override {
                std::cout<< "binderDied" <<std::endl;
                mParent.binderDied(who);
            }
        };


    private:
    android::sp<android::IServiceManager> mServiceManager;
    android::sp<android::IBinder> binder;
    android::sp<IFace> iface;
    android::sp<DeathRecipient> deathRecipient;

    public:
        

    Client() {
        mServiceManager = android::defaultServiceManager();
        assert(mServiceManager != 0);
        binder = mServiceManager->getService(android::String16("Server"));
        assert(binder != 0);
        iface = android::interface_cast<IFace>(binder);
        assert(iface != 0);

        deathRecipient = new DeathRecipient(*this);
        binder->linkToDeath(deathRecipient);
    }

    void send(int32_t data) {
        iface->send(data);
    }

    void binderDied(const android::wp<android::IBinder>& who) {
        if(binder == who){
            std::cout<<"Client Binder disconnected " <<std::endl;
        }
        else {
            std::cout<<"Else Client Binder disconnected " <<std::endl;
        }
    }
    void testDeath() {
        binder = nullptr;
    }
};





int main() {

    pid_t pid = fork();
    if(pid>0) {
        android::defaultServiceManager()->addService(android::String16("Server"), new Server());
        android::ProcessState::self()->startThreadPool();
        android::IPCThreadState::self()->joinThreadPool();
    }
    else {
        Client* client = new Client();
        client->send(10);
        client->testDeath();
    }
}