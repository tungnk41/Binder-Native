#include <Interface.h>


IMPLEMENT_META_INTERFACE(Face,"IFace");

int32_t BpFace::send(int32_t input){
        android::Parcel data, reply;
        data.writeInterfaceToken(IFace::getInterfaceDescriptor());
        data.writeInt32(input);
        remote()->transact(CMD_SEND, data, &reply);
        return reply.readInt32();
    }