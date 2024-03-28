#include <stdio.h>
#include <stdlib.h>
#include "main.h"

char username[20];
char secret[128];
char display_name[20];
char channel_id[20];

//************************* CONNECT *************************//
int establishing_connection(int client_socket, char *addr)
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(S_PORT);
    inet_pton(AF_INET, addr, &server_addr.sin_addr);

    return connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
}

//************************* DISCONNECT *************************//
void disconnect_err(int client_socket)
{
    shutdown(client_socket, SHUT_RDWR);
    exit(1);
}

//************************* SEND *************************//
int send_message(int client_socket, char *message, char *state)
{
    char message_check[128];
    scanf(" %[^\n]", message_check);

    if(!strcmp(state, "start") || !strcmp(state, "auth"))
    {
        char *word = strtok(message_check, " ");

        if(!strcmp(word, "/auth"))
        {
            strcpy(state, "auth"); //TODO pripojeni do default
            //printf("%s\n", state);
            strcpy(username, strtok(NULL, " "));
            strcpy(secret, strtok(NULL, " "));
            strcpy(display_name, strtok(NULL, " "));
            char *more_arguments = strtok(NULL, " ");
            if(!strcmp(username, "") || !strcmp(secret, "") || !strcmp(display_name, "") || more_arguments != NULL) 
            {
                //TODO spatne
                exit(1);
            }
            //TODO bud dat state = open, nebo az prijde potvrzeni o pripojeni
            sprintf(message, "AUTH %s AS %s USING %s\r\n", username, display_name, secret);
            //printf("%s\n", message);
        }
        else
        {
            printf("nejdrive se prihlas pomoci /auth\n");
            //TODO nesmi? nebo ERR
        }
    }
    else if(!strcmp(state, "open"))
    {
        char *word = strtok(message_check, " ");

        if(!strcmp(word, "/join"))
        {
            strcpy(channel_id, strtok(NULL, " "));
            char *more_arguments = strtok(NULL, " ");
            if(!strcmp(channel_id, "") || more_arguments != NULL)
            {
                exit(1);
            }

            sprintf(message, "JOIN %s AS %s\r\n", channel_id, display_name);
        }
        else if(!strcmp(word, "/msg"))
        {

        }
        else
        {
            printf("zkus neco jineho\n");
        }

    }
    else
    {

    }

    int bytes_tx = send(client_socket, message, strlen(message), 0);
    return bytes_tx;
}

//************************* RECEIVE *************************//
void receive_message(int client_socket, char *message, char *state)
{
    int bytes_rx = recv(client_socket, message, 128, 0);
    //printf("%i\n",bytes_rx);
    if(bytes_rx < 0)
    {
        disconnect_err(client_socket);
    }

    if(!strcmp(state, "auth"))
    {
        //TODO kladna zprava -> state = open, jinak nic
        char *reply = strtok(message, " ");
        if(!strcmp(reply, "Success:"))
        {
            strcpy(state, "open");
            //TODO kladna
        }
        else if(!strcmp(reply, "Failure:"))
        {
            //TODO !kladna
        }
        else
        {
            //nejaka jina zprava??
        }
    }
}

void message_dispose(char *message)
{
    for(int i = 0; message[i] != '\0'; i++)
    {
        message[i] = '\0';
    }
}

//************************* TCP_CLIENT *************************//
int tcp_client(char *addr)
{
    char state[10] = "start";
    //char *action = "auth";

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1)
    {
        perror("ERROR: socket");
        exit(1);
    }
    
    if(establishing_connection(client_socket, addr))
    {
        perror("ERROR: connect");
        exit(1);
    }

    int flag = 1;
    while(flag)
    {
        /*************** send ***************/
        char *message = malloc(128 * sizeof(char));
        if(send_message(client_socket, message, state) < 0)
        {
            disconnect_err(client_socket);
        }  
        //printf("%s\n", state);
        message_dispose(message);

        /*************** receive ***************/
        receive_message(client_socket, message, state);
        printf("%s\n", message); 

        message_dispose(message);
    }
    return 0;
}

//************************* MAIN *************************//
int main(int argc, char *argv[]) 
{
    //kontrola argumentu
    if(argc < 5)
    {
        perror("ERROR: arguments ./main -t [tcp/udp] -s [ip_addr]");
        exit(1); //malo argumentu
    }

    if(argc < 5 || strcmp(argv[1], "-t") || (strcmp(argv[2], "tcp") && strcmp(argv[2], "udp")) || strcmp(argv[3], "-s")) //TODO kontrola adresy
    {
        perror("ERROR: arguments ./main -t [tcp/udp] -s [ip_addr]");
        exit(1);
    }

    char *protocol = argv[2];
    char *addr = argv[4];

    if (!strcmp(protocol, "tcp"))
    {
        tcp_client(addr);
    }
    
    return 0;
}