#include <iostream>
#include <boost/asio.hpp>

using namespace std;

using namespace std;
const int MAX_LENGTH = 1024;
using namespace boost::asio::ip;


int main(){

    try
    {
        boost::asio::io_context ioc;
        tcp::endpoint remote_ep(address::from_string("127.0.0.1"), 10086);
        tcp::socket socket(ioc);
        boost::system::error_code error = boost::asio::error::host_not_found;
        socket.connect(remote_ep, error);
        if(error){
            cout<<"connect failed, code is "<<error.value()<<" err msg is "<< error.message()<<endl; 
            return 0;
        }

        std::cout<<"Enter Message: ";
        char request[MAX_LENGTH];
        std::cin.getline(request, MAX_LENGTH);
        size_t request_length = strlen(request);
        boost::asio::write(socket, boost::asio::buffer(request,request_length));
        char reply[MAX_LENGTH];
        size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, request_length));
        cout<<"reply: "<<string(reply, reply_length);
        getchar();
    }
    catch(const std::exception& e)
    {
        std::cerr<<"Exception is "<<e.what()<<std::endl;
    }
    

}