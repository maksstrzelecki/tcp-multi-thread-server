#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int main() {
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);


    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        return 1;
    }
    std::cout << "Server listening on port 8080\n";



    sockaddr_in client{};
    socklen_t client_size = sizeof(client);

    int client_fd = accept(server_fd, (sockaddr*)&client, &client_size);

    if (client_fd < 0) {
        perror("accept");
        return 1;
    }

    std::cout << "Client connected\n";



    char buffer[1024];
    while (true) {

        memset(buffer, 0, sizeof(buffer));

        int bytes;
        if ((bytes = recv(client_fd, buffer, sizeof(buffer), 0)) <= 0) break;

        std::cout << "Client: " << buffer << "\n";

        send(client_fd, buffer, bytes, 0);
    }
    close(client_fd);
    close(server_fd);
    return 0;
}