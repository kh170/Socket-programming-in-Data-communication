/*
 * Simple client to work with server.c program.
 * Host name and port used by server are to be
 * passed as arguments.
 *
 * To test: Open a terminal window.
 * At prompt ($ is my prompt symbol) you may
 * type the following as a test:
 *
 * $./client 127.0.0.1 54554
 * Please enter the message: Programming with sockets is fun!  
 * I got your message
 * $ 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)     &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
	
	//reading choice sent by the server
	int choice;

	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	if (n<0) error("Error reading from socket");
	printf("Server - %s\n",buffer);
	scanf("%d",&choice);
	
	//sending the choice to server
	write(sockfd,&choice,sizeof(int));
	
	if (choice==1)	
	{	
		//sending file to server
		FILE *fp;
		int words = 0;
		char c;
		fp = fopen("intext.txt","r");
		while((c = getc(fp))!=EOF)
		{
			fscanf(fp,"%s",buffer);
			if (isspace(c)||c=='\t')
				words++;
		}			
		write(sockfd,&words,sizeof(int));
		printf("Client - Sending number of words\n");
		rewind(fp);

		char ch;			
		while(ch!=EOF)
		{	
			fscanf(fp,"%s",buffer);
			write(sockfd,buffer,256);
			ch = fgetc(fp);
		}
		printf("Client - Input file has been sent\n");

		//reading output file from the server
		FILE *fp1;
		int count = 0;
		fp1 = fopen("fileUpper.txt","w");
		int words_;
		read(sockfd, &words_, sizeof(int));
		while (count!=words_)
		{
			read(sockfd,buffer,256);
			fprintf(fp1,"%s ",buffer);
			count++;
		}
		fclose(fp1);
		printf("Client - Output file has been received\n");
	}
	
	else if (choice == 2)
	{
		char sc;
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n<0) error("Error reading from socket");
		printf("Server - %s\n",buffer);
		scanf("%s",&sc);
		write(sockfd,&sc,sizeof(char));	

		FILE *fp;
		int words = 0;
		char c;
		fp = fopen("intext.txt","r");
		while((c = getc(fp))!=EOF)
		{
			fscanf(fp,"%s",buffer);
			if (isspace(c)||c=='\t')
				words++;
		}			
		write(sockfd,&words,sizeof(int));
		printf("Client - Sending number of words\n");
		rewind(fp);

		char ch;			
		while(ch!=EOF)
		{	
			fscanf(fp,"%s",buffer);
			write(sockfd,buffer,256);
			ch = fgetc(fp);
		}
		printf("Client - Input file has been sent\n");

		FILE *fp1;
		int count = 0;
		fp1 = fopen("fileChar.txt","w");
		int words_;
		read(sockfd, &words_, sizeof(int));
		while (count!=words_)
		{
			read(sockfd,buffer,256);
			fprintf(fp1,"%s ",buffer);
			count++;
		}
		fclose(fp1);
		printf("Client - Output file has been received\n");
	}
	





    close(sockfd);
    return 0;
}

