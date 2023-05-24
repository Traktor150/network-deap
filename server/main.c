// gcc -c -o main.o main.c
// gcc -o server.exe main.o -lws2_32

#include <stdio.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>

#include <winsock2.h>

void main(void) {
    printf("Hello World!\n");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        printf("Error code: %d\n", WSAGetLastError());
        return;
    }
    printf("WSAStartup succeeded.\n");

    SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("socket failed.\n");
        printf("Error code: %d\n", WSAGetLastError());
        return;
    }
    printf("socket succeeded.\n");

    // start doing stuff here

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(sock, (const struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        printf("bind failed.\n");
        printf("Error code: %d\n", WSAGetLastError());
        return;
    }

    if (listen(sock, 5) == -1)
    {
        printf("listen failed.\n");
        printf("Error code: %d\n", WSAGetLastError());
        return;
    }

    // on port 8080
    printf("listening on port 8080...\n");

    int running = 1;

    while (running)
    {
        struct sockaddr_in client_addr = {0};
        int client_addr_len = sizeof(client_addr);
        SOCKET client_sock = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock == -1)
        {
            printf("accept failed.\n");
            printf("Error code: %d\n", WSAGetLastError());
            return;
        }

        char buf[1024];
        int len = recv(client_sock, buf, sizeof(buf), 0);
        buf[len] = '\0';
        printf("received: %s\n", buf);

        char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World!</h1></body></html>";
        send(client_sock, response, strlen(response), 0);

        closesocket(client_sock);

    }
    



    closesocket(sock);
    WSACleanup();
    printf("WSACleanup succeeded.\n");
}
