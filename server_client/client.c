//
// Created by parthka on 18/6/22.
//

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char** args) {

    int PORT = 8080;
    printf("Enter The Port Number: ");
    scanf("%d", &PORT);
    getchar();

    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
                "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((client_fd
                 = connect(sock, (struct sockaddr*)&serv_addr,
                           sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    char buff[200];
    while (1) {


        bzero(buff, sizeof buff);
        int n = 0;
        char t;
        while ((t = getchar()) != '\n') {
            buff[n] = t;
            n++;
        }

        write(sock, buff, strlen(buff));
        read(sock, buffer, 1024);
        printf("%s\n", buffer);

        if(strcmp(buff, "bye") == 0) {
            close(sock);
            return 0;
        }

    }
    return 0;
}

