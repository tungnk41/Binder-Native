#include "Interface.h"
#include <iostream>

status_t BnService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
    data.checkInterface(this);

    switch(code) {
        case CMD_SEND: {
            int32_t inData = data.readInt32();
            std::cout<<"#### "<<inData<<std::endl;
            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}