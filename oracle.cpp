#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "build.h"

using namespace std;

struct __attribute__((packed)) messages {
    uint32_t ip_addr;
    uint16_t udp_port;
};

struct __attribute__((packed)) link_msg {
    char node_alpha;
    uint32_t ip_addr;
    uint16_t udp_port;
    int32_t cost;
};

const int ORACLE_PORT = 5000;
const unsigned char LINK_STATE = 2;
const int TUPLE_SIZE = sizeof(link_msg);
const int MAX_MESSAGE_SIZE = 2 + MAX_NODES * TUPLE_SIZE;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <config-file>\n", argv[0]);
        return 1;
    }

    int adj[MAX_NODES][MAX_NODES];
    int nodeCount;

    if (!buildMatrix(argv[1], adj, nodeCount)) {
        printf("Could not read configuration file\n");
        return 1;
    }

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(ORACLE_PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket,(sockaddr*)&server,sizeof(server));

    listen(serverSocket, MAX_NODES);

    int virtual_fd[MAX_NODES];
    messages virtual_info[MAX_NODES];
    
    for (int i = 0; i < nodeCount; i++) {
        sockaddr_in client{};
        socklen_t clientSize = sizeof(client);

        virtual_fd[i] = accept(serverSocket, (sockaddr*)&client, &clientSize);

        if (virtual_fd[i] < 0) {
            printf("Could not accept connection\n");
            close(serverSocket);
            return 1;
        }

        int received = 0;

        while (received < sizeof(messages)) {
            int count = recv(virtual_fd[i],((char*)&virtual_info[i]) + received,sizeof(messages) - received,0);

            if (count <= 0) {
                printf("Could not receive CONNECT message\n");
                close(virtual_fd[i]);
                close(serverSocket);
                return 1;
            }
            received += count;
        }

        printf("Node %c connected\n", 'A' + i);
    }

    
    for (int node = 0; node < nodeCount; node++) {
        unsigned char message[MAX_MESSAGE_SIZE];

        message[0] = LINK_STATE;
        message[1] = 0;

        int tupleCount = 0;

        for (int other = 0; other < nodeCount; other++) {
            if (other != node && adj[node][other] == -1) {
                continue;
            }

            link_msg tuple{};

            tuple.node_alpha = 'A' + other;
            tuple.ip_addr = virtual_info[other].ip_addr;
            tuple.udp_port = virtual_info[other].udp_port;
            tuple.cost = htonl(adj[node][other]);

            int position = 2 + tupleCount * TUPLE_SIZE;

            memcpy(message + position,&tuple,sizeof(link_msg));
            tupleCount++;
        }

        message[1] = tupleCount;

        int messageSize = 2 + tupleCount * TUPLE_SIZE;

        int sent = 0;

        while (sent < messageSize) {
            int count = send(virtual_fd[node],message + sent,messageSize - sent,0);

            if (count <= 0) {
                printf("Could not send LINK-STATE to node %c\n",'A' + node);
                break;
            }

            sent += count;
        }

        printf("LINK-STATE sent to node %c\n", 'A' + node);
        close(virtual_fd[node]);
    }

    close(serverSocket);

    return 0;
}