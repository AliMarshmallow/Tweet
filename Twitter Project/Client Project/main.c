#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "cJSON.h"
int initialize(int);
int parse_com(char *comment);
char *send_data(char *data);
int firstmenu();
int show_com(char *data);
void parse_tw_com(char *data,char *id);
struct sockaddr_in server;
char *signup();
char *login();
int menu();
int timeline();
char *search1(char *token,char *name);
int personal();
void parse_tweets(char *result9);
void tweet_parser(char *messagee4);
void parser(char *data);
int main() {
    char *string=malloc(100);
    int po;
    po=initialize(12345);
    while(po == 0)
    {
        int check = firstmenu();
        if(check==3){
            return 0;
        }
        if(check==1)
        {
            string = login();
            char str[100];
            for(int i=0;i<100;i++)
            {
                if(string[i]!='}')
                {
                    str[i]=string[i];
                }
                else {
                    str[i] = string[i];
                    str[i + 1] = '\0';
                    break;
                }
            }
            cJSON *root=cJSON_Parse(str);
            cJSON *type=cJSON_GetObjectItemCaseSensitive(root,"type");
            cJSON *message=cJSON_GetObjectItemCaseSensitive(root,"message");
            char *typ=cJSON_Print(type);
            char *messag=cJSON_Print(message);
            if(strcmp(typ,"\"Token\"")==0)
            {
                char token[33];
                for(int i=1;i<33;i++){
                    token[i-1]=messag[i];
                }
                token[32]='\0';
                int ch =0;
                while(ch!=5) {
                    ch=menu();
                    if (ch == 1) {
                        int ch2 = timeline();
                        if(ch2==1){
                            char tweet[1000];
                            printf("your tweet:\n");
                            scanf("%*c%[^\n]s",&tweet);
                            char data1[10000]="send tweet ";
                            strcat(data1,token);
                            strcat(data1,", ");
                            strcat(data1,tweet);
                            strcat(data1,"\n");
                            char *result5=send_data(data1);
                            cJSON *root1=cJSON_Parse(result5);
                            cJSON *message1=cJSON_GetObjectItemCaseSensitive(root1,"message");
                            char *messag1=cJSON_Print(message1);
                            printf("%s\n",messag1);
                        }
                        else if(ch2==2){
                            char data6[10000]="refresh ";
                            strcat(data6,token);
                            strcat(data6,"\n");
                            char *result9=send_data(data6);
                            cJSON *root4=cJSON_Parse(result9);
                            cJSON *message4=cJSON_GetObjectItemCaseSensitive(root4,"message");
                            char *messag4=cJSON_Print(message4);
                            parse_tweets(result9);
                            printf("\n\n\n");
                            printf("1.like or comment a tweet or show comment\n");
                            printf("2.back\n");
                            int check5;
                            scanf("%d",&check5);
                            if(check5==1){
                                int check6=0;
                                while(check6!=4){
                                    printf("1.like\n");
                                    printf("2.comment\n");
                                    printf("3.show comments\n");
                                    printf("4.back\n");
                                    scanf("%d",&check6);
                                    if(check6==1){
                                        char id[10000];
                                        printf("write id number of tweet\n");
                                        scanf("%*c%s",&id);
                                        char data10[1000]="like ";
                                        strcat(data10,token);
                                        strcat(data10,", ");
                                        strcat(data10,id);
                                        strcat(data10,"\n");
                                        char *result10=send_data(data10);
                                        cJSON *root1=cJSON_Parse(result10);
                                        cJSON *message1=cJSON_GetObjectItemCaseSensitive(root1,"message");
                                        char *messag1=cJSON_Print(message1);
                                        printf("%s\n",messag1);
                                    }
                                    else if(check6==2){
                                        char id[10000],comment[10000];
                                        char data10[10000]="comment ";
                                        printf("write id number of tweet:\n");
                                        scanf("%*c%s",&id);
                                        printf("write tour comment:\n");
                                        scanf("%*c%[^\n]s",comment);
                                        strcat(data10,token);
                                        strcat(data10,", ");
                                        strcat(data10,id);
                                        strcat(data10,", ");
                                        strcat(data10,comment);
                                        strcat(data10,"\n");
                                        char *result10=send_data(data10);
                                        cJSON *root1=cJSON_Parse(result10);
                                        cJSON *message1=cJSON_GetObjectItemCaseSensitive(root1,"message");
                                        char *messag1=cJSON_Print(message1);
                                        printf("%s\n",messag1);
                                    }
                                    else if(check6==3){
                                        char id[10000];
                                        memset(id,0,10000);
                                        printf("write id number of tweet:\n");
                                        scanf("%*c%s",&id);
                                        parse_tw_com(messag4,&id);
                                    }
                                }
                            }
                        }
                    } else if (ch == 2) {
                        char *name1[1000];
                        printf("Username:\n");
                        scanf("%*c%[^\n]s",name1);
                        char *result2=malloc(10000);
                        memset(result2, 0,10000);
                        result2=search1(token,name1);
                        cJSON *root2=cJSON_Parse(result2);
                        cJSON *type10=cJSON_GetObjectItemCaseSensitive(root2,"type");
                        cJSON *message2=cJSON_GetObjectItemCaseSensitive(root2,"message");
                        char *messag2=cJSON_Print(message2);
                        if(strcmp(type10->valuestring,"Error")==0)
                            printf("%s\n",messag2);
                        else{
                            cJSON *root3=cJSON_Parse(messag2);
                            cJSON *user=cJSON_GetObjectItemCaseSensitive(root3,"username");
                            cJSON *bio1=cJSON_GetObjectItemCaseSensitive(root3,"bio");
                            cJSON *followers=cJSON_GetObjectItemCaseSensitive(root3,"numberOfFollowers");
                            cJSON *followings=cJSON_GetObjectItemCaseSensitive(root3,"numberOfFollowings");
                            cJSON *statu=cJSON_GetObjectItemCaseSensitive(root3,"followStatus");
                            cJSON *tweets=cJSON_GetObjectItemCaseSensitive(root3,"allTweets");
                            char *user1=cJSON_Print(user);
                            char *bioo=cJSON_Print(bio1);
                            char *follo1=cJSON_Print(followers);
                            char *follo2=cJSON_Print(followings);
                            char *me1=cJSON_Print(statu);
                            char *tweet=cJSON_Print(tweets);
                            printf("Username:%s\nBio:%s\nfollowers:%s   followings:%s\n",user1,bioo,follo1,follo2);
                            tweet_parser(tweet);
                            printf("\n\n\n");
                            int flag2;
                            if(strcmp(me1,"\"NotFollowed\"")==0){
                                printf("1.if you want follow this user\n");
                                flag2=0;
                            }
                            else{
                                printf("1.if you want unfollow this use\n");
                                flag2=1;
                            }
                            printf("2.back\n");
                            int check2;
                            scanf("%*c%d",&check2);
                            if(check2==1){
                                if(flag2==0){
                                    char data8[10000]="follow ";
                                    strcat(data8,token);
                                    strcat(data8,", ");
                                    strcat(data8,name1);
                                    strcat(data8,"\n");
                                    char *result8=send_data(data8);
                                    cJSON *root8=cJSON_Parse(result8);
                                    cJSON *message8=cJSON_GetObjectItemCaseSensitive(root8,"message");
                                    char *messag8=cJSON_Print(message8);
                                    printf("%s\n",messag8);
                                }
                                else{
                                    char data8[10000]="unfollow ";
                                    strcat(data8,token);
                                    strcat(data8,", ");
                                    strcat(data8,name1);
                                    strcat(data8,"\n");
                                    char *result8=send_data(data8);
                                    cJSON *root8=cJSON_Parse(result8);
                                    cJSON *message8=cJSON_GetObjectItemCaseSensitive(root8,"message");
                                    char *messag8=cJSON_Print(message8);
                                    printf("%s\n",messag8);
                                }
                            }
                        }
                    } else if (ch == 3) {
                        char data7[100]="profile ";
                        strcat(data7,token);
                        strcat(data7,"\n");
                        char *result3=send_data(data7);
                        cJSON *root4=cJSON_Parse(result3);
                        cJSON *message4=cJSON_GetObjectItemCaseSensitive(root4,"message");
                        char *messag7=cJSON_Print(message4);
                        cJSON *root3=cJSON_Parse(messag7);
                        cJSON *user1=cJSON_GetObjectItemCaseSensitive(root3,"username");
                        cJSON *bio2=cJSON_GetObjectItemCaseSensitive(root3,"bio");
                        cJSON *followers1=cJSON_GetObjectItemCaseSensitive(root3,"numberOfFollowers");
                        cJSON *followings1=cJSON_GetObjectItemCaseSensitive(root3,"numberOfFollowings");
                        cJSON *tweets=cJSON_GetObjectItemCaseSensitive(root3,"allTweets");
                        char *user2=cJSON_Print(user1);
                        char *bioo=cJSON_Print(bio2);
                        char *follo1=cJSON_Print(followers1);
                        char *follo2=cJSON_Print(followings1);
                        char *tweet=cJSON_Print(tweets);
                        printf("username:%s\nbio:%s\nfollowers:%s   followings:%s\n",user2,bioo,follo1,follo2);
                        tweet_parser(tweet);
                    } else if (ch == 4) {
                         int ch3=personal();
                         if(ch3==1){
                             char *bio[1000];
                             printf("set your bio\n");
                             scanf("%*c%[^\n]s",bio);
                             char data4[10000]="set bio ";
                             strcat(data4,token);
                             strcat(data4,", ");
                             strcat(data4,bio);
                             strcat(data4,"\n");
                             char *result4=send_data(data4);
                             cJSON *root4=cJSON_Parse(result4);
                             cJSON *message4=cJSON_GetObjectItemCaseSensitive(root4,"message");
                             char *messag4=cJSON_Print(message4);
                             printf("%s\n",messag4);
                         }
                         else if(ch3==2){
                             char pass1[100],pass2[100];
                             char data6[1000]="change password ";
                             strcat(data6,token);
                             printf("old pass:\n");
                             scanf("%*c%[^\n]s",pass1);
                             strcat(data6,", ");
                             strcat(data6,pass1);
                             printf("new pass:\n");
                             scanf("%*c%[^\n]s",pass2);
                             if(strlen(pass2)>16){
                                 printf("password too long please try again.\n");
                                 continue;
                             }
                             strcat(data6,", ");
                             strcat(data6,pass2);
                             strcat(data6,"\n");
                             char *result4=send_data(data6);
                             cJSON *root4=cJSON_Parse(result4);
                             cJSON *message4=cJSON_GetObjectItemCaseSensitive(root4,"message");
                             char *messag4=cJSON_Print(message4);
                             printf("%s\n",messag4);
                         }
                    }
                }
                if(ch==5){
                    char data5[10000]="logout ";
                    strcat(data5,token);
                    strcat(data5,"\n");
                    send_data(data5);
                }
            }
            else{
                printf("%s\n",messag);
            }
        }
        if(check==2){
            string = signup();
            char str[100];
            for(int i=0;i<100;i++)
            {
                if(string[i]!='}')
                {
                    str[i]=string[i];
                }
                else {
                    str[i] = string[i];
                    string[i + 1] = '\0';
                    break;
                }
            }
            cJSON *root=cJSON_Parse(str);
            cJSON *type=cJSON_GetObjectItemCaseSensitive(root,"type");
            cJSON *message=cJSON_GetObjectItemCaseSensitive(root,"message");
            char *typ=cJSON_Print(type);
            char *messag=cJSON_Print(message);
            if(strcmp(typ,"\"Successful\"")==0)
            {
                printf("%s\n",typ);
            }
            else{
                printf("%s\n %s\n",typ,messag);
            }
        }
    }
}
void parse_tw_com(char *data,char *id){
    int a=strlen(data);
    int d=0;
    for(int i=0;i<a;i++){
        if(data[i]=='{')
            d+=1;
    }
    int cou=1;
    for(int i=0;i<d/2;i++){
        char *tweet=malloc(10000);
        memset(tweet,0,10000);
        int flag=0;
        int cou1=0;
        while(flag<2){
            if(data[cou]=='}')
                flag+=1;
            tweet[cou1]=data[cou];
            cou+=1;
            cou1+=1;
        }
        cou+=2;
        tweet[cou1]="\0";
        cJSON *root5=cJSON_Parse(tweet);
        cJSON *id1=cJSON_GetObjectItemCaseSensitive(root5,"id");
        char *id2=cJSON_Print(id1);
        if(strcmp(id,id2)==0){
            show_com(tweet);
            return;
       }
    }
    printf("this id dont exist\n");
}
int show_com(char *data){
    cJSON *root5=cJSON_Parse(data);
    cJSON *comments1=cJSON_GetObjectItemCaseSensitive(root5,"comments");
    char *comments=cJSON_Print(comments1);
    int hold=parse_com(comments);
    if(hold==0){
        printf("dont have a comment\n");
        return 2;
    }
    for(int i=0;i<hold;i++){
        cJSON *com=cJSON_GetArrayItem(comments1,i);
        printf("%s:%s\n",com->string,com->valuestring);
    }
}
void parse_tweets(char *result9){
    cJSON *root4=cJSON_Parse(result9);
    cJSON *message4=cJSON_GetObjectItemCaseSensitive(root4,"message");
    cJSON *tee=cJSON_GetArraySize(message4);
    for(int i=0;i<tee;i++){
        cJSON *tweet=cJSON_GetArrayItem(message4,i);
        char *tweeet=cJSON_Print(tweet);
        parser(tweeet);
    }
}
void tweet_parser(char *messagee4){
    cJSON *message4=cJSON_Parse(messagee4);
    cJSON *tee=cJSON_GetArraySize(message4);
    for(int i=0;i<tee;i++){
        cJSON *tweet=cJSON_GetArrayItem(message4,i);
        char *tweeet=cJSON_Print(tweet);
        parser(tweeet);
    }
}
void parser(char *data){
    cJSON *root5=cJSON_Parse(data);
    cJSON *id1=cJSON_GetObjectItemCaseSensitive(root5,"id");
    cJSON *author1=cJSON_GetObjectItemCaseSensitive(root5,"author");
    cJSON *content1=cJSON_GetObjectItemCaseSensitive(root5,"content");
    cJSON *comments1=cJSON_GetObjectItemCaseSensitive(root5,"comments");
    cJSON *likes1=cJSON_GetObjectItemCaseSensitive(root5,"likes");
    char *id=cJSON_Print(id1);
    char *author=cJSON_Print(author1);
    char *content=cJSON_Print(content1);
    char *comments=cJSON_Print(comments1);
    char *likes=cJSON_Print(likes1);
    int hold=parse_com(comments);
    printf("user:%s    id:%s\n%s\nLikes:%s    comments:%d\n",author,id,content,likes,hold);
}
int parse_com(char *comment){
    int flag=strlen(comment);
    int k=0;
    for(int i=0;i<flag-1;i++)
    {
        if(comment[i]==':'){
            if(comment[i-1]=='\"')
                k+=1;
        }
    }
    return k;
}
int personal()
{
    int check;
    printf("1.Set(Change) bio\n");
    printf("2.Change password\n");
    printf("3.Back\n");
    scanf("%d",&check);
    return check;
}
int menu()
{
    int check;
    printf("1.Timeline\n");
    printf("2.Search\n");
    printf("3.Tweet Profile\n");
    printf("4.Personal area\n");
    printf("5.Log out\n");
    scanf("%d",&check);
    return check;
}
char *search1(char *token,char *name)
{
    char data2[10000]="search ";
    strcat(data2,token);
    strcat(data2,", ");
    strcat(data2,name);
    strcat(data2,"\n");
    char *result=malloc(10000);
    result=send_data(data2);
    return result;
}
int timeline()
{
    int check;
    printf("1.Send Tweet\n");
    printf("2.Refresh\n");
    printf("3.Back\n");
    scanf("%d",&check);
    return check;
}
int firstmenu()
{
    int check;
    printf("1.Log in\n");
    printf("2.create account (If you do not have an account)\n");
    printf("3.Exit\n");
    scanf("%d",&check);
    return check;
}
char *signup()
{
    char name[100],pass[100],pass2[100];
    printf("Set your Username:\n");
    scanf("%*c%[^\n]s",name);
    printf("Set your password:\n");
    scanf("%*c%[^\n]s",pass);
    if(strlen(pass)>16){
        printf("this password too long try again.\n");
        return signup();
    }
    char data[1000]="signup ";
    strcat(data,name);
    strcat(data,", ");
    strcat(data,pass);
    strcat(data,"\n");
    char *rec=send_data(data);
    return rec;
}
char *login()
{
    char name[100],pass[100];
    printf("Username:\n");
    scanf("%s",name);
    printf("Password:\n");
    scanf("%s",pass);
    char data[1000]="login ";
    strcat(data,name);
    strcat(data,", ");
    strcat(data,pass);
    strcat(data,"\n");
    char *rec=send_data(data);
    return rec;

}
int initialize(int port)
{
    struct WSAData wsadata;
    int wsastartup=WSAStartup(MAKEWORD(2,2), &wsadata);
    if(wsastartup!=0)
    {
    printf("Eror");
    return -1;
    }
    server.sin_family= AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port=htons(port);
    return 0;
}
char *send_data(char *data){
    char *buf=malloc(100000);
    memset(buf,0,100000);
    int clientsock=socket(AF_INET, SOCK_STREAM,0);
    if(clientsock==INVALID_SOCKET){
        return buf;
    }
    int can_connect=connect(clientsock,(struct sockaddr *)&server, sizeof(server));
    if(can_connect!=0){
        return buf;
    }
    send(clientsock,data,strlen(data),0);
    recv(clientsock,buf,100000,0);
    closesocket(clientsock);
    return buf;
}