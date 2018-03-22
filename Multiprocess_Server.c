#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"

#define PORT 4444
#define BUF_SIZE 2000
#define CLADDR_LEN 100

static int count = 1;   // ���� 0���� �����ؾ��մϴ�.

void main()
{
        struct sockaddr_in addr, cl_addr;
        int sockfd, len, ret, newsockfd;
        char buffer[BUF_SIZE];
        pid_t childpid; //��Ƽ ���μ����� ���� ����
        char clientAddr[CLADDR_LEN];
        int i=0;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);       //���� ����
        int sockList[5];
        int check = 0;

        if (sockfd < 0) {
                printf("Error creating socket!\n");
                exit(1);
        }
        printf("Socket created...\n");

        memset(&addr, 0, sizeof(addr)); // ���Ͽ��� ����
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = PORT;

        ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));    //���ε弳��(���������޾ƾ���)
        if (ret < 0) {
                printf("Error binding!\n");
                exit(1);
        }
        printf("Binding done...\n");


        for (;;) { //infinite loop
                len = sizeof(cl_addr);  // ����
                newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len); // Ŭ���̾�Ʈ���޽��ϴ�.
//              printf("%d��° newsockfd : %d\n", count, newsockfd);
                //for(i=0; i<count; i++) {
                //      if(newsockfd == sockList[i]){
                //              check = 1;
                //}                     // �ٸ� ip��� �ٸ� newsockfd�� ������,
                                        // ���� ȣ��Ʈ���� �������̵��� �ּ�ó���߽��ϴ�.

        //      if(check == 0) {
        //              sockList[count] = newsockfd;    // �������� ���� �� ����Ʈ�߰�
        //              count++;
        //      }

                check = 0;      // false��


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
                                // �޾Ƽ�, ����� IP�� ������ִ� �κ�
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
                  