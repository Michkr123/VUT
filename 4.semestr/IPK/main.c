#include <stdio.h>
#include <stdlib.h>
#include "main.h"


int establishing_connection(int client_socket)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(S_PORT);
    inet_pton(AF_INET, SERVER, &server_addr.sin_addr);

    int connect_check = connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    return connect_check;
}

int send_message(int client_socket, char *message)
{
    scanf("%s", message);
    int bytes_tx = send(client_socket, message, strlen(message), 0);
    return bytes_tx;
}

int receive_message(int client_socket, char *message)
{
    int bytes_rx = recv(client_socket, message, 100, 0);
    return bytes_rx;
}

void disconnect_err(int client_socket)
{
    shutdown(client_socket, SHUT_RDWR);
    exit(1);
}

//************************* MAIN *************************//
int main(int argc, char *argv[]) 
{

    // kontrola argumetu poganek
    
    
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1)
    {
        perror("ERROR: socket");
        exit(1);
    }
    
    if(establishing_connection(client_socket))
    {
        perror("ERROR: connect");
        exit(1);
    }

    int flag = 1;
    while(flag)
    {
        char *message = malloc(100 * sizeof(char));
        if(send_message(client_socket, message) < 0)
        {
            disconnect_err(client_socket);
        }  
        if(receive_message(client_socket, message) < 0)
        {
            disconnect_err(client_socket);
        }
        printf(message);   
    }
    
    return 0;
}