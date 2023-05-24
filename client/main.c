// gcc -c -o main.o main.c
// gcc -o client.exe main.o -lws2_32

#include <stdio.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>

#include <winsock2.h>

int main(void) {
    printf("Hello World!\n");

    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }
    printf("Socket created.\n");


    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(8080);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("connect error");
        return 1;
    }

    char buf[1024] = {"POST api/1.0/pass/\"hidden\" / HTTP/1.1\r\nHost:127.0.0.1:8080\r\nContent-type:application/json\r\nAccept:application/json\r\n\r\n{\"name\":\"William\"}"};

    if (send(sock, buf, strlen(buf), 0) < 0) {
        printf("Send failed");
        return 1;
    }

    printf("Data sent\n");

    char buf_rec[1024];
    int recv_size = recv(sock, buf_rec, 1024, 0);
    buf_rec[recv_size] = '\0';
    printf("Received: %s\n", buf_rec);
    
    closesocket(sock);

    WSACleanup();
}