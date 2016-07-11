#include <libgonc/gonc_array.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "gonm.h"
#include "gonm_parent.h"
#include "gonm_child.h"

void gonm_start(size_t child_count, struct gonm_socket_array* child_socket_array)
{
    int socket_pair[2];
    if(child_count == 0)
    {
        struct sigaction sigchld_action;
        sigchld_action.sa_handler = gonm_parent_sigchld_handler;
        sigemptyset(&sigchld_action.sa_mask);
        sigchld_action.sa_flags = 0;
        sigaction(SIGCHLD, &sigchld_action, NULL);
        gonm_parent_start(child_socket_array);
        //gonm_parent_stop();
        return;
    }
    socketpair(AF_UNIX, SOCK_STREAM, 0, socket_pair);
    if(fork() != 0)
    {
       close(socket_pair[1]);
       GONC_ARRAY_SET(child_socket_array, child_socket_array->size, socket_pair[0]);
       gonm_start(child_count - 1, child_socket_array);
    }
    else
    {
        close(socket_pair[0]);
        gonm_child_start(socket_pair[1]);
    }
}

int main(int argc, char* argv[])
{
    struct gonm_socket_array* child_socket_array = malloc(sizeof(struct gonm_socket_array));
    GONC_ARRAY_INIT(child_socket_array, int, 3);
    gonm_start(3, child_socket_array);
    return 0;
}