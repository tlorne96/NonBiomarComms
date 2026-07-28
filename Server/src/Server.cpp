#include "Server.h"

#include <iostream>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>

#include <unistd.h>
#include <arpa/inet.h>


/*
    Helper function.

    Creates timestamps like:

    [14:32]

    This keeps the time formatting
    consistent for all users.
*/
std::string getTimestamp()
{
    auto now =
        std::chrono::system_clock::now();


    auto time =
        std::chrono::system_clock::to_time_t(now);


    std::stringstream ss;


    ss << "["
       << std::put_time(
            std::localtime(&time),
            "%H:%M"
          )
       << "] ";


    return ss.str();
}





Server::Server(int port)
{
    this->port = port;
}





void Server::start()
{
    /*
        Create TCP socket
    */

    serverSocket =
        socket(
            AF_INET,
            SOCK_STREAM,
            0
        );


    if(serverSocket < 0)
    {
        throw std::runtime_error(
            "Could not create socket"
        );
    }



    sockaddr_in serverAddress{};


    serverAddress.sin_family =
        AF_INET;


    serverAddress.sin_port =
        htons(port);


    /*
        Listen on every network interface
    */

    serverAddress.sin_addr.s_addr =
        INADDR_ANY;



    if(bind(
        serverSocket,
        (sockaddr*)&serverAddress,
        sizeof(serverAddress)
    ) < 0)
    {
        throw std::runtime_error(
            "Bind failed"
        );
    }



    listen(
        serverSocket,
        10
    );


    std::cout
        << "Server listening on port "
        << port
        << std::endl;



    acceptClients();
}





void Server::acceptClients()
{
    while(true)
    {
        sockaddr_in clientAddress{};

        socklen_t size =
            sizeof(clientAddress);



        int clientSocket =
            accept(
                serverSocket,
                (sockaddr*)&clientAddress,
                &size
            );



        if(clientSocket < 0)
            continue;



        std::cout
            << "Client connected"
            << std::endl;



        /*
            Each client gets its own thread.

            This allows multiple users
            to chat simultaneously.
        */

        std::thread(
            &Server::handleClient,
            this,
            clientSocket
        ).detach();
    }
}





void Server::handleClient(
    int clientSocket
)
{
    char buffer[1024];



    /*
        First packet from client
        should contain:

        LOGIN:Username
    */


    memset(
        buffer,
        0,
        sizeof(buffer)
    );



    int bytes =
        recv(
            clientSocket,
            buffer,
            sizeof(buffer),
            0
        );



    if(bytes <= 0)
    {
        close(clientSocket);
        return;
    }



    std::string username(buffer);



    /*
        Remove newline characters

        LOGIN:Bob\n

        becomes:

        LOGIN:Bob
    */

    username.erase(
        std::remove(
            username.begin(),
            username.end(),
            '\n'
        ),
        username.end()
    );


    username.erase(
        std::remove(
            username.begin(),
            username.end(),
            '\r'
        ),
        username.end()
    );



    /*
        Remove LOGIN prefix
    */

    if(username.rfind("LOGIN:",0)==0)
    {
        username =
            username.substr(6);
    }



    /*
        Store this connected user
    */

    Client newClient;


    newClient.socket =
        clientSocket;


    newClient.username =
        username;



    {
        std::lock_guard lock(
            clientsMutex
        );


        clients.push_back(
            newClient
        );
    }



    std::cout
        << username
        << " joined"
        << std::endl;



    broadcast(
        "SYSTEM:"
        + username
        + " joined the server",
        -1
    );


    sendUserList();





    /*
        Main chat loop.

        The client stays connected here.

        Every message received gets
        broadcast to everyone else.
    */

    while(true)
    {
        memset(
            buffer,
            0,
            sizeof(buffer)
        );


        int received =
            recv(
                clientSocket,
                buffer,
                sizeof(buffer),
                0
            );



        if(received <= 0)
        {
            break;
        }



        std::string message(buffer);



        message.erase(
            std::remove(
                message.begin(),
                message.end(),
                '\n'
            ),
            message.end()
        );



        if(message.empty())
            continue;



        std::string formatted =
            getTimestamp()
            + username
            + ": "
            + message;



        broadcast(
            formatted,
            -1
        );
    }





    /*
        Client disconnected.

        Remove them from the list.
    */

    {
        std::lock_guard lock(
            clientsMutex
        );


        clients.erase(
            std::remove_if(
                clients.begin(),
                clients.end(),

                [&](Client& c)
                {
                    return c.socket ==
                           clientSocket;
                }

            ),
            clients.end()
        );
    }



    close(clientSocket);



    broadcast(
        "SYSTEM:"
        + username
        + " left the server",
        -1
    );


    sendUserList();



    std::cout
        << username
        << " disconnected"
        << std::endl;
}







void Server::broadcast(
    const std::string& message,
    int senderSocket
)
{
    std::lock_guard lock(
        clientsMutex
    );


    for(auto& client : clients)
    {
        /*
            Do not send the message
            back to the person who sent it.

            senderSocket == -1 means
            send to everyone.
        */

        if(
            senderSocket == -1 ||
            client.socket != senderSocket
        )
        {
            send(
                client.socket,
                message.c_str(),
                message.size(),
                0
            );
        }
    }
}







void Server::sendUserList()
{
    std::string list = "USERS:";


    /*
        Build the user list.

        We only hold the mutex while
        accessing the vector.
    */
    {
        std::lock_guard lock(
            clientsMutex
        );


        for(auto& client : clients)
        {
            list += client.username;
            list += ",";
        }
    }



    /*
        Add newline so the client knows
        this packet is complete.
    */
    list += "\n";



    /*
        Send the update to everyone.

        Lock again because clients
        may change while sending.
    */
    {
        std::lock_guard lock(
            clientsMutex
        );


        for(auto& client : clients)
        {
            send(
                client.socket,
                list.c_str(),
                list.size(),
                0
            );
        }
    }
}