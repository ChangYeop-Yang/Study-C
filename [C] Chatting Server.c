#include "Header.h"

/* Global Variable */
pthread_mutex_t pMutx;
int nCurrent_User_Num = 0;
int anUser_Name_Flag[FLAG_USER_NUM];
USER asUser_Current[FLAG_USER_NUM];

/* Declear Function */
int select_Position();
void * handle_Read(void * arg);
void handle_Error(const short sError);
void send_Msg(const int nSocket, const ssize_t nLength, const char * sMsg);
void Whisper_Member(const char * sBuf, const int nSock);
char * Select_Nickname(const int nSock);
char * join_Member(const char * sBuf, const int nSock);
char * login_Member(const char * sBuf, const int nSock);
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
    
    while(FLAG_TRUE)
    {
        socklen_t unClnt_Adr_Sz = sizeof(sClnt_Adr);
        if ( (nClnt_Sock = accept(nServ_Sock, (struct sockaddr *)&sClnt_Adr, &unClnt_Adr_Sz)) == EOF ) { break; }
        
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
        printf("New client is connected – Number of total client is %d\n", ++nCurrent_User_Num);
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
{ write(nSocket, sMsg, nLength); }
void * handle_Read(void * arg)
{
    /* Integer */
    ssize_t nStr_Len = 0;
    ssize_t nPosition = 0;
    int nClnt_Sock = *((int*)arg);
    
    /* Char */
    char sMsg[BUFSIZ];
    memset(sMsg, 0, sizeof(sMsg));
    
    while ( (nStr_Len = read(nClnt_Sock, sMsg, sizeof(sMsg))) != 0 )
    {
        printf("%s\n", sMsg);
        if (sMsg[0] == '/')
        {
            switch (sMsg[1])
            {
                /* Register */
                case ('r') : { send_Msg(nClnt_Sock, nStr_Len, join_Member(sMsg, nClnt_Sock)); break; }
                /* Login */
                case ('l') : { send_Msg(nClnt_Sock, nStr_Len, login_Member(sMsg, nClnt_Sock)); break; }
                /* Game */
                case ('g') : { break; }
                /* Ban */
                case ('b') : { break; }
                /* Whisper */
                case ('w') : { Whisper_Member(sMsg, nClnt_Sock); break; }
            }
        }
        else
        {
            /* Select User */
            for (size_t ii = 0; ii < FLAG_USER_NUM; ii++)
            { if (asUser_Current[ii].nFileDiscript == nClnt_Sock) { nPosition = ii; } }
            
            /* UnEable Login User */
            if (asUser_Current[nPosition].sLoginState == FLAG_FALSE)
            {    send_Msg(asUser_Current[nPosition].nFileDiscript, nStr_Len,sMsg);   }
            /* Eable Login User */
            else
            {
                /* Send User Message */
                for (size_t ii = 0; ii < nCurrent_User_Num; ii++)
                {
                    /* UnRegistered User Or UnEable Login User */
                    if (asUser_Current[ii].nFileDiscript != nClnt_Sock && asUser_Current[ii].nFileDiscript != 0)
                    {
                        /* Registered User Or Eable Login User */
                        char * sNickName = MALLOC(char, BUFSIZ);
                        sprintf(sNickName, "%s)%s", asUser_Current[nPosition].sID, sMsg);
                        send_Msg(asUser_Current[ii].nFileDiscript, strlen(sNickName), sNickName);
                        free(sNickName);
                    }
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
                
                while (ii++ < nCurrent_User_Num) { asUser_Current[ii] = asUser_Current[ii+1]; }
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
        case (FLAG_IP_ERROR) : { printf("Using Current IP Address Error!!!\n"); break; }
        case (FLAG_SOCKET_ERROR) : { printf("Create Socket Error!!!\n"); break; }
        case (FLAG_CONNECT_ERROR) : { printf("Connect Server Error!!!\n"); break; }
        case (FLAG_BIND_ERROR) : { printf("Bind Socket Error!!!\n"); break; }
        case (FLAG_LISTEN_ERROR) : { printf("Listen Socket Error!!!\n"); break; }
        default : { printf("Network UnKnown Error!!!\n"); break; }
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
        { sprintf(sNickName ,"%s)", asUser_Current[ii].sID); }
    }
    
    return sNickName;
}
void Whisper_Member(const char * sBuf, const int nSock)
{
    /* String */
    char sID_Temp[BUF_REG];
    char sComment[BUF_REG];
    char * sSend_Msg = "Whisper fail, no user\n";
    
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
            sSend_Msg = Select_Nickname(nSock);
            strcat(sSend_Msg, sComment);    /* Append Comment */
            nReceive_Sock = asUser_Current[ii].nFileDiscript;
            break;
        }
    }
    
    /* Send Message to Sender */
    send_Msg(nReceive_Sock, strlen(sSend_Msg), sSend_Msg);
}
char * join_Member(const char * sBuf, const int nSock)
{
    /* String */
    char sID_Temp[BUF_REG];
    char sPasswd_Temp[BUF_REG];
    char * sMessage = "Registration success\n";
    
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
            }
        }
        
        /* Close File */
        fclose(fRead);
    }
    
    if (nChecking == FLAG_FALSE)
    {
        /* Not Overlap ID */
        FILE * fWrite = fopen("member_list", "wb");
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
    
    return sMessage;
}
char * login_Member(const char * sBuf, const int nSock)
{
    /* String */
    char sID_Temp[BUF_REG];
    char sPasswd_Temp[BUF_REG];
    char * sMessage = "Login fail\n";
    
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
        while (fread(sUser_Temp, sizeof(USER), 1, fRead) > 0)
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
            }
        }
        
        /* File Close */
        fclose(fRead);
    }
    
    pthread_mutex_unlock(&pMutx);
    
    return sMessage;
}
USER * insert_UserData(const int nSocket, const int nVersion, const char * sID, const char * sPasswd)
{
    /* Struct User */
    USER * sUser = (USER*)malloc(sizeof(USER));
    sUser->nVenID = 0;          sUser->nFileDiscript = nSocket;         sUser->nVersion = nVersion;
    strcpy(sUser->sID, sID);    strcpy(sUser->sPassword, sPasswd);      sUser->sLoginState = FLAG_FALSE;
    
    return sUser;
}
