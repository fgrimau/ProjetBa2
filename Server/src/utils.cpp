#include <iostream>
#include <unistd.h>
#include <cstdarg>

#include "../includes/utils.hpp"
#include "../includes/comm_macros.hpp"
#include "../includes/listener.hpp"
#include "../includes/database.hpp"
#include "../cpl_proto/user.pb.h"
#include "../includes/connected_player.hpp"


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


void handle_instruction(int msg_type, Listener* la_poste , DataBase* db, ConnectedPlayer* usr){
    if(msg_type == CON_S){
        la_poste->reception();
        usr->ParseFromString(la_poste->get_buffer());
        if(usr->isregister()){ // si joueur a deja un compte
            if(usr->check_passwd(db, usr->password())){ // test indentifiant + password
                int user_id;
                db->get_user_id(usr->pseudo(), &user_id);
                usr->set_id(user_id);
                la_poste->envoie_bool(CON_R,1);
            }
            else{
                la_poste->envoie_bool(CON_R,0);
            }
        }
        else{
            db->register_user(usr->pseudo(),usr->password());
            int user_id;
            db->get_user_id(usr->pseudo(), &user_id);
            usr->set_id(user_id);
            la_poste->envoie_bool(CON_R,1);
        }
    }
    else if(usr->is_auth()){
        switch(msg_type){
            case CHAT_S:{
                Chat chat_ob;
                la_poste->reception();
                chat_ob.ParseFromString(la_poste->get_buffer());
                int receiver_id;db->get_user_id(chat_ob.pseudo(), &receiver_id);
                db->send_message(usr->get_id(), receiver_id, chat_ob.msg());
                break;
            }
            case CHAT_R:{
                Chat_r chat_r;
                //int friend_id;db->get_user_id(fri.user(), &friend_id);
                //db->get_convo(usr->get_id(), friend_id, &chat_r); // Need id user
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
                Lomb_r lomb_r;
                db->get_lombrics(usr->get_id(), &lomb_r);
                la_poste->envoie_msg(LOMB_R, lomb_r.SerializeAsString());
                break;
            }
            case LOMB_MOD:{
                Lomb_mod modif;
                la_poste->reception();
                modif.ParseFromString(la_poste->get_buffer());
                db->set_lombric_name(modif.id_lomb(),modif.name_lomb());
                break;
            }
            case GET_HISTORY:{
                Get_history r_history;
                la_poste->reception();
                r_history.ParseFromString(la_poste->get_buffer());
                History_r history_list;
                int user_r_id;db->get_user_id(r_history.pseudo(), &user_r_id);
                db->get_history(user_r_id, r_history.first_game(), r_history.nbr_game(), &history_list);
                la_poste->envoie_msg(HISTORY_R, history_list.SerializeAsString());
                break;
            }
            //case GET_RANK:{
            //    Get_rank r_rank;
            //    la_poste->reception();
            //    r_rank.ParseFromString(la_poste->get_buffer());
            //    Rank_r rank_list;
            //    db->get_rank(r_rank.first_player(),r_rank.nbr_player(), &rank_list);
            //    la_poste->envoie_msg(RANK_R, rank_list.SerializeAsString());
            //}
            case FRI_ADD:{
                Fri_add fri;
                la_poste->reception();
                fri.ParseFromString(la_poste->get_buffer());
                int friend_id;db->get_user_id(fri.user(), &friend_id);
                db->add_friend(usr->get_id(), friend_id);
                break;
            }
            case FRI_ACCEPT:{
                Fri_accept fri;
                la_poste->reception();
                fri.ParseFromString(la_poste->get_buffer());
                int friend_id;db->get_user_id(fri.user(), &friend_id);
                db->accept_friend_invite(usr->get_id(), friend_id);
                break;
            }
            case FRI_LS_S:{
                Fri_ls_r fri;
                db->get_friend_list(usr->get_id(), &fri);
                la_poste->envoie_msg(FRI_LS_R, fri.SerializeAsString());
                break;
            }
            case FRI_RMV:{
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