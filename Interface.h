
#include <stdlib.h>
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

enum {
    CMD_SEND = 1,
};
class IServer;

class BnService : public android::BnInterface<IServer>
{
    public:
    virtual android::status_t onTransact(uint32_t code, const android::Parcel& data, android::Parcel* reply, uint32_t flags = 0);
};

class Server : public BnService{
    public:
    Server();
    ~Server();
    virtual void send(int32_t data) {
        std::cout<<"#### "<<data<<std::endl;
    } 
};

class IServer: public android::IInterface
{
    public:
    virtual void send(int32_t data) = 0;
    DECLARE_META_INTERFACE(Server);
};

class BpService : public android::BpInterface<IServer> 
{
    public:
    BpService(const android::sp<android::IBinder>& impl) : android::BpInterface<IServer>(impl){};

    virtual void send(int32_t input) {
        android::Parcel data, reply;
        data.writeInterfaceToken(IServer::getInterfaceDescriptor());
        data.writeInt32(input);
        remote()->transact(CMD_SEND, data, &reply);
    }

};


// IServer::IServer();
// IServer::~IServer();
// const android::String16 IServer::descriptor("Server");
// const android::String16& IServer::getInterfaceDescriptor() const {
//     return IServer::descriptor;
// }
// android::sp<IServer> IServer::asInterface(const android::sp<android::IBinder>& impl) {
//     android::sp<IServer> server;
//         if (impl != NULL) {
//             server = static_cast<IServer*>(impl->queryLocalInterface(IServer::descriptor).get());
//             if (server == NULL) {
//                 server = new BpService(impl);
//             }
//         }
//         return server;
// }

class Client {
    private:
    android::sp<android::IServiceManager> mServiceManager;
    android::sp<android::IBinder> binder;
    android::sp<IServer> server;

    public:
    Client() {
        mServiceManager = android::defaultServiceManager();
        assert(mServiceManager != 0);
        binder = mServiceManager->getService(android::String16("Server"));
        assert(binder != 0);
        server = android::interface_cast<IServer>(binder);
        assert(server != 0);
    }

    void send(int32_t data) {
        server->send(data);
    }
};

