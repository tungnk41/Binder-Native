#include <Client.h>

void Client::init() {
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

void Client::send(int32_t data) {
        iface->send(data);
}

void DeathRecipient::binderDied(const wp<IBinder>& who) {
                std::ignore = who;
                ALOGI("Client binder died");
                std::cout<<"Client binder died" <<std::endl;
}