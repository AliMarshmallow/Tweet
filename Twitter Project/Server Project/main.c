#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <unistd.h>
#include "cJSON.h"
char *signup(char *str1,char *str2);
char *token_rand();
char *login(char *str1,char *str2);
int initialize(int port);
void start(int server_sock);
char *send_tweet(char *tweet,char *name);
char **Token,**Names;
int cou;
int main() {
    cou=0;
    Names = calloc(10000, sizeof(char *));
    Token = calloc(10000, sizeof(char *));
    for(int i=0;i<1000;i++){
        Token[i] = calloc(33,1);
        Names[i] = calloc(1000,1);
        memset(Token[i],0,33);
        memset(Names[i],0,1000);
    }
    int sock=initialize(12345);
    while (sock!=INVALID_SOCKET){
        start(sock);
    }
    return 0;
}
int initialize(int port){
    WSADATA wsadata;
    int wsa_res=WSAStartup(MAKEWORD(2,2), &wsadata);
    if(wsa_res !=0 ){
        printf("Failed wsadata\n");
        return INVALID_SOCKET;
    }
    int server_sock=socket(AF_INET,SOCK_STREAM,0);
    if(server_sock == INVALID_SOCKET){
        printf("Failed Create Socket");
        return INVALID_SOCKET;
    }
    struct sockaddr_in server_sock_addr;
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_addr.s_addr = INADDR_ANY;
    server_sock_addr.sin_port = htons(port);
    int bind_res= bind(server_sock,(struct sockaddr*)&server_sock_addr, sizeof(server_sock_addr));
    if(bind_res == SOCKET_ERROR){
        printf("bind failed\n");
        return INVALID_SOCKET;
    }
    listen(server_sock,SOMAXCONN);
    return server_sock;
}
void start(int server_sock){
    int len = sizeof(struct sockaddr_in);
    int client_sock;
    struct sockaddr_in client;
    char name[1000];
    while((client_sock =accept(server_sock, (struct sockaddr*)&client,&len))!=INVALID_SOCKET) {
        char *response;
        printf("Client connected\n");
        char buf[10000];
        memset(buf, 0, 10000);
        recv(client_sock, buf, 9999, 0);
        char input[1000];
        sscanf(buf, "%s", input);
        if (strcmp(input, "signup") == 0) {
            char str1[1000], str2[1000];
            sscanf(buf, "%*s%s%s", str1, str2);
            printf("%s\n%s\n", str1, str2);
            response = signup(str1, str2);
        } else if (strcmp(input, "login") == 0) {
            char str2[1000];
            sscanf(buf, "%*s%s%s", name, str2);
            response = login(name, str2);
        } else if (strcmp(input, "send") == 0) {
            char str1[1000];
            char tok0[1000];
            sscanf(buf, "%*s%*s%s%s", tok0, str1);
            tok0[strlen(tok0) - 1] = '\0';
            int flag0 = -1;
            for (int i = 0; i < cou; i++) {
                if (strcmp(tok0, Token[i]) == 0) {
                    flag0 = i;
                }
            }
            if (flag0 == -1) {
                response = "{\"type\":\"Error\",\"message\":\"Token Invalid.\"}";
            } else {
                response = send_tweet(str1, Names[flag0]);
            }
        } else if (strcmp(input, "refresh") == 0) {

        } else if (strcmp(input, "like")) {

        } else if (strcmp(input, "comment")) {

        } else if (strcmp(input, "search")) {

        } else if (strcmp(input, "follow")) {

        } else if (strcmp(input, "unfollow")) {

        } else if (strcmp(input, "set")) {

        } else if (strcmp(input, "logout")) {

        } else if (strcmp(input, "profile")) {

        } else if (strcmp(input, "change")) {

        } else {
            printf("Bad Request\n");
            response = "Try Again";
        }
        printf("%s\n", response);
        send(client_sock, response, strlen(response), 0);
  //    free(response);
        closesocket(client_sock);
    }
}
char *signup(char *str1,char *str2){
    int a=strlen(str1);
    char *result=calloc(1000,1);
    str1[a-1]='\0';
    char fname[1000];
    memset(fname,0,1000);
    strcat(fname,"../Resource/Users/");
    strcat(fname,str1);
    strcat(fname,".txt");
    if(!access(fname,F_OK)) {
        strcpy(result, "{\"type\":\"Error\",\"message\":\"This username is already taken.\"}");
        return result;
    }
    FILE *ptr;
    ptr=fopen(fname,"w");
    fprintf(ptr,"{\"username\":\"%s\",\"password\":\"%s\",\"bio\":\"\",\"followers\":[],\"followings\":[],\"personalTweets\":[]}",str1,str2);
    fclose(ptr);
    strcpy(result,"{\"type\":\"Successful\",\"message\":\"\"}");
    return result;

}
char *login(char *str1,char *str2){
    int a=strlen(str1);
    char *result=calloc(1000,1);
    str1[a-1]='\0';
    char fname[1000];
    memset(fname,0,1000);
    strcat(fname,"../Resource/Users/");
    strcat(fname,str1);
    strcat(fname,".txt");
    if(!access(fname,F_OK)){
        FILE *ptr=fopen(fname,"r");
        char data[10000];;
        fgets(data,9999,ptr);
        fclose(ptr);
        cJSON *root=cJSON_Parse(data);
        cJSON *pass0=cJSON_GetObjectItemCaseSensitive(root,"password");
        if(strcmp(pass0->valuestring,str2)!=0){
            strcpy(result,"{\"type\":\"Error\",\"message\":\"Incorrect Password.\"}");
            return result;
        }
        for (int i=0;i<cou;i++){
            if(strcmp(str1,Names[i])==0){
                strcpy(result,"{\"type\":\"Error\",\"message\":\"This user has already logged in.\"}");
                return result;
            }
        }
        char *tok;
        tok=token_rand(32);
        sprintf(Token[cou],"%s",tok);
        sprintf(Names[cou],"%s",str1);
        cou+=1;
        sprintf(result,"{\"type\":\"Token\",\"message\":\"%s\"}",Token[cou-1]);
        return result;
    }
    strcpy(result,"{\"type\":\"Error\",\"message\":\"Username not found.\"}");
    return result;
}
char *send_tweet(char *tweet,char *name){
    FILE *ptr;
    int flag=0;
    char check[10000];
    for(int i=1;i>0;i++){
        memset(check,0,10000);
        sprintf(check,"../Resource/Tweets/%d.txt",i);
        if(access(check,F_OK)){
            flag=i;
            break;
        }
    }
    ptr=fopen(check,"w");
    if(ptr==NULL){
        char *result="{\"type\":\"Error\",\"message\":\"sorry please try again.\"}";
        return result;
    }
    fprintf(ptr,"{\"id\":%d,\"author\":\"%s\",\"content\":\"%s\",\"comments\":{},\"likes\":0}",flag,name,tweet);
    fclose(ptr);
    char *result="{\"type\":\"Successful\",\"message\":\"tweeted your tweet\"}";
    return result;
};

char *token_rand() {

    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-#'?!@$`+*";
    char *randomString = NULL;
    int length=32;
    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {
            for (int n = 0;n < length;n++) {
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }

    return randomString;
}