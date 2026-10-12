#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (sockaddr*)&server, sizeof(server));

    string msg = "Hello";
    send(sockfd, msg.c_str(), msg.length(), 0);

    cout << "Sent: " << msg << endl;

    char buffer[100] = {0};
    recv(sockfd, buffer, sizeof(buffer), 0);
    cout<< "Received mssg from server : " <<buffer<<endl; 

    close(sockfd);

    return 0;
}
