#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

int main() {
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (sockaddr*)&server, sizeof(server));

    listen(sockfd, 5);

    cout << "Waiting" << endl;

    sockaddr_in client{};
    socklen_t size = sizeof(client);

    int new_fd = accept(sockfd, (sockaddr*)&client, &size);

    char buffer[100] = {0};

    recv(new_fd, buffer, sizeof(buffer), 0);

    cout << "Received: " << buffer << endl;

    // Reply for the recieved message
    string reply = "Welcome";
    send(new_fd, reply.c_str(), reply.length(), 0);

    close(new_fd);
    close(sockfd);

    return 0;
}
