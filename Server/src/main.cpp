#include "Server.h"


int main()
{
    try
    {
        Server server(10000);

        server.start();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}