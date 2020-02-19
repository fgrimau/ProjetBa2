#include <iostream>
#include <unistd.h>

#include "../includes/user_thread.hpp"
#include "../includes/shared_memory.hpp"
#include "../includes/listener.hpp"
#include "../includes/constant.hpp"
#include "../cpl_proto/user.pb.h"
#include "../includes/connected_player.hpp"
#include "../includes/database.hpp"

int client_thread(int socket_client, DataBase* db ){
    Listener yolo(socket_client);
    ConnectedPlayer usr;

    usr.set_pseudo("michel");
    int res;
    std::string msg;

    while(1){
        res = yolo.reception();
        if (res == EXIT_FAILURE){
            break;
        }
        printf("%s \n", yolo.get_buffer());
        usr.SerializeToString(&msg);
        res = yolo.envoie_msg(socket_client, msg);
        if (res == EXIT_FAILURE){
            break;
        }
    }

    close(socket_client);
    return EXIT_SUCCESS;
}
