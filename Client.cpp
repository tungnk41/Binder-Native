
#include "interface/Interface.h"

class Client {
    private:
    android::sp<IServiceManager> mServiceManager;
    android::sp<IBinder> binder;
    android::sp<IServer> server;

    public:
    Client() {
        mServiceManager = defaultServiceManager();
        ASSERT(mServiceManager != 0);
        binder = mServiceManager->getService(String16("Server"));
        ASSERT(binder != 0);
        server = interface_cast<IServer>(binder);
        ASSERT(server != 0);
    }

    void send(int32_t data) {
        server->send(data);
    }
}