#include "chatserver/server.hpp"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <address> <port>" << std::endl;
        return 1;
    }
    
    std::string address = argv[1];
    int port = std::stoi(argv[2]);

    Server server;
    server.start(address, port);
    
    return 0;
}
