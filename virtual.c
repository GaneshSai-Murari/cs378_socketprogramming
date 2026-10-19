#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <ON_IP> <UDP_PORT>\n", argv[0]);
        return 1;
    }

    int udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp < 0) {
        perror("socket creation failed");
        return 1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) != 1) {
        printf("Invalid IP address\n");
        close(udp);
        return 1;
    }

    if (bind(udp, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(udp);
        return 1;
    }

    printf("UDP socket bound successfully\n");

    close(udp);
    return 0;
}