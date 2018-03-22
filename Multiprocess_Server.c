#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"

#define PORT 4444
#define BUF_SIZE 2000
#define CLADDR_LEN 100

static int count = 1;   // 원래 0부터 시작해야합니다.

void main()
{
        struct sockaddr_in addr, cl_addr;
        int sockfd, len, ret, newsockfd;
        char buffer[BUF_SIZE];
        pid_t childpid; //멀티 프로세서를 위한 변수
        char clientAddr[CLADDR_LEN];
        int i=0;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);       //소켓 연결
        int sockList[5];
        int check = 0;

        if (sockfd < 0) {
                printf("Error creating socket!\n");
                exit(1);
        }
        printf("Socket created...\n");

        memset(&addr, 0, sizeof(addr)); // 소켓연결 셋팅
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = PORT;

        ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));    //바인드설정(값을먼저받아야함)
        if (ret < 0) {
                printf("Error binding!\n");
                exit(1);
        }
        printf("Binding done...\n");


        for (;;) { //infinite loop
                len = sizeof(cl_addr);  // 길이
                newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len); // 클라이언트를받습니다.
//              printf("%d번째 newsockfd : %d\n", count, newsockfd);
                //for(i=0; i<count; i++) {
                //      if(newsockfd == sockList[i]){
                //              check = 1;
                //}                     // 다른 ip라면 다른 newsockfd이 들어가지만,
                                        // 같은 호스트에선 같은값이들어가여 주석처리했습니다.

        //      if(check == 0) {
        //              sockList[count] = newsockfd;    // 같은값이 없을 때 리스트추가
        //              count++;
        //      }

                check = 0;      // false형


                if (newsockfd < 0) {
                        printf("Error accepting connection!\n");
                        exit(1);
                }

                printf("Connection accepted...\n");

                inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);        // IPAddress
                if ((childpid = fork()) == 0) { //creating a child process

                        close(sockfd);
                        //stop listening for new connections by the main process. 
                        //the child will continue to listen. 
                        //the main process now handles the connected client.

                        for (;;) {
                                // 받아서, 내용과 IP를 출력해주는 부분
                                memset(buffer, 0, BUF_SIZE);
                                ret = recvfrom(newsockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, &len);
                                if (ret < 0) {
                                        printf("Error receiving data!\n");
                                        exit(1);
                                }
                                printf("Received data from IP Address : %s\n", clientAddr);
                                printf("Contents : %s\n", buffer);

                                for(i=0; i<count; i++) {
                                        ret = sendto(sockList[i], buffer, BUF_SIZE, 0, (struct sockaddr *) &cl_addr, len);
                                        if (ret < 0) {
                                                printf("Error sending data!\n");
                                                exit(1);
                                        }
                                        printf("Sent data to %s: %s\n", clientAddr, buffer);
                                }
                        }
                }
                close(newsockfd);
        }
}
                  