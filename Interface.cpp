#include "Interface.h"
#include <iostream>

android::status_t BnService::onTransact(uint32_t code, const android::Parcel& data, android::Parcel* reply, uint32_t flags) {
    data.checkInterface(this);

    switch(code) {
        case CMD_SEND: {
            auto inData = data.readInt32();
            std::cout<<"#### "<<inData<<std::endl;
            return android::NO_ERROR;
        } break;
        default:
            return android::BBinder::onTransact(code, data, reply, flags);
    }
}