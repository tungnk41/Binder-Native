
#include "interface/Interface.h"
#include <assert.h>

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