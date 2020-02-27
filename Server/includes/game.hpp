#ifndef game
    #define game

#include <vector>
//#include <ctime>
#include "../cpl_proto/user.pb.h"


//Tout ce qui est en rapport avec la fin de la partie(timer) sera gerer partie 3(eau qui monte)
class Game{
    public:
        Game();
        ~Game();

        //void set_begin();
        void set_lomb(int nbr_player,int nb_lomb);
        //bool check_time();
        uint8_t* who_next();
        void handle_room(ZMQ_msg zmq_msg, int* current_step);

    private:
        uint8_t map_id;
        //int time_game;
        //time_t begin;
        int time_round_game; //duréé d'un partie en sec
        uint8_t nbr_lomb;
        uint8_t nbr_eq;
        std::vector<bool> alive_lomb;
        std::vector<uint8_t> current_lomb;
        uint8_t current_player;
        uint8_t lomb[2];
        uint32_t player_id[4];
        uint32_t owner_id;

};

#endif
