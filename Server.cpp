
#include "interface/Interface.h"
#include <iostream>

class Server : public BnService{
    public:
    Server() {}
    virtual void send(int32_t data) {
        std::cout<<"#### "<<data<<std::endl;
    } 
};