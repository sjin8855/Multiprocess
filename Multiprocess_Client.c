#include"stdio.h"
#include"stdlib.h"
#include"sys/types.h"
#include"sys/socket.h"
#include"string.h"
#include"netinet/in.h"
#include"netdb.h"

#define PORT 4444       // ��Ʈ��ȣ 
#define BUF_SIZE 2000   // �Է¹������ִ� ������

int main(int argc, char**argv) {
        struct sockaddr_in addr, cl_addr;
        int sockfd, ret;
        char buffer[BUF_SIZE];
        struct hostent * server;
        char * serverAddr;
        pid_t pid;

        if (argc < 2) { // ip�ּ� �Է¹޾ƾ��մϴ�/
                printf("usage: client < ip address >\n");
                exit(1);
        }

        serverAddr = argv[1];   // ip�ּ� �Է�

        sockfd = socket(AF_INET, SOCK_STREAM, 0);       // ���Ͽ���
        if (sockfd < 0) {       // ���н�
                printf("Error creating socket!\n");
                exit(1);
        }
        printf("Socket created...\n");  // ���ϻ���

        memset(&addr, 0, sizeof(addr)); // �� ����
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(serverAddr);
        addr.sin_port = PORT;

        ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr)); // ����, Ŭ���̾�Ʈ ����
        if (ret < 0) {  // ���н�
                printf("Error connecting to the server!\n");
                exit(1);

        memset(buffer, 0, BUF_SIZE);
        printf("Enter your message(s): ");

        while (fgets(buffer, BUF_SIZE, stdin) != NULL ) {       // �Է¹��������� ��ȣ
                // �Է°� �����ְ�
                ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &addr, sizeof(addr));
                if (ret < 0) {
                        printf("Error sending data!\n\t-%s", buffer);
                }
                //���� �� ó��
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

        ���μ����� fork�Ͽ� �ڽ����μ������� �Է°��� ó���ϰ�, �θ����μ��������� �������� �Է¹��� ��
        �� ó���ϴ� ���� ����� �;�����, �ϴ� �������� ���ѷ����� ����� �������� ���� �������� ���ϰ�
        ������ ���μ������� 1:1������� �޼����� ��������� IP������ ������ �� �ִ� ������Ʈ��
        �����Ͽ����ϴ�.

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