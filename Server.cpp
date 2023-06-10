
#include "interface/Interface.h"
#include <iostream>

class Server : public BnService{
    public:
    Server() {}
    void init() {
        defaultServiceManager()->addService(String16("Server"), *this);
        android::ProcessState::self()->startThreadPool();
        IPCThreadState::self()->joinThreadPool();
    }
    virtual void send(int32_t data) {
        std::cout<<"#### "<<data<<std::endl;
    } 
}