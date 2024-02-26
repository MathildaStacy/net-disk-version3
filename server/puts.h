#ifndef _PUTS_H_
#define _PUTS_H_

int msgtrans(char *msg,int sockfd);

int msgrecv(char* msg , int sockfd);

int sha1file(const char * filename , char *sha1buf);

int putsfile(char* name , int netfd,long offset);
int getfile(int sockfd,int opfd ,long offset);

int commandPuts_C(char* filename,int sockfd);
int commandPuts_S(int sockfd);

#endif











