#include <iostream>

// Shared dependencies between server and client
#include "../../sharedFiles/includes/comm_macros.hpp"

// ZMQ lib file
#include "../includes/zhelpers.hpp"

#include "../includes/utils.hpp"

int broker_thread(){
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5563");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "all", 3);

    ZMQ_msg zmqmsg;
    std::ostringstream stream_obj;

    while(true){
        stream_obj.str("");
        stream_obj.clear();

        std::string address = s_recv(subscriber);
        std::string contents = s_recv(subscriber);

        zmqmsg.ParseFromString(contents);
        std::cout << "Broker got [" << address << "] '" << contents << "' ";
        if(zmqmsg.receiver_id() == 0){
            std::cout << "---interpreting---> local" << std::endl;
            switch(zmqmsg.type_message()){
                case ADD_ROOM_S:{
                    create_room_thread(zmqmsg);
                    break;
                }
                default:
                    break;
            }
        }
        else{
            stream_obj << "users/" << zmqmsg.receiver_id() << "/broker";
            pub_mutex.lock();
            s_sendmore_b(publisher, stream_obj.str());
            s_send_b(publisher, zmqmsg.SerializeAsString());
            pub_mutex.unlock();
            std::cout << "---transfered---> [" << stream_obj.str() << "]" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
