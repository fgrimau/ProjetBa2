#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#include "../includes/constant.hpp"
#include "../includes/listener.hpp"

Listener::Listener(int socket_client){
    str_buffer = new char[INIT_SIZE_BUFFER];
    current_size_buffer = INIT_SIZE_BUFFER;
    sockfd = socket_client;
}
Listener::~Listener(){
    delete[] str_buffer;
    close(sockfd);
}

int Listener::reception_type(){
    m_res = static_cast<int>(recv(sockfd, &m_packet_size, sizeof(uint32_t), 0));
    if(m_res == -1){
        perror("Failed receive message\n");
        return EXIT_FAILURE;
    }

    m_len_char = ntohl(m_packet_size);
    return m_len_char;
}

int Listener::reception(){
    m_res = static_cast<int>(recv(sockfd, &m_packet_size, sizeof(uint32_t), 0));
    if(m_res == -1){
        perror("Failed receive message\n");
        return EXIT_FAILURE;
    }

    m_len_char = ntohl(m_packet_size);
    if( static_cast<long unsigned int>(m_len_char+1) > current_size_buffer ){
        delete[] str_buffer;
        str_buffer = new char[m_len_char+1];
        current_size_buffer = m_len_char+1;
    }

    else if( m_len_char+1 < INIT_SIZE_BUFFER && current_size_buffer != INIT_SIZE_BUFFER){
        std::cout << "b" << std::endl;
        delete[] str_buffer;
        str_buffer = new char[INIT_SIZE_BUFFER];
    }
    bzero(str_buffer, current_size_buffer);
    for(m_str_parser = str_buffer, m_received_size = 0;static_cast<uint32_t>(m_received_size) < m_len_char; ){
        m_res = static_cast<int>(recv(sockfd, str_buffer, static_cast<long unsigned int>(m_len_char), 0));
        if(m_res == -1){
            perror("Unable to receive message.\n");
            return EXIT_FAILURE;
        }
        else if(m_res == 0){
            perror("Client closed socket.\n");
            return EXIT_FAILURE;
        }

        m_received_size += m_res;
        m_str_parser += m_res;
    }
    (str_buffer)[m_len_char+1] = '\0';
    return EXIT_SUCCESS;
}

int Listener::envoie_bool(int type_msg ,int boolint){
    m_res = 0;

    m_packet_size = htonl(type_msg);
    m_res = static_cast<int>(send(sockfd, &m_packet_size, sizeof(uint32_t), 0));
    if(m_res == -1){
        perror("Unable to send message size.\n");
        return EXIT_FAILURE;
    }

    m_packet_size = htonl(boolint);
    m_res = static_cast<int>(send(sockfd, &m_packet_size, sizeof(uint32_t), 0));
    if(m_res == -1){
        perror("Unable to send message size.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Listener::envoie_msg(int type_msg , std::string msg){

    m_res = 0;
    const char *cmsg = msg.c_str();
    m_len_char = static_cast<uint32_t>(strlen(cmsg));

    m_packet_size = htonl(type_msg);
    m_res = static_cast<int>(send(sockfd, &m_packet_size, sizeof(uint32_t), 0));
    if(m_res == -1){
        perror("Unable to send message size.\n");
        return EXIT_FAILURE;
    }

    m_packet_size = htonl(m_len_char);
    m_res = static_cast<int>(send(sockfd, &m_packet_size, sizeof(uint32_t), 0));
    if(m_res == -1){
        perror("Unable to send message size.\n");
        return EXIT_FAILURE;
    }

    m_str_parser = static_cast<char*>(malloc (sizeof(char) * m_len_char));
    if(!m_str_parser){
        perror("Initialization of the parser buffer");
        return EXIT_FAILURE;
    }

    for(strncpy(m_str_parser, cmsg, m_len_char), m_sent_size=0; m_sent_size < m_len_char;){
        m_res = static_cast<int>(send(sockfd, cmsg, static_cast<long unsigned int>(m_len_char), 0));
        if(m_res == -1){
            perror("Unable to send message\n");
            return EXIT_FAILURE;
        }

        m_sent_size += static_cast<uint32_t>(m_res);
        m_str_parser += m_res;
    }
    if(m_res == -1){
        perror("Unable to send message\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

char* Listener::get_buffer(){
    return str_buffer;
}