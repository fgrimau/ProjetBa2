#include <iostream>
#include <unistd.h>
#include <cstdarg>

#include "../includes/utils.hpp"
#include "../includes/comm_macros.hpp"
#include "../includes/listener.hpp"
#include "../includes/database.hpp"
#include "../cpl_proto/user.pb.h"
#include "../includes/connected_player.hpp"

int match_queue[5];
int nb_waiting_players;
std::mutex mu;
std::condition_variable cv;

void add_me_to_queue(int user_id){
    match_queue_mut.lock();
    match_queue[nb_waiting_players] = user_id;
    nb_waiting_players++;
    match_queue_mut.unlock();
    std::cout << "Nokeromheruilhgeri" << std::endl;

    {
        std::lock_guard<std::mutex> lk(mu);
    }
    cv.notify_all();
}

// Depending on the value of res, indicate an error, with the error message provided,
// and closes given pipes before exiting the program
void catch_error(int res, int is_perror, const char* msg, int nb_to_close, ...){
    if(res == -1){
        if(is_perror == 1){
            perror(msg);
        }
        else{
            std::cout << msg << std::endl;
        }

        if(nb_to_close > 0){
            va_list ap;
            va_start(ap, nb_to_close);

            int i;
            // traverse rest of the arguments to find out minimum
            for(i = 2; i <= nb_to_close; i++)
                close(va_arg(ap, int));
        }
        exit(EXIT_FAILURE);
    }
}


void handle_instruction(uint8_t msg_type, Listener* la_poste , DataBase* db, ConnectedPlayer* usr){
    if(msg_type == CON_S){
        std::cout << "con_s" << std::endl;
        la_poste->reception();
        std::cout << "con_s bien reçu" << std::endl;
        usr->ParseFromString(la_poste->get_buffer());
        std::cout << usr->DebugString() << std::endl;
        if(usr->isregister()){ // si joueur a deja un compte
            if(usr->check_passwd(db, usr->password())){
                std::cout << "bon pass" << std::endl; // test indentifiant + password
                int user_id;
                db->get_user_id(usr->pseudo(), &user_id);
                usr->set_id(user_id);
                usr->set_auth(true);
                la_poste->envoie_bool(CON_R,1);
            }
            else{
                std::cout << "mauvais pass" << std::endl;
                la_poste->envoie_bool(CON_R,0);
            }
        }
        else{
            int id;
            db->get_user_id(usr->pseudo(), &id);
            if(id > 0){ // A user with the same pseudonym exists
                std::cout << "Existing user with same pseudo id = " << id << std::endl;
                la_poste->envoie_bool(CON_R, 0);
            }
            else{
                int res = db->register_user(usr->pseudo(),usr->password());
                int user_id;
                db->get_user_id(usr->pseudo(), &user_id);
                usr->set_id(user_id);
                usr->set_auth(true);
                for(int i=0;i<8;i++){
                    db->add_lombric(user_id, i, "anélonyme");
                }
                la_poste->envoie_bool(CON_R, 1);
            }
        }
    }
    else if(usr->is_auth()){
        switch(msg_type){
            case CHAT_S:{
                std::cout << "chat_s" << std::endl;
                Chat chat_ob;
                la_poste->reception();
                chat_ob.ParseFromString(la_poste->get_buffer());
                std::cout << chat_ob.DebugString() << std::endl;
                int receiver_id;db->get_user_id(chat_ob.pseudo(), &receiver_id);
                db->send_message(usr->get_id(), receiver_id, chat_ob.msg());
                break;
            }
            case GET_CONVO:{
                std::cout << "get_convo" << std::endl;
                convo_s request_convo;
                Chat_r chat_r;
                la_poste->reception();
                request_convo.ParseFromString(la_poste->get_buffer());
                int friend_id;db->get_user_id(request_convo.pseudo(), &friend_id);
                db->get_convo(usr->get_id(), friend_id, &chat_r); // Need id user
                la_poste->envoie_msg(CHAT_R, chat_r.SerializeAsString());
                break;
            }
            //case INVI_S:{
            //    Invitation invit;
            //    la_poste->reception();
            //    invit.ParseFromString(la_poste->get_buffer());
            //    db->send_invitation(usr->pseudo(),invit.pseudo());
            //}
            case GET_LOMB:{
                std::cout << "get_lomb" << std::endl;
                Lomb_r lomb_r;
                db->get_lombrics(usr->get_id(), &lomb_r);
                la_poste->envoie_msg(LOMB_R, lomb_r.SerializeAsString());
                break;
            }
            case LOMB_MOD:{
                std::cout << "lomb_mob" << std::endl;
                Lomb_mod modif;
                la_poste->reception();
                modif.ParseFromString(la_poste->get_buffer());
                db->set_lombric_name(modif.id_lomb(),modif.name_lomb());
                break;
            }
            case GET_HISTORY:{
                std::cout << "get_history" << std::endl;
                Get_history r_history;
                la_poste->reception();
                r_history.ParseFromString(la_poste->get_buffer());
                History_r history_list;
                int user_r_id;db->get_user_id(r_history.pseudo(), &user_r_id);
                db->get_history(user_r_id, r_history.first_game(), r_history.nbr_game(), &history_list);
                la_poste->envoie_msg(HISTORY_R, history_list.SerializeAsString());
                break;
            }
            case GET_RANK:{
                std::cout << "get_rank" << std::endl;
                Get_rank r_rank;
                la_poste->reception();
                r_rank.ParseFromString(la_poste->get_buffer());
                Rank_r rank_list;
                int user_id;db->get_user_id(r_rank.first_player(), &user_id);
                db->get_rank(user_id,r_rank.nbr_player(), &rank_list);
                la_poste->envoie_msg(RANK_R, rank_list.SerializeAsString());
                break;
            }
            case FRI_ADD:{
                std::cout << "fri_add" << std::endl;
                Fri_add fri;
                la_poste->reception();
                fri.ParseFromString(la_poste->get_buffer());
                int friend_id;db->get_user_id(fri.user(), &friend_id);
                db->add_friend(usr->get_id(), friend_id);
                break;
            }
            case FRI_ACCEPT:{
                std::cout << "fri_accept" << std::endl;
                Fri_accept fri;
                la_poste->reception();
                fri.ParseFromString(la_poste->get_buffer());
                int friend_id;db->get_user_id(fri.user(), &friend_id);
                db->accept_friend_invite(usr->get_id(), friend_id);
                break;
            }
            case FRI_LS_S:{
                std::cout << "fri_ls_s" << std::endl;
                Fri_ls_r fri;
                db->get_friend_list(usr->get_id(), &fri);
                la_poste->envoie_msg(FRI_LS_R, fri.SerializeAsString());
                break;
            }
            case FRI_RMV:{
                std::cout << "fri_rmv" << std::endl;
                Fri_rmv fri;
                la_poste->reception();
                fri.ParseFromString(la_poste->get_buffer());
                int friend_id;db->get_user_id(fri.user(), &friend_id);
                db->remove_friend(usr->get_id(), friend_id);
                break;
            }
        }
    }
}
