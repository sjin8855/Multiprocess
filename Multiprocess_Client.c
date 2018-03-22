#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"netdb.h"

#define PORT 4444       // 포트번호 
#define BUF_SIZE 2000   // 입력받을수있는 사이즈

int main(int argc, char**argv) {
        struct sockaddr_in addr, cl_addr;
        int sockfd, ret;
        char buffer[BUF_SIZE];
        struct hostent * server;
        char * serverAddr;
        pid_t pid;

        if (argc < 2) { // ip주소 입력받아야합니다/
                printf("usage: client < ip address >\n");
                exit(1);
        }

        serverAddr = argv[1];   // ip주소 입력

        sockfd = socket(AF_INET, SOCK_STREAM, 0);       // 소켓연결
        if (sockfd < 0) {       // 실패시
                printf("Error creating socket!\n");
                exit(1);
        }
        printf("Socket created...\n");  // 소켓생성

        memset(&addr, 0, sizeof(addr)); // 값 지정
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(serverAddr);
        addr.sin_port = PORT;

        ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)); // 서버, 클라이언트 연결
        if (ret < 0) {  // 실패시
                printf("Error connecting to the server!\n");
                exit(1);

        memset(buffer, 0, BUF_SIZE);
        printf("Enter your message(s): ");

        while (fgets(buffer, BUF_SIZE, stdin) != NULL ) {       // 입력받을때마다 신호
                // 입력값 전해주고
                ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));
                if (ret < 0) {
                        printf("Error sending data!\n\t-%s", buffer);
                }
                //같은 값 처리
                ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
                if (ret < 0) {
                        printf("Error receiving data!\n");
                }
                else {
                        printf("Received: ");
                        fputs(buffer, stdout);
                        printf("\n");
                }
        }

        프로세스로 fork하여 자식프로세스에는 입력값을 처리하고, 부모프로세스에서는 서버에서 입력받은 값
        을 처리하는 것을 만들고 싶었지만, 하는 과정에서 무한루프와 기술적 부족으로 인해 구현하지 못하고
        서버와 프로세스간의 1:1통신으로 메세지와 보낸사람의 IP까지를 조사할 수 있는 프로젝트를
        개발하였습니다.

*/

/*      
        if((pid = fork()) < 0) {
                perror("fork");
                exit(1);
        }

        else if(pid == 0) {
                while (fgets(buffer, BUF_SIZE, stdin) != NULL ) {
                        ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));
                        if (ret < 0) {
                                printf("Error sending data!\n\t-%s", buffer);
                        }
                        ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
                        if (ret < 0) {
                                printf("Error receiving data!\n");
                        }
                        else {
                                printf("Received: ");
                                fputs(buffer, stdout);
                                printf("\n");
                        }
                }
        }
        


        else{
                if((ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL)) > 0) {
                        
                        //ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
                        ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));                   
                        printf("Received: ");
                        fputs(buffer, stdout);
                        printf("\n");
                }
        }

*/

        return 0;
}