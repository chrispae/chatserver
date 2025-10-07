#ifndef INCLUDE_CHATSERVER_SERVER_HPP
#define INCLUDE_CHATSERVER_SERVER_HPP

#include <iostream>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <vector>
#include <thread>

class Server
{
public:
    Server() = default;
    ~Server() = default;

public:
    void start(std::string &address, int port)
    {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port); // convert to network byte order (little-endian to big-endian)
        server_addr.sin_addr.s_addr = inet_addr(address.c_str());

        const sockaddr *server_sock = (struct sockaddr *)&server_addr;
        bind(server_fd, server_sock, sizeof(*server_sock));
        listen(server_fd, 1); // single-threaded server, max 1 client in the queue

        char readable_ip_buf[20];
        inet_ntop(AF_INET, (const void *)&(server_addr.sin_addr.s_addr), readable_ip_buf, sizeof(readable_ip_buf));

        std::cout << "Running server on " << readable_ip_buf << std::endl;
        std::cout << "Server listening on port " << port << std::endl;

        sockaddr_in addr;
        unsigned int addrlen = sizeof(addr);
        while (true)
        {
            int connection_fd = accept(server_fd, (sockaddr *)&addr, &addrlen);
            std::cout << "New connection on " << connection_fd << std::endl;

            auto handle_connection = [&]()
            {
                if (connection_fd < 0)
                {
                    std::cerr << "Error on accept" << std::endl;
                    return;
                }

                inet_ntop(AF_INET, (const void *)&(addr.sin_addr.s_addr), readable_ip_buf, sizeof(readable_ip_buf));
                std::cout << "Accepted connection from " << readable_ip_buf << std::endl;

                std::vector<char> recvbuffer(512);
                std::vector<char> sendbuffer(512);

                while (true)
                {
                    // call of read is blocking until data is available
                    int n = read(connection_fd, recvbuffer.data(), recvbuffer.size());
                    if (n < 0)
                    {
                        std::cerr << "Error reading from socket" << std::endl;
                        close(connection_fd);
                        break;
                    }

                    if (n == 0)
                    {
                        std::cout << "Client disconnected" << std::endl;
                        close(connection_fd);
                        break;
                    }

                    std::cout << "Received message: " << ": " << std::string(recvbuffer.data(), n) << std::endl;

                    std::memcpy(sendbuffer.data(), recvbuffer.data(), n);
                    sendbuffer[n] = '\0';

                    int m = write(connection_fd, sendbuffer.data(), n + 1);
                }
            };

            std::thread connection_thread(handle_connection);
            connection_thread.detach(); // detach the thread to allow it to run independently
        }
    }

private:
    int server_fd;
    int port;
};

#endif
