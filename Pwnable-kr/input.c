#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main()
{
	char *argv[101]={"/home/input2/input",[1 ... 99]="A",NULL};
	argv['A']="\x00";
	argv['B']="\x20\x0a\x0d";
	argv['C']="9999";
	FILE* fp = fopen("\x0a", "w");
	fwrite("\x00\x00\x00\x00",4,1,fp);
	fclose(fp);
	int pipe2stdin[2] = {-1,-1};
	int pipe2stderr[2] = {-1,-1};
pid_t childpid;
if (pipe(pipe2stdin)<0||pipe(pipe2stderr)<0)
{
	printf("FATAL: cannot create pipe\n");
	exit(1);
}
if ((childpid=fork())<0)
{
	printf("FATAL: cannot fork child\n");
	exit(1);
}
else if (childpid==0)
{
	close(pipe2stdin[0]);close(pipe2stderr[0]);
	write(pipe2stdin[1],"\x00\x0a\x00\xff",4);
	write(pipe2stderr[1],"\x00\x0a\x02\xff",4);
}
else
{
	close(pipe2stdin[1]);close(pipe2stderr[1]);
	dup2(pipe2stdin[0],0);dup2(pipe2stderr[0],2);
	close(pipe2stdin[0]);close(pipe2stderr[1]);
	char* env[2]={"\xde\xad\xbe\xef=\xca\xfe\xba\xbe"};
	execve("/home/input2/input",argv,env);
}
	sleep(5);
	int sockfd;
	struct sockaddr_in server;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		perror("Cannot create the socket");
		exit(1);
	}
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(9999);
	if(connect(sockfd,(struct sockaddr*) &server,sizeof(server))<0)
	{
		 perror("Problem connecting");
         exit(1);
	}
	char buf[4]="\xde\xad\xbe\xef";
	write(sockfd,buf,4);
	close(sockfd);
	return 0;

}