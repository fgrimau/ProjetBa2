#include <iostream>
#include <thread>
#include <unistd.h>

#include "../includes/user_thread.hpp"
#include "../includes/game_thread.hpp"
#include "../includes/listener.hpp"
#include "../includes/utils.hpp"
#include "../includes/zhelpers.hpp"


int client_thread(int socket_client){
    Listener la_poste(socket_client);
    ConnectedPlayer usr;

    //std::cout << "Sending" << std::endl;
    pub_mutex.lock();
    s_sendmore_b(publisher, "A");
    s_send_b(publisher, "We don't want to see this");
    s_sendmore_b(publisher, "all");
    s_send_b(publisher, "We would like to see this");
    pub_mutex.unlock();

    while(1){
        uint8_t type = la_poste.reception_type();
        if(type == EXIT_FAILURE){
            break;
        }
        handle_instruction(type, &la_poste, &usr);
    }
    close(socket_client);
    return EXIT_SUCCESS;
}
