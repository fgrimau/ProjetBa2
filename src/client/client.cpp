#include "client.hpp"

Client::Client(char* adresse, uint16_t port):msg({}), sendMutex(),waitMutex(),reponseAttendue(0),client_socket(){
	int res;
	struct sockaddr_in server_addr, client_addr;


	//init du sockaddr du serveur
	server_addr.sin_family = AF_INET;
	server_addr.sin_port=htons(port);
	inet_aton(adresse,&server_addr.sin_addr);

	//init du sockaddr du client
	client_addr.sin_family = AF_INET;
	client_addr.sin_port=htons(0);
	client_addr.sin_addr.s_addr=htonl(INADDR_ANY);

	client_socket=socket(PF_INET,SOCK_STREAM,0);
	if(client_socket==-1){perror("socket failed: ");}

	res=bind(client_socket,reinterpret_cast<sockaddr*> (&client_addr),sizeof(struct sockaddr));
	if(res==-1){perror("Bind failed: ");close(client_socket);}

	res=connect(client_socket,reinterpret_cast<sockaddr*> (&server_addr),sizeof(struct sockaddr));
	if(res==-1){perror("Connect failed: ");close(client_socket);}

}

void* Client::run(){
	int res;
	bool running = true;
	
	fd_set rfds;//utilisation du multiplexage
	int n = client_socket+1;
	//sendString("C'est le 12");	

	while(running){
		FD_ZERO(&rfds);
		FD_SET(client_socket,&rfds);
		res = select(n,&rfds,NULL,NULL,NULL);
		if(res == -1){
			perror("'select' failed");close(client_socket);return nullptr;
		}

		if(FD_ISSET(client_socket, &rfds)){//il y'a un message à lire
			readMessage();
			if (reponseAttendue == msg.type){
				waitMutex.unlock();//notify la fct qui attendait cette réponse
				reponseAttendue = 0;
			}
		}
	}
	return nullptr;
}

void Client::sendMessage(message& msg){
	int res;
	uint32_t size;
	uint32_t sent_size = 0;
	const char* parser = msg.text.c_str();
	
	size = static_cast<uint32_t>(msg.text.length());
	size = htonl(size);

	res = static_cast<int>(send(client_socket, &msg.type, sizeof(msg.type), 0));
	res = static_cast<int>(send(client_socket, &size, sizeof(size), 0));
	while (sent_size<size){
		res = static_cast<int>(send(client_socket, parser, size-sent_size, 0));
		sent_size += static_cast<uint32_t>(res);
        parser += res;
	}	
}

void Client::readMessage(){
	//on lit la taille du message sur un uint_8 puis on lit tous les caractères
	uint32_t size;//taille du message
	int res;

	res = static_cast<int>(recv(client_socket, &msg.type , sizeof(msg.type), 0)); // reçois le type du message
	if(res==-1){

	}
	res = static_cast<int>(recv(client_socket, &size, sizeof(size), 0));
	if (res==-1){

	}
	size = ntohl(size);

	char* buffer = new char[size+1];
	char* parser = buffer;
	buffer[size] = '\0';

	while (size>0){
		int r = static_cast<int>(recv(client_socket, parser, size, 0));
		size -= static_cast<uint32_t>(r);
		parser += r;
	}

	msg.text = static_cast<std::string>(buffer); 
}

std::string* Client::waitAnswers(uint8_t typeAttendu, message& m){
	std::string* res = new std::string;
	sendMutex.lock();

	reponseAttendue = typeAttendu;
	sendMessage(m);

	waitMutex.lock();//saura toujours le verrouiller
	waitMutex.lock();//bloc le verrouillage (sorte de wait)

	*res = msg.text;
	waitMutex.unlock();
	sendMutex.unlock();
	return res;

}


bool Client::connection(std::string username, std::string password){
	message m{};
	//construction de la structure
	UserConnect obj;
	obj.set_pseudo(username);
	obj.set_password(password);

	obj.SerializeToString(&m.text);
	m.type = CON_S;

	std::string* reponse = waitAnswers(CON_R, m);

	bool res = (*reponse)[0];

	delete reponse;
	return res;
}

bool Client::createAcount(std::string username, std::string password){
	message m{};
	//construction de la structure
	UserRegister obj;
	obj.set_pseudo(username);
	obj.set_password(password);

	obj.SerializeToString(&m.text);
	m.type = RG_S;

	std::string* reponse = waitAnswers(RG_R, m);

	bool res = (*reponse)[0];

	delete reponse;
	return res;
}

void Client::chatSend(std::string m, std::string destinataire){
	/*
	On gère le CHAT_R comme message inattendu donc on fait pas de waitAnswer? ou on fait quand meme?
	*/
	message msg{};

	Chat obj;
	obj.set_pseudo(destinataire);
	obj.set_msg(m);

	obj.SerializeToString(&msg.text);
	msg.type = CHAT_S;

	std::string* reponse = waitAnswers(CHAT_R, msg);

	bool res = (*reponse)[0];

	delete reponse;
	//return res
}

void Client::sendInvitation(std::string destinataire){
	/*
	IDEM Chat pour CHAT_R
	*/
	message m{};

	Invitation obj;
	obj.set_pseudo(destinataire);

	obj.SerializeToString(&m.text);
	m.type = INVI_S;

	std::string* reponse = waitAnswers(INVI_R, m);

	bool res = (*reponse)[0];

	delete reponse;
	//return res;
}

bool Client::joinPartie(std::string host){
	message m{};

	Join obj;
	obj.set_pseudo(host);

	obj.SerializeToString(&m.text);
	m.type = JOIN_S;

	std::string* reponse = waitAnswers(JOIN_R, m);

	bool res = (*reponse)[0];

	delete reponse;
	return res;
}

void Client::setLombricName(uint32_t id, std::string name){
	message m{};

	Lomb_mod obj;
	obj.set_id_lomb(id);
	obj.set_name_lomb(name); //dans .proto c'est nome_Lomb mais compil dit de mettre name_lomb

	obj.SerializeToString(&m.text);
	m.type = LOMB_MOD;

	std::string* reponse = waitAnswers(LOMB_R, m);

	//bool res = (*reponse)[0];

	delete reponse;
	//return res;
}

int main(){}

