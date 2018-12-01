#include <stdio.h>
#include "Multiplayer.h"

int main(int argc, char *argv[]) {
    char ip[12];
    bool keys[] = {0, 1, 0, 1, 0, 1, 1, 1, 0};

    server_initialise();

    switch(argv[1][0]) {
        case 's': // Server
            set_server();


            r_receive(keys);

            break;

        case 'c': // Client
            printf("ip: ");
            scanf("%s", ip);
            
            set_client(ip);

            // keys = {0, 1, 0, 1, 0, 1, 0, 1, 0};

            r_send(keys);

            break;

        default:
            printf("Please inform 's' or 'c' to operate as server or client\n");
            break;
    }
    

    

    return 0;
}