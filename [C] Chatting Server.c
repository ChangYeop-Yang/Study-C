#include "Header.h"

/* Global Variable */
pthread_mutex_t pMutx;
int nCurrent_User_Num = 0;
int FLAG_GAME[2] = { 0,0 };      //Game User FileDiscription
int gameResult[3] = { 0, 0, 0};      //win1, win2,  totalgamenum
short FLAG_GSTART = FLAG_FALSE;
char gameBuF[2][BUFSIZ];
int gameBufSz[2] = { 0,0 };

int anUser_Name_Flag[FLAG_USER_NUM];
USER asUser_Current[FLAG_USER_NUM];

/* Declear Function */
int select_Position();
void * handle_Read(void * arg);
void handle_Error(const short sError);
void send_Msg(const int nSocket, const ssize_t nLength, const char * sMsg);
void Whisper_Member(const char * sBuf, const int nSock);
char * Select_Nickname(const int nSock);
void join_Member(const char * sBuf, const int nSock, const int msgSz);
void login_Member(const char * sBuf, const int nSock, const int msgSz);
void game(const char * sBuf, const int nSock, const int msgSz);
//void Game_Rule(const char *sBuf, const int nSock, const int msgSz);
void ban_Member(const char *sBuf, const int nSock, const int msgSz);
USER * insert_UserData(const int nSocket, const int nVersion, const char * sID, const char * sPasswd);

int main(int argc, char * argv[])
{
    if (argc != 2) { handle_Error(FLAG_IP_ERROR); }
    
    /* Pthread */
    pthread_t read_id;
    pthread_mutex_init(&pMutx, NULL);
    
    /* Integer */
    int nClnt_Sock = 0;
    int nServ_Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    
    /* struct sockaddr_in */
    SOCK_IN sServ_Adr, sClnt_Adr;
    
    /* sockaddr_in Initialization */
    memset(&sServ_Adr, 0, sizeof(sServ_Adr));
    sServ_Adr.sin_family = AF_INET;
    sServ_Adr.sin_addr.s_addr = htonl(INADDR_ANY);
    sServ_Adr.sin_port = htons(atoi(argv[1]));
    
    /* Socket Bind & Listen */
    if (bind(nServ_Sock, (struct sockaddr *) &sServ_Adr, sizeof(sServ_Adr)) == EOF) { handle_Error(FLAG_BIND_ERROR); }
    if (listen(nServ_Sock, 10) == EOF) { handle_Error(FLAG_LISTEN_ERROR); }
    memset(asUser_Current, 0, sizeof(USER) * 6);
    while (FLAG_TRUE)
    {
        socklen_t unClnt_Adr_Sz = sizeof(sClnt_Adr);
        if ((nClnt_Sock = accept(nServ_Sock, (struct sockaddr *)&sClnt_Adr, &unClnt_Adr_Sz)) == EOF) { break; }
        
        const int nPosition = select_Position();
        USER * sTemp_User = insert_UserData(nClnt_Sock, nPosition, "NoUser", "NoUser");
        char * sTemp_String = (char*)malloc(sizeof(char) * BUFSIZ);
        
        asUser_Current[nPosition] = *sTemp_User;
        sprintf(sTemp_String, "Connection Success, Your ID is User%d\n", sTemp_User->nVersion + 1);
        send_Msg(sTemp_User->nFileDiscript, strlen(sTemp_String), sTemp_String);
        
        /* Memory Reset */
        free(sTemp_User);
        free(sTemp_String);
        
        pthread_create(&read_id, NULL, handle_Read, (void*)&nClnt_Sock);
        pthread_detach(read_id);
        
        /* Printf Current Total User Number */
        printf("New client is connected - Number of total client is %d\n", ++nCurrent_User_Num);
    }
    
    close(nServ_Sock);
    return 0;
}

/* Define Function */
int select_Position()
{
    /* Integer */
    int nPosition = 0;
    
    for (nPosition = 0; anUser_Name_Flag[nPosition]; nPosition++);
    anUser_Name_Flag[nPosition] = FLAG_TRUE;
    
    return nPosition;
}
void send_Msg(const int nSocket, const ssize_t nLength, const char * sMsg)
{
    write(nSocket, sMsg, nLength);
}
void * handle_Read(void * arg)
{
    /* Integer */
    ssize_t nStr_Len = 0;
    ssize_t nPosition = 0;
    int nClnt_Sock = *((int*)arg);
    short sendTrue = FLAG_TRUE;
    
    /* Char */
    char sMsg[BUFSIZ];
    memset(sMsg, 0, sizeof(sMsg));
    
    while ((nStr_Len = read(nClnt_Sock, sMsg, sizeof(sMsg))) != 0)
    {
        if (sMsg[0] == '/')
        {
            switch (sMsg[1])
            {
                    /* Register */
                case ('r'): { join_Member(sMsg, nClnt_Sock, nStr_Len);   break; }
                    /* Login */
                case ('l'): {  login_Member(sMsg, nClnt_Sock, nStr_Len);  break; }
                    /* Game */
                case ('g'): { if (FLAG_GSTART == FLAG_FALSE) { game(sMsg, nClnt_Sock, nStr_Len);  break; }
                else {
                    char *eMsg = "Game is alreay start\n";
                    send_Msg(nClnt_Sock, sizeof("Game is alreay start\n"), eMsg);
                    break;
                }
                }
                    /* Ban */
                case ('b'): {    ban_Member(sMsg, nClnt_Sock, nStr_Len);  break; }
                    /* Whisper */
                case ('w'): { Whisper_Member(sMsg, nClnt_Sock); ; break; }
                    
                default: { send_Msg(nClnt_Sock, sizeof(char) * 18, "Not correct Order\n"); break; }
            }
        }
        else
        {
            /* Select User */
            for (size_t ii = 0; ii < FLAG_USER_NUM; ii++)
            {
                if (asUser_Current[ii].nFileDiscript == nClnt_Sock) { nPosition = ii; }
            }
            /* UnEable Login User */
            if (asUser_Current[nPosition].sLoginState == FLAG_FALSE)
            {
                send_Msg(asUser_Current[nPosition].nFileDiscript, nStr_Len, sMsg);
            }
            ////////////Playing game Users0
            
            /* Eable Login User */
            else
            {
                /* Send User Message */
                for (size_t ii = 0; ii < nCurrent_User_Num; ii++)
                {
                    /* UnRegistered User Or UnEable Login User and User are Not Playing Game Or Ready Game */
                    if (asUser_Current[ii].nFileDiscript != nClnt_Sock && asUser_Current[ii].nFileDiscript != 0 && asUser_Current[ii].nFileDiscript!= FLAG_GAME[0] && asUser_Current[ii].nFileDiscript != FLAG_GAME[1])
                    {
                        ///////////Not in Ban-list
                        for (int a = 0; a < FLAG_USER_NUM; a++)
                        {
                            if (asUser_Current[ii].nVenID[a] == nClnt_Sock)
                            {
                                sendTrue = FLAG_FALSE;
                                break;
                            }
                        }
                        /* Not Ban && (Registered User Or Eable Login User)*/
                        if (sendTrue == FLAG_TRUE && asUser_Current[ii].sLoginState == FLAG_TRUE)
                        {
                            /* Send User Message*/
                            char * sNickName = MALLOC(char, BUFSIZ);
                            sprintf(sNickName, "%s)%s", asUser_Current[nPosition].sID, sMsg);
                            send_Msg(asUser_Current[ii].nFileDiscript, strlen(sNickName), sNickName);
                            free(sNickName);
                        }
                    }
                    
                    
                    sendTrue = FLAG_TRUE;
                }
            }
        }
        memset(sMsg, 0, sizeof(sMsg));
    }
    
    pthread_mutex_lock(&pMutx);
    {
        /* Remove Disconnected Client */
        for (int ii = 0; ii < nCurrent_User_Num; ii++)
        {
            if (nClnt_Sock == asUser_Current[ii].nFileDiscript)
            {
                /* Printf Current Total User Number */
                printf("User %d is disconnected - Number of total client is %d\n", asUser_Current[ii].nVersion + 1, --nCurrent_User_Num);
                
                /* Disenable UserID */
                anUser_Name_Flag[asUser_Current[ii].nVersion] = FLAG_FALSE;
                
                while (ii++ < nCurrent_User_Num) { asUser_Current[ii] = asUser_Current[ii + 1]; }
                break;
            }
        }
    }
    pthread_mutex_unlock(&pMutx);
    
    
    
    /* Reset Memory */
    close(nClnt_Sock);
    return NULL;
}
void handle_Error(const short sError)
{
    switch (sError)
    {
        case (FLAG_IP_ERROR): { printf("Using Current IP Address Error!!!\n"); break; }
        case (FLAG_SOCKET_ERROR): { printf("Create Socket Error!!!\n"); break; }
        case (FLAG_CONNECT_ERROR): { printf("Connect Server Error!!!\n"); break; }
        case (FLAG_BIND_ERROR): { printf("Bind Socket Error!!!\n"); break; }
        case (FLAG_LISTEN_ERROR): { printf("Listen Socket Error!!!\n"); break; }
        default: { printf("Network UnKnown Error!!!\n"); break; }
    }
}
char * Select_Nickname(const int nSock)
{
    /* String */
    char * sNickName = MALLOC(char, BUFSIZ);
    
    /* Select User Nickname */
    for (int ii = 0; ii < FLAG_USER_NUM; ii++)
    {
        /* Select User Nickname Success */
        if (nSock == asUser_Current[ii].nFileDiscript)
        {
            sprintf(sNickName, "%s)", asUser_Current[ii].sID);
        }
    }
    
    return sNickName;
}

void ban_Member(const char *sBuf, const int nSock,const int msgSz)
{
    char  sID_Temp[BUF_REG];
    USER * sbUser_Temp = MALLOC(USER, 1);
    size_t nPosition = 3;
    char *sMessage;
    for (int ii = 0; sBuf[nPosition] != '\n'; sID_Temp[ii++] = sBuf[nPosition++]);       /* ID */
    pthread_mutex_lock(&pMutx);
    FILE * fRead = fopen("member_list", "rb");
    while (fread(sbUser_Temp, sizeof(USER), 1, fRead) > 0)
    {
        for (size_t ii = 0; ii < nCurrent_User_Num; ii++)
        {
            //If user is not Login Status
            if (nSock == asUser_Current[ii].nFileDiscript && asUser_Current[ii].sLoginState == FLAG_FALSE)
            {
                sMessage = "You have to Login before Ban\n";
                fclose(fRead);   free(sbUser_Temp);
                send_Msg(nSock, sizeof("You have to Login before Ban\n"), sMessage);
                pthread_mutex_unlock(&pMutx);
                return;
            }
            // Ban name is not me and In User List
            else if (nSock == asUser_Current[ii].nFileDiscript &&(strcmp(sID_Temp, sbUser_Temp->sID) == 0))
            {
                for (int a = 0; a < FLAG_USER_NUM; a++)
                {
                    if (asUser_Current[ii].nVenID[a] == 0)
                    {
                        asUser_Current[ii].nVenID[a] = sbUser_Temp->nFileDiscript;
                        break;
                    }
                }
                sMessage = "Ban success\n";
                fclose(fRead);   free(sbUser_Temp);
                send_Msg(nSock, sizeof("Ban success\n"), sMessage);
                pthread_mutex_unlock(&pMutx);
                return;
            }
        }
    }
    pthread_mutex_unlock(&pMutx);
    fclose(fRead);   free(sbUser_Temp);
    sMessage = "Incorrect User name\n";
    send_Msg(nSock, sizeof("Incorrect User name\n"), sMessage);
    return;
}
void game(const char * sBuf, const int nSock, const int msgSz)
{
    short gameReady = FLAG_FALSE;
    char *sMessage;
    int myPosition;
    float u1win = 0;
    float u2win = 0;
    if (strcmp(sBuf, "/game\n") == 0)
    {
        pthread_mutex_lock(&pMutx);
        for (myPosition = 0; myPosition < 2; myPosition++)
        {
            if (FLAG_GAME[myPosition] == 0)
            {
                FLAG_GAME[myPosition] = nSock;
                gameReady = FLAG_TRUE;
                
                if (myPosition == 1)
                    FLAG_GSTART = FLAG_TRUE;
                break;
            }
        }
        pthread_mutex_unlock(&pMutx);
        if (gameReady == FLAG_TRUE && FLAG_GSTART == FLAG_FALSE)
        {
            sMessage = "Game ready. waiting...\n";
            send_Msg(nSock, sizeof("Game ready. waiting...\n"), sMessage);
        }
        while (FLAG_GSTART == FLAG_FALSE);      // waiting until start game
        //(FLAG_GSTART == FLAG_TRUE)
        if(myPosition && FLAG_GSTART)
            printf("new game start\n");
        
        
        while (!gameBufSz[myPosition] && FLAG_GSTART == FLAG_TRUE)
        {
            sMessage = "Input your decision\n";
            send_Msg(nSock, sizeof("Input your decision\n"), sMessage);
            gameBufSz[myPosition] = read(nSock, gameBuF[myPosition], sizeof(gameBuF[myPosition]));
            if (FLAG_GSTART == FLAG_FALSE)////////// If Competator leave, Finish
                return;
            
            pthread_mutex_lock(&pMutx);
            if (gameBufSz[myPosition] == 0)         //Competator disconnectd
            {
                sMessage = "Game Finish\n";
                send_Msg(FLAG_GAME[myPosition], sizeof("Game Finish\n"), sMessage);
                sMessage = "Game Finish. (typed anything to continue)\n";
                send_Msg(FLAG_GAME[(myPosition + 1) % 2], sizeof("Game Finish. (typed anything to continue)\n"), sMessage);
                u1win = (float)gameResult[0] / (float)gameResult[2] * 100;   u2win = (float)gameResult[1] / (float)gameResult[2] * 100;
                printf("%.2f    %.2f\n", u1win, u2win);
                for (int ii = 0; ii < nCurrent_User_Num; ii++)
                {
                    if (nSock == asUser_Current[ii].nFileDiscript)
                    {
                        /* Printf Current Total User Number */
                        printf("User %d is disconnected - Number of total client is %d\n", asUser_Current[ii].nVersion + 1, --nCurrent_User_Num);
                        
                        /* Disenable UserID */
                        anUser_Name_Flag[asUser_Current[ii].nVersion] = FLAG_FALSE;
                        
                        while (ii++ < nCurrent_User_Num) { asUser_Current[ii] = asUser_Current[ii + 1]; }
                        break;
                    }
                }
                /* Reset Memory */
                memset(FLAG_GAME, 0, sizeof(FLAG_GAME));
                memset(gameResult, 0, sizeof(gameResult));
                memset(gameBuF, 0, sizeof(gameBuF));
                memset(gameBufSz, 0, sizeof(gameBufSz));
                FLAG_GSTART = FLAG_FALSE;
                close(nSock);
                pthread_mutex_unlock(&pMutx);
                return;
                
            }
            pthread_mutex_unlock(&pMutx);
            
            
            //         pthread_mutex_lock(&pMutx);
            switch (gameBufSz[myPosition])
            {
                case 2:
                {
                    if (gameBuF[myPosition][0] - '0' != 1 && gameBuF[myPosition][0] - '0' != 2 && gameBuF[myPosition][0] - '0' != 3)
                    {
                        sMessage = "Incorrect input, try again\n";
                        send_Msg(nSock, sizeof("Incorrect input, try again\n"), sMessage);
                        gameBufSz[myPosition] = 0;
                        //                  pthread_mutex_unlock(&pMutx);
                        continue;
                    }
                    break;
                }
                case 3:
                {
                    if (gameBuF[myPosition][0]== '/' && gameBuF[myPosition][1] == 'p'&& gameBuF[myPosition][2] == '\n')//game end
                    {
                        sMessage = "Game Finish\n";
                        send_Msg(FLAG_GAME[myPosition], sizeof("Game Finish\n"), sMessage);
                        sMessage = "Game Finish. (typed anything to continue)\n";
                        send_Msg(FLAG_GAME[(myPosition + 1) % 2], sizeof("Game Finish. (typed anything to continue)\n"), sMessage);                  u1win = (float)gameResult[0] / (float)gameResult[2] * 100;   u2win = (float)gameResult[1] / (float)gameResult[2] * 100;
                        printf("%.2f    %.2f\n", u1win, u2win);
                        memset(FLAG_GAME, 0, sizeof(FLAG_GAME));
                        memset(gameResult, 0, sizeof(gameResult));
                        memset(gameBuF, 0, sizeof(gameBuF));
                        memset(gameBufSz, 0, sizeof(gameBufSz));
                        FLAG_GSTART = FLAG_FALSE;
                        //                  pthread_mutex_unlock(&pMutx);
                        return;
                    }
                    else
                    {
                        sMessage = "Incorrect input, try again\n";
                        send_Msg(nSock, sizeof("Incorrect input, try again\n"), sMessage);
                        gameBufSz[myPosition] = 0;
                        //                  pthread_mutex_unlock(&pMutx);
                        continue;
                    }
                }
                default:
                {
                    sMessage = "Incorrect input, try again\n";
                    send_Msg(nSock, sizeof("Incorrect input, try again\n"), sMessage);
                    gameBufSz[myPosition] = 0;
                    //               pthread_mutex_unlock(&pMutx);
                    continue;
                    //printf("%d\n"read)
                }
            }
            //         pthread_mutex_unlock(&pMutx);
            while (gameBufSz[0] == 0 || gameBufSz[1]== 0);
            
            //         pthread_mutex_lock(&pMutx);
            if (gameBufSz[0]!=0 && gameBufSz[1] != 0 && (   ( gameBuF[myPosition][0] - gameBuF[(myPosition + 1) % 2][0]) == -1 || (gameBuF[myPosition][0] - gameBuF[(myPosition + 1) % 2][0]) == 2))   //myPosition lose
            {
                //            pthread_mutex_lock(&pMutx);
                sMessage = "You Lose\n";
                send_Msg(FLAG_GAME[myPosition], sizeof("You Lose\n"), sMessage);
                memset(gameBuF[myPosition], 0, sizeof(gameBuF[myPosition]));
                gameBufSz[myPosition] = 0;
                //            pthread_mutex_unlock(&pMutx);
                continue;
            }
            else  if(gameBufSz[0] != 0 && gameBufSz[1] != 0 && (gameBuF[myPosition][0] - gameBuF[(myPosition + 1) % 2][0]) == 0) //draw
            {
                
                sMessage = "Draw\n";
                send_Msg(FLAG_GAME[myPosition], sizeof("Draw\n"), sMessage);
                gameBufSz[myPosition] = 0;
                //            pthread_mutex_lock(&pMutx);
                if (myPosition == 1)
                {
                    gameResult[2]++;
                    memset(gameBuF, 0, sizeof(gameBuF));
                    gameBufSz[0] = 0; gameBufSz[1] = 0;
                }
                //            pthread_mutex_unlock(&pMutx);
                continue;
            }
            else if (gameBufSz[0] != 0 && gameBufSz[1] != 0 && ((gameBuF[myPosition][0] - gameBuF[(myPosition + 1) % 2][0]) == -2 || (gameBuF[myPosition][0] - gameBuF[(myPosition + 1) % 2][0]) == 1))//myposition win
            {
                //            pthread_mutex_lock(&pMutx);
                sMessage = "You Win\n";
                send_Msg(FLAG_GAME[myPosition], sizeof("You Win\n"), sMessage);
                gameResult[myPosition]++; gameResult[2]++;         //for Win rate
                memset(gameBuF[myPosition], 0, sizeof(gameBuF[myPosition]));
                gameBufSz[myPosition] = 0;
                //            pthread_mutex_unlock(&pMutx);
                continue;
            }
            else
            {
                //            pthread_mutex_lock(&pMutx);
                memset(gameBuF[myPosition], 0, sizeof(gameBuF[myPosition]));
                gameBufSz[myPosition] = 0;
                //            pthread_mutex_unlock(&pMutx);
                continue;
            }
            
            //memset(readBuf, 0, sizeof(readBuf));
        }
    }
    else
    {
        sMessage = "Game order must be ""/game"". Incorrect Order\n";
        send_Msg(nSock, sizeof("Game order must be ""/game"". Incorrect Order\n"), sMessage);
    }
    
    memset(FLAG_GAME, 0, sizeof(FLAG_GAME));
    memset(gameResult, 0, sizeof(gameResult));
    memset(gameBuF, 0, sizeof(gameBuF));
    FLAG_GSTART = FLAG_FALSE;
    return;
}
void Whisper_Member(const char * sBuf, const int nSock)
{
    /* String */
    char sID_Temp[BUF_REG];
    char sComment[BUF_REG];
    char sSend_Msg[BUF_REG] = "Whisper fail, no user\n";
    
    /* Integer */
    size_t nPosition = 3;
    int nReceive_Sock = nSock;
    for (int ii = 0; sBuf[nPosition] != ' '; sID_Temp[ii++] = sBuf[nPosition++]);   /* ID */
    for (int ii = 0; sBuf[nPosition] != '\n'; sComment[ii++] = sBuf[nPosition++]);  /* Comment */
    
    /* Select User */
    for (int ii = 0; ii < FLAG_USER_NUM; ii++)
    {
        /* Select User Success */
        if (strcmp(asUser_Current[ii].sID, sID_Temp) == 0)
        {
            for (int a = 0; a < FLAG_USER_NUM; a++)
            {
                if (asUser_Current[ii].nVenID[a] == nSock)
                {
                    return;
                }
            }
            
            strcpy(sSend_Msg, Select_Nickname(nSock));
            strcat(sSend_Msg, sComment);    /* Append Comment */
            strcat(sSend_Msg, "\n");
            nReceive_Sock = asUser_Current[ii].nFileDiscript;
            break;
        }
    }
    
    /* Send Message to Sender */
    send_Msg(nReceive_Sock, strlen(sSend_Msg), sSend_Msg);
    return;
}


void join_Member(const char * sBuf, const int nSock,const int msgSz)
{
    /* String */
    char sID_Temp[BUF_REG];
    char sPasswd_Temp[BUF_REG];
    char *sMessage;
    
    /* Struct User */
    USER * sCheck_Temp = MALLOC(USER, 1);
    
    /* Integer */
    size_t nPosition = 10;
    size_t nChecking = FLAG_FALSE;
    for (int ii = 0; sBuf[nPosition] != ' '; sID_Temp[ii++] = sBuf[nPosition++]);       /* ID */
    for (int ii = 0; sBuf[nPosition] != '\n'; sPasswd_Temp[ii++] = sBuf[nPosition++]);  /* Password */
    
    pthread_mutex_lock(&pMutx);
    
    /* Overlap ID Check */
    FILE * fRead = fopen("member_list", "rb");
    if (fRead != NULL)
    {
        /* Read Struct */
        while (fread(sCheck_Temp, sizeof(USER), 1, fRead) > 0)
        {
            if (strcmp(sCheck_Temp->sID, sID_Temp) == 0)
            {
                printf("Incorrect registration\n");
                sMessage = "Incorrect registration\n";
                nChecking = FLAG_TRUE;
                send_Msg(nSock, sizeof("Incorrect registration\n"), sMessage);
                fclose(fRead);
                pthread_mutex_unlock(&pMutx);
                return;
            }
        }
        
        /* Close File */
        fclose(fRead);
        
    }
    
    if (nChecking == FLAG_FALSE)
    {
        /* Not Overlap ID */
        FILE * fWrite = fopen("member_list", "ab");
        if (fWrite != NULL)
        {
            /* Write Struct */
            fwrite(insert_UserData(nSock, 0, sID_Temp, sPasswd_Temp), sizeof(USER), 1, fWrite);
            
            /* Register Print */
            printf("%s is registered\n", sID_Temp);
            
            /* Close File */
            fclose(fWrite);
        }
    }
    
    pthread_mutex_unlock(&pMutx);
    sMessage = "Registration success\n";
    send_Msg(nSock, sizeof("Registration success\n"), sMessage);
    return;
}
void login_Member(const char * sBuf, const int nSock,const int msgSz)
{
    /* String */
    char sID_Temp[BUF_REG];
    char sPasswd_Temp[BUF_REG];
    char *sMessage;
    
    /* Integer */
    size_t nPosition = 7;
    for (int ii = 0; sBuf[nPosition] != ' '; sID_Temp[ii++] = sBuf[nPosition++]);       /* ID */
    for (int ii = 0; sBuf[nPosition] != '\n'; sPasswd_Temp[ii++] = sBuf[nPosition++]);  /* Password */
    
    pthread_mutex_lock(&pMutx);
    
    /* FILE */
    FILE * fRead = fopen("member_list", "rb");
    if (fRead != NULL)
    {
        /* Struct Users */
        USER * sUser_Temp = MALLOC(USER, 1);
        while (fread(sUser_Temp,  sizeof(USER), 1, fRead) > 0)
        {
            /* Login Sucess */
            if (strcmp(sUser_Temp->sID, sID_Temp) == 0 && strcmp(sUser_Temp->sPassword, sPasswd_Temp) == 0)
            {
                /* Select FileDescriptor */
                for (size_t ii = 0; ii < nCurrent_User_Num; ii++)
                {
                    if (nSock == asUser_Current[ii].nFileDiscript)
                    {
                        strcpy(asUser_Current[ii].sID, sID_Temp);           /* ID */
                        strcpy(asUser_Current[ii].sPassword, sPasswd_Temp); /* Password */
                        asUser_Current[ii].sLoginState = FLAG_TRUE;
                        break;
                    }
                }
                sMessage = "Login successful\n";
                send_Msg(nSock, sizeof("Login successful\n"), sMessage);
                FILE * fWrite = fopen("member_list", "ab");
                
                /* Write Struct for puts Socket Discripter*/
                fwrite(insert_UserData(nSock, 0, sID_Temp, sPasswd_Temp), sizeof(USER), 1, fWrite);
                
                /* Close File */
                fclose(fWrite);
                fclose(fRead);
                pthread_mutex_unlock(&pMutx);
                return;
            }
        }
        
        /* File Close */
        fclose(fRead);
    }
    
    pthread_mutex_unlock(&pMutx);
    sMessage = "Login fail\n";
    send_Msg(nSock, sizeof("Login fail\n"), sMessage);
    return;
}
USER * insert_UserData(const int nSocket, const int nVersion, const char * sID, const char * sPasswd)
{
    
    /* Struct User */
    USER * sUser = (USER*)malloc(sizeof(USER));
    for (int ii = 0; ii < FLAG_USER_NUM; ii++)
        sUser->nVenID[ii] = 0;
    sUser->nFileDiscript = nSocket;         sUser->nVersion = nVersion;
    strcpy(sUser->sID, sID);    strcpy(sUser->sPassword, sPasswd);      sUser->sLoginState = FLAG_FALSE;
    
    return sUser;
}
