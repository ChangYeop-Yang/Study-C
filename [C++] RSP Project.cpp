#include <ctime>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;

/* Namespace */
namespace ERROR_FLAG
{
    const static int FLAG_IP_ERROR = 1;
    const static int FLAG_SOCKET_ERROR = 2;
    const static int FLAG_CONNECT_ERROR = 3;
}
namespace GAME_FLAG
{
    const static int FLAG_SCISSORS = 1;
    const static int FLAG_ROCK = 2;
    const static int FLAG_PAPER = 3;
    
    const static int FLAG_MOD_NUM = 3;
    
    const static int FLAG_MOD_WIN = 4;
    const static int FLAG_MOD_DRAW = 5;
    const static int FLAG_MOD_LOSE = 6;
    const static int FLAG_MOD_START = 7;
}

/* Global Vaiable */
vector<int> cWinVec;

/* Declear Inline Function */
inline bool sendMsg(const int sModFlag, const int nSocket);

/* Declear Function */
int selectDynamic(const int sType, const int sPrivious, int nSock);
int selectRandomFLAG(const int nMaximum);
void * recvMsg(void *arg);

int main(int argc, char * argv[])
{
    try
    {
        if (argc != 3) { throw ERROR_FLAG::FLAG_IP_ERROR; }
        
        pthread_t rcv_thread;
        
        /* Integer */
        int nSock = socket(PF_INET, SOCK_STREAM, 0);
        if (nSock < 0) { throw ERROR_FLAG::FLAG_SOCKET_ERROR; }
        
        /* Struct */
        struct sockaddr_in sServ_Addr;
        memset(&sServ_Addr, 0, sizeof(sServ_Addr));
        
        sServ_Addr.sin_family = AF_INET;
        sServ_Addr.sin_addr.s_addr = inet_addr(argv[1]);
        sServ_Addr.sin_port = htons(atoi(argv[2]));
        
        if (connect(nSock, (struct sockaddr *) &sServ_Addr, sizeof(sServ_Addr)) == -1)
        { throw ERROR_FLAG::FLAG_CONNECT_ERROR; }
        
        pthread_create(&rcv_thread, NULL, recvMsg, (void*)&nSock);
        
        pthread_join(rcv_thread, NULL);
    }
    catch (const int sMsgNum)
    {
        switch (sMsgNum)
        {
            case (ERROR_FLAG::FLAG_IP_ERROR) : { cout << "Using Current IP Address Error!!!" << endl; break; }
            case (ERROR_FLAG::FLAG_SOCKET_ERROR) : { cout << "Create Socket Error!!!" << endl; break; }
            case (ERROR_FLAG::FLAG_CONNECT_ERROR) : { cout << "Connect Server Error!!!" << endl; break; }
            default : { cout << "Network UnKnown Error!!!" << endl; break; }
        } exit(false);
    }
}

/* Define Inline Function */
inline bool sendMsg(const int sModFlag, const int nSocket)
{
    char * anMsg = new char[2];
    sprintf(anMsg, "%d", sModFlag);
    return write(nSocket, anMsg, strlen(anMsg)) != EOF ? true : false;
}

/* Define Function */
void * recvMsg(void * arg)
{
    int nSock = *((int*)arg);
    unsigned int sPrivious = GAME_FLAG::FLAG_ROCK;
    
    while (true)
    {
        /* Char */
        char * acBuffer = new char[BUFSIZ];
        memset(acBuffer, 0, BUFSIZ);
        read(nSock, acBuffer, BUFSIZ);
        
        switch (acBuffer[0])
        {
                /* START */
            case ('S') :
            {
                sPrivious = selectDynamic(GAME_FLAG::FLAG_MOD_START, sPrivious, nSock);
                break;
            }
                /* WIN */
            case ('W') :
            {
                sPrivious = selectDynamic(GAME_FLAG::FLAG_MOD_WIN, sPrivious, nSock);
                break;
            }
                /* DRAW */
            case ('D') :
            {
                sPrivious = selectDynamic(GAME_FLAG::FLAG_MOD_DRAW, sPrivious, nSock);
                break;
            }
                /* LOSE */
            case ('L') :
            {
                sPrivious = selectDynamic(GAME_FLAG::FLAG_MOD_LOSE, sPrivious, nSock);
                break;
            }
                /* END */
            case ('E') :
            {
                cWinVec.clear();
                close(nSock); exit(true);
            }
        }
        
        cout << acBuffer << endl;
    }
}
int selectRandomFLAG(const int nMaximum)
{
    srand((unsigned int)time(NULL));
    const unsigned int sRandomFlag = rand() % nMaximum;
    
    return cWinVec[sRandomFlag];
}
int selectDynamic(const int sType, const int sPrivious, int nSock)
{
    /* Short */
    unsigned int sSelectFlag = GAME_FLAG::FLAG_ROCK;
    
    switch (sType)
    {
        case (GAME_FLAG::FLAG_MOD_WIN) :
        {
            cWinVec.push_back(sPrivious);
            sendMsg(sPrivious, nSock);
            sSelectFlag = sPrivious; break;
        }
        case (GAME_FLAG::FLAG_MOD_DRAW) :
        {
            sSelectFlag = selectRandomFLAG(GAME_FLAG::FLAG_MOD_NUM);
            sendMsg(sSelectFlag, nSock);
            break;
        }
        case (GAME_FLAG::FLAG_MOD_LOSE) :
        {
            switch (sPrivious)
            {
                case (GAME_FLAG::FLAG_ROCK) : { sSelectFlag = GAME_FLAG::FLAG_PAPER; break; }
                case (GAME_FLAG::FLAG_SCISSORS) : { sSelectFlag = GAME_FLAG::FLAG_ROCK; break; }
                case (GAME_FLAG::FLAG_PAPER) : { sSelectFlag = GAME_FLAG::FLAG_SCISSORS; break; }
            }
            sendMsg(sSelectFlag, nSock); break;
        }
        case (GAME_FLAG::FLAG_MOD_START) :
        {
            /* Insert Value */
            cWinVec.push_back(GAME_FLAG::FLAG_SCISSORS);
            cWinVec.push_back(GAME_FLAG::FLAG_ROCK);
            cWinVec.push_back(GAME_FLAG::FLAG_PAPER);
            
            sSelectFlag = selectRandomFLAG(GAME_FLAG::FLAG_MOD_NUM);
            sendMsg(sSelectFlag, nSock);
            break;
        }
    }
    
    return sSelectFlag;
}
