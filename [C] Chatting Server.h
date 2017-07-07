/* Include Header */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

/* MACRO */
#define MALLOC(X, Y) (X*)malloc(sizeof(X) * Y)

/* FLAG ERROR Define */
#define FLAG_IP_ERROR 10
#define FLAG_BIND_ERROR 12
#define FLAG_LISTEN_ERROR 13
#define FLAG_SOCKET_ERROR 14
#define FLAG_CONNECT_ERROR 15

/* FLAG DATA Define */
#define FLAG_TRUE 1
#define FLAG_FALSE 0
#define FLAG_USER_NUM 6

/* MEMORY Define */
#define BUF_MSG 1024
#define BUF_REG 255

/* Typedef */
typedef struct sockaddr_in SOCK_IN;

/* Struct */
typedef struct users
{
    /* Short */
    short sLoginState;
    
    /* Integer */
    int nVenID[FLAG_USER_NUM];
    int nVersion;
    int nFileDiscript;
    
    /* String */
    char sID[BUF_REG];
    char sPassword[BUF_REG];
} USER;
