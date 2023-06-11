#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <iostream>
#include <thread>      
#include <chrono>
#include <tuple>
#include "Interface.h"
#include "Server.h"
#include "Client.h"

using namespace android;


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