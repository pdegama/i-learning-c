//
// Created by parthka on 18/6/22.
//

#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "fcntl.h"
#include "sys/epoll.h"

int main(int argc, char** args){

    int PORT = 8080;
    printf("Enter The Port Number: ");
    scanf("%d", &PORT);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    int server_fd = socket(address.sin_family, SOCK_STREAM, 0);
    if (server_fd == 0){
        printf("Socket Make Error...\n");
        exit(0);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof opt);

    int server_bind = bind(server_fd, (struct sockaddr*)&address, sizeof address);
    if (server_bind < 0) {
        printf("Bind Error...\n");
        exit(0);
    }

    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

    if (listen(server_fd, 215) < 0) {
        printf("Listen Error...\n");
        exit(0);
    }

    int epfd = epoll_create1(0);

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = server_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

    // printf("Server Listen On http://localhost:%d\n", PORT);

    int addr_len = sizeof address;

    char buff[1024] = {};
    char buff2[100] = {};

    while (1) {
        struct epoll_event evf[1];
        int nv = epoll_wait(epfd, evf, 1, -1);
        for (int j = 0; j < nv; j++) {
            if (evf[j].data.fd == server_fd) {
                int sock = 0;
                do {
                    sock = accept(evf[j].data.fd, (struct sockaddr*)&address, (socklen_t*)&addr_len);
                    if (sock > 0) {
                        int flags = fcntl(sock, F_GETFL, 0);
                        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
                        struct epoll_event ev1;
                        ev1.events = EPOLLIN | EPOLLET;
                        ev1.data.fd = sock;
                        epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev1);
                    }
                } while (sock > 0);
            } else {
                bzero(buff, sizeof buff);

                read(evf[j].data.fd, buff, 1024);
                printf("%d: %s\n", evf[j].data.fd, buff);

                if(strcmp(buff, "bye") == 0) {
                    close(evf[j].data.fd);
                } else {
                    sprintf(buff2, "Length: %lu", strlen(buff));
                    write(evf[j].data.fd, buff2, strlen(buff2));
                }

            }
        }
    }

    return 0;
}
