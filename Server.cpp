#include "Server.h"

 int32_t Server::send(int32_t data){
        std::cout<<data;
        return data;
} 

android::status_t Server::onTransact(uint32_t code, const android::Parcel& data, android::Parcel* reply, uint32_t flags) {
    CHECK_INTERFACE(IFace,data,reply);
        switch(code) {
            case CMD_SEND: {
                auto inData = data.readInt32();
                 std::cout << inData << std::endl;
                //  exit(0);
                return android::NO_ERROR;
            } break;
            default:
                return android::BBinder::onTransact(code, data, reply, flags);
        }
}