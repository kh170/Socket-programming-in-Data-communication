/* Program: server.c
 * A simple TCP server using sockets.
 * Server is executed before Client.
 * Port number is to be passed as an argument. 
 * 
 * To test: Open a terminal window.
 * At the prompt ($ is my prompt symbol) you may
 * type the following as a test:
 *
 * $ ./server 54554
 * Run client by providing host and port
 *
 * 
 */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include<ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     fprintf(stdout, "Run client by providing host and port\n");
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
        error("ERROR on accept");
     	
	//sending service list to the client
	int choice;
	n = write(newsockfd, "Enter request: \n1.toUpper\n2.count\n", strlen("Enter request: \ntoUpper\ncount\n3.Exit\n"));
	if (n<0) error("Error in writing to socket");
	
	//reading client's choice of service
	read(newsockfd, &choice, sizeof(int));
	printf("Server - Choice is %d\n",choice);
	
	if (choice == 1)
	{
		//Reading file from the client		
		FILE *fp1;
		int count = 0;
		fp1 = fopen("fileBuffer.txt","w");
		int words;
		read(newsockfd, &words, sizeof(int));
		while (count!=words)
		{
			read(newsockfd,buffer,256);
			fprintf(fp1,"%s ",buffer);
			count++;
		}
		fclose(fp1);
		printf("Server - File has been received\n");
		
		//Server completing request				
		FILE *fp2;
		FILE *fp3;
		char ch;
		fp2 = fopen("fileBuffer.txt","r");
		fp3 = fopen("server_fileUpper.txt","w");
		while((ch=fgetc(fp2))!=EOF)
		{
			if(islower(ch))
				ch = ch - 32;
			fprintf(fp3,"%c",ch);
		}
		fclose(fp2);
		fclose(fp3);
		printf("Server - Request has been completed\n");	
		
		//Sending the request output to the client
		FILE *fp4;
		int word_ = 0;
		char read_ch;
		fp4 = fopen("server_fileUpper.txt","r");
		while((read_ch = getc(fp4))!=EOF)
		{
			fscanf(fp4,"%s",buffer);
			if (isspace(read_ch)||read_ch=='\t')
				word_++;
		}			
		write(newsockfd,&word_,sizeof(int));
		rewind(fp4);

		char send_ch;			
		while(send_ch!=EOF)
		{	
			fscanf(fp4,"%s",buffer);
			write(newsockfd,buffer,256);
			send_ch = fgetc(fp4);
		}	
		fclose(fp4);
		printf("Server - Output file has been sent\n");
	}
	
	else if(choice == 2)
	{
		//reading the character to be counted
		char sc;
		n = write(newsockfd, "Enter the character to be counted: \n", strlen("Enter the character to be counted: \n"));
		if (n<0) error("Error in writing to socket");
		read(newsockfd, &sc, sizeof(char));
		printf("Client - Character is %c\n",sc);
		
		//reading the file from the client
		FILE *fp1;
		int count = 0;
		fp1 = fopen("fileBuffer.txt","w");
		int words;
		read(newsockfd, &words, sizeof(int));
		while (count!=words)
		{
			read(newsockfd,buffer,256);
			fprintf(fp1,"%s ",buffer);
			count++;
		}
		fclose(fp1);
		printf("Server - File has been received\n");

		//Server completing request
		FILE *fp2;
		FILE *fp3;
		char ch;
		int counter = 0;
		fp2 = fopen("fileBuffer.txt","r");
		fp3 = fopen("server_fileChar.txt","w");
		while((ch=fgetc(fp2))!=EOF)
		{
			if(ch == sc)
				counter++;
			
		}
		fprintf(fp3,"Count of %c:%d\n",sc,counter);
		fclose(fp2);
		fclose(fp3);
		printf("Server - Request has been completed\n");	

		//Sending the request output to the client
		FILE *fp4;		
		int word_ = 0;
		char read_ch;
		fp4 = fopen("server_fileChar.txt","r");
		while((read_ch = getc(fp4))!=EOF)
		{
			fscanf(fp4,"%s",buffer);
			if (isspace(read_ch)||read_ch=='\t')
				word_++;
		}			
		write(newsockfd,&word_,sizeof(int));
		printf("Server - Sending number of words\n");
		rewind(fp4);

		char send_ch;			
		while(send_ch!=EOF)
		{	
			fscanf(fp4,"%s",buffer);
			write(newsockfd,buffer,256);
			send_ch = fgetc(fp4);
		}
		printf("Server - Output File has been sent\n");

	}
		



     close(newsockfd);
     close(sockfd);
     return 0; 
}

