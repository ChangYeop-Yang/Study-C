#include "Header.h"

/* Global Variable */
pthread_mutex_t pMutx;
int nCurrent_User_Num = 0;
int anUser_Name_Flag[FLAG_USER_NUM];
USER asUser_Current[FLAG_USER_NUM];

/* Declear Function */
int select_Position();
void * handle_Clnt(void * arg);
void handle_Error(const short sError);
char * join_Member(const char * sBuf, const int nSock);
char * login_Member(const char * sBuf, const int nSock);
short send_Msg(const int nSocket, const char * sMsg);
USER * insert_UserData(const int nSocket, const int nVersion, const char * sID, const char * sPasswd);

int main(int argc, char * argv[])
{
    if (argc != 2) { handle_Error(FLAG_IP_ERROR); }

    /* Pthread */
    pthread_t t_id;
    
    /* Integer */
    int nServ_Sock = socket(PF_INET, SOCK_STREAM, 0);
    
    /* struct sockaddr_in */
    SOCK_IN sServ_Adr, sClnt_Adr;
    
    /* sockaddr_in Initialization */
    memset(&sServ_Adr, 0, sizeof(sServ_Adr));
    sServ_Adr.sin_family = AF_INET;
    sServ_Adr.sin_addr.s_addr = htonl(INADDR_ANY);
    sServ_Adr.sin_port = htons(atoi(argv[1]));
    
    /* Mutex Initialization */
    pthread_mutex_init(&pMutx, NULL);
    
    /* Socket Bind & Listen */
    if (bind(nServ_Sock, (struct sockaddr *) &sServ_Adr, sizeof(sServ_Adr)) == EOF) { handle_Error(FLAG_BIND_ERROR); }
    if (listen(nServ_Sock, 5) == EOF) { handle_Error(FLAG_LISTEN_ERROR); }
    
    for (int nClnt_Sock = 0; FLAG_TRUE;)
    {
        socklen_t unClnt_Adr_Sz = sizeof(sClnt_Adr);
        if ( (nClnt_Sock = accept(nServ_Sock, (struct sockaddr *)&sClnt_Adr, &unClnt_Adr_Sz)) == EOF ) { continue; }
        
        /* Pthread Mutex Range */
        pthread_mutex_lock(&pMutx);
        {
            const int nPosition = select_Position();
            USER * sTemp_User = insert_UserData(nClnt_Sock, nPosition, "", "");
            char * sTemp_String = (char*)malloc(sizeof(char) * BUFSIZ);
            
            asUser_Current[nPosition] = *sTemp_User;
            sprintf(sTemp_String, "Connection Success, Your ID is User%d\n", sTemp_User->nVersion + 1);
            send_Msg(sTemp_User->nFileDiscript, sTemp_String);
            
            free(sTemp_String);
        }
        pthread_mutex_unlock(&pMutx);
        
        pthread_create(&t_id, NULL, handle_Clnt, (void*)&nClnt_Sock);
        pthread_detach(t_id);
        
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
void * handle_Clnt(void * arg)
{
    /* Integer */
    int nClnt_Sock = *((int*)arg);
    int nStr_Len = 0;
    
    /* Char */
    char * sMsg = (char*)malloc(sizeof(char) * BUFSIZ);
    
    while ( (nStr_Len = read(nClnt_Sock, sMsg, BUFSIZ) != 0) )
    {
        /* Pthread Mutex Range */
        pthread_mutex_lock(&pMutx);
        {
            /* Special Instruction */
            if (sMsg[0] == '/')
            {
                switch (sMsg[1])
                {
                    /* Register */
                    case ('r') : { send_Msg(nClnt_Sock, join_Member(sMsg, nClnt_Sock)); break; }
                    /* Login */
                    case ('l') : { send_Msg(nClnt_Sock, login_Member(sMsg, nClnt_Sock)); break; }
                    /* Game */
                    case ('g') : { break; }
                    /* Ban */
                    case ('b') : { break; }
                }
            }
            /* Nomal Instruction */
            else
            {
                for (int ii = 0; ii < nCurrent_User_Num; ii++)
                { send_Msg(asUser_Current[ii].nFileDiscript, sMsg); }
            }
        }
        pthread_mutex_unlock(&pMutx);
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
char * join_Member(const char * sBuf, const int nSock)
{
    /* String */
    char * sID_Temp = MALLOC(char, BUFSIZ);
    char * sPasswd_Temp = MALLOC(char, BUFSIZ);
    
    /* Integer */
    size_t nPosition = 10;
    for (int ii = 0; sBuf[nPosition] != ' '; sID_Temp[ii++] = sBuf[nPosition++]);       /* ID */
    for (int ii = 0; sBuf[nPosition] != '\n'; sPasswd_Temp[ii++] = sBuf[nPosition++]);  /* Password */
    
    /* FILE */
    FILE * fRead = fopen("member_list", "rb");
    FILE * fWrite = fopen("member_list", "ab");

    /* Overlap ID Check */
    if (fRead != NULL)
    {
        /* Struct User */
        USER * sCheck_Temp = MALLOC(USER, 1);
        
        /* Read Struct */
        while (fread(sCheck_Temp, sizeof(USER), 1, fRead) > 0)
        {
            if (strcmp(sCheck_Temp->sID, sID_Temp) == 0)
            {
                printf("Incorrect registration\n");
                free(sCheck_Temp);  fclose(fRead);
                
                return "Incorrect registration";
            }
        }
        
        free(sCheck_Temp);  fclose(fRead);
    }
    
    /* Not Overlap ID */
    if (fWrite != NULL)
    {
        /* Write Struct */
        fwrite(insert_UserData(nSock, 0, sID_Temp, sPasswd_Temp), sizeof(USER), 1, fWrite);
        
        /* Register Print */
        printf("%s is registered\n", sID_Temp);     fclose(fWrite);
    }

    /* FILE Close */
    free(sID_Temp);     free(sPasswd_Temp);
    
    return "Registration success";
}
char * login_Member(const char * sBuf, const int nSock)
{
    /* String */
    char * sID_Temp = MALLOC(char, BUFSIZ);
    char * sPasswd_Temp = MALLOC(char, BUFSIZ);
    
    /* Integer */
    size_t nPosition = 7;
    for (int ii = 0; sBuf[nPosition] != ' '; sID_Temp[ii++] = sBuf[nPosition++]);       /* ID */
    for (int ii = 0; sBuf[nPosition] != '\n'; sPasswd_Temp[ii++] = sBuf[nPosition++]);  /* Password */
    
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
                        break;
                    }
                }
                
                /* Memory Reset */
                free(sUser_Temp);    fclose(fRead);
                
                return "Login successful";
            }
        }
        
        /* Memory Reset */
        free(sUser_Temp);    fclose(fRead);
    }
    
    /* Memory Reset */
    free(sID_Temp);     free(sPasswd_Temp);
    
    return "Login fail";
}
USER * insert_UserData(const int nSocket, const int nVersion, const char * sID, const char * sPasswd)
{
    /* Struct User */
    USER * sUser = (USER*)malloc(sizeof(USER));
    sUser->nVenID = 0;          sUser->nFileDiscript = nSocket;         sUser->nVersion = nVersion;
    strcpy(sUser->sID, sID);    strcpy(sUser->sPassword, sPasswd);
    
    return sUser;
}
short send_Msg(const int nSocket, const char * sMsg)
{ return write(nSocket, sMsg, strlen(sMsg)) != EOF ? FLAG_TRUE : FLAG_FALSE; }
