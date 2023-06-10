
#include <utils/RefBase.h>
#include <binder/TextOutput.h>
#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

uint32_t CMD_SEND = 1;

class IServer: public android::IInterface
{
    DECLARE_META_INTERFACE(Server)
    virtual void send(int32_t data) = 0;
}

class BpService : public BpInterface<IServer> 
{
    BpService(const sp<IBinder>& binder) : BpInterface<IServer>(binder){};

    virtual void send(int32_t data) {
        Parcel data, reply;
        data.writeInterfaceToken(IServer::getInterfaceDescriptor());
        data.writeInt32(data);
        remote()->transact(CMD_PUSH, data, &reply);
    }

}

class BnService : public BnInterface<IServer>
{
    virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0);
}