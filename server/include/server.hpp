/*
** EPITECH PROJECT, 2024
** B-CPP-500-TLS-5-2-rtype-eddy.gardes
** File description:
** server.hpp
*/

#pragma once

#include <netinet/in.h>
#include <map>
#include <queue>
#include <thread>
#include "client.hpp"
#include <mutex>

struct Packet {
    int socket;
    enum dest {ALL, OTHERS, ONE} destination;
    std::string message;
};

class Server {
	public:
		static Server& get();
		void init();
		void run();
		void manage_file_descriptors();
		std::map<int, Client>& getClients();
		void add_client();
		void sendToClient(int client_socket, const std::string &msg);
		std::string receiveFromClient(int clientSocket, int client_id);
		void serialize ( const std::string &str, std::ostream &out, char key);
		std::string deserialize(std::istream &in, char key);
		void sendToAllClients(const std::string &msg);
		void sendToAllClientsExcept(int client_id, const std::string &msg);
		void receiveLoop();
		void sendLoop();
		fd_set& getRds();
		fd_set& getWds();
		std::mutex &getQueueMutex();
		std::condition_variable &getCond();
		std::queue<Packet>& getQueue();
	private:
		Server();
		~Server();
		std::map<int, Client> _clients;
		std::queue<Packet> _queue;
		std::condition_variable _cond;
		std::thread _receiveThread;
    	std::thread _sendThread;
		int _tcpSocket = -1;
		std::string _buffer;
		char _key = 0xA3;
		int id = 0;
		fd_set rfds;
		fd_set wfds;
		sockaddr_in _tcpAddr{};
		std::mutex _clientMutex;
		std::mutex _queueMutex;
};
