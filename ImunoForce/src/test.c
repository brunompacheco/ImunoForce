#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Multiplayer.h"

int main(int argc, char *argv[]) {
    char ip[12];
    bool keys[] = {0, 1, 0, 1, 0, 1, 1, 1, 0};
    char msg[98];
    char seqnum = 1;


    server_initialise();

    switch(argv[1][0]) {
        case 's': // Server
            set_server();


            // r_receive(keys);
            // sleep(1);
            // r_send(keys);
            safe_receive(seqnum, msg, 98);

            break;

        case 'c': // Client
            strcpy(ip, argv[2]);

            printf("ip: %s\n", ip);

            // scanf("%s", ip);
            
            set_client(ip);

            // r_send(keys);
            // r_receive(keys);

            strcpy(msg, "asdfasdfasdf");

            safe_send(seqnum, msg, 98);

            break;

        default:
            printf("Please inform 's' or 'c' to operate as server or client\n");
            break;
    }
    

    

    return 0;
}