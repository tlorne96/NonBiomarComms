#pragma once

#include "Client.h"

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>


class Server
{
public:

    Server(int port);

    // Starts listening for incoming connections
    void start();


private:

    int port;
    int serverSocket;


    // Stores connected clients and their usernames
    std::vector<Client> clients;


    // Prevents multiple threads editing clients simultaneously
    std::mutex clientsMutex;


    // Waits for new connections
    void acceptClients();


    // Handles communication with one client
    void handleClient(int clientSocket);


    // Sends a message to everyone connected
    void broadcast(
        const std::string& message,
        int senderSocket
    );


    // Sends the current user list to all connected clients
    void sendUserList();
};