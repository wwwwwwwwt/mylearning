#include <iostream>
#include "Session.h"
#include <boost/asio.hpp>
#include <exception>

int main(){

    try{
        boost::asio::io_context ioc;
        using namespace std;
        Server s(ioc, 10086);
        ioc.run();

    }catch(std::exception& e){
        std::cout<<"err ! err is "<<e.what()<<std::endl;
    }

}