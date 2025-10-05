#ifndef INCLUDE_CHATSERVER_SERVER_HPP
#define INCLUDE_CHATSERVER_SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Server
{
public:
    Server() = default;
    ~Server() = default;

public:
    void start()
    {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(25569);
        server_addr.sin_addr.s_addr = inet_addr("192.168.178.186");

        const sockaddr* ccc = (struct sockaddr *) &server_addr;
        bind(server_fd, ccc, sizeof(*ccc));
        listen(server_fd, 3);

        char buf[50];
        inet_ntop(AF_INET, (const void*) &(server_addr.sin_addr.s_addr), buf, 50);

        std::cout << "Running server on " << buf << std::endl;
        std::cout << "Server listening on port " << 25569 << std::endl;

        sockaddr_in addr;
        unsigned int addrlen = sizeof(addr);
        while (true)
        {
            int res = accept(server_fd, (sockaddr*)&addr, &addrlen);
            inet_ntop(AF_INET, (const void *)&(addr.sin_addr.s_addr), buf, 50);
            std::cout << "Accepted connection from " << buf << std::endl;
        }
    }

private:
    int server_fd;
    int port;
};

#endif
