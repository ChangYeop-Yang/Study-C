//
//  Header.h
//  Algorithm
//
//  Created by 양창엽 on 2017. 6. 15..
//  Copyright © 2017년 양창엽. All rights reserved.
//

#ifndef Header_h
#define Header_h

/* Include Header */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
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

/* Typedef */
typedef struct sockaddr_in SOCK_IN;

/* Struct */
typedef struct users
{
    /* Integer */
    int nVenID;
    int nVersion;
    int nFileDiscript;
    
    /* String */
    char sID[BUFSIZ];
    char sPassword[BUFSIZ];
} USER;

#endif /* Header_h */
