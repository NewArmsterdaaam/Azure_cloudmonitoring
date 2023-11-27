#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 9999

/*
client1
*/

int main(int argc, char const *argv[]){
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    const char* hello = "서버에서 전송합니다";
    const char* monitor = "자원을 모니터링합니다";
    const char* complete = "연동 완료 메세지";
    char buffer[1024] = {0};

    if((sock = socket(AF_INET, SOCK_STREAM, 0))<0){
        std::cerr<<"Socket creation error"<<std::endl;
        return -1;
    }
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(PORT);

    if(inet_pton(AF_INET, "210.110.39.83", &serv_addr.sin_addr)<=0){
        std::cout<<"address not supported"<<std::endl;
        return -1;
    }
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
        std::cout<<"connetction failed"<<std::endl;
        return -1;
    }
    //send(sock, hello, strlen(hello),0);

    //std::cout<<"hello mmesage sent"<<std::endl;

    /*
    int i = 0;
    while(i<5){
        valread = read(sock, buffer, 1024);
        std::cout<<buffer<<" - 서버에서 온 메세지, 어플리케이션 차원에서 폐기"<<std::endl;
        i++;
    }
    */

    valread = read(sock, buffer, 1024);

    if (strcmp(buffer, hello)==0){
        int i = 1;
        int control = 0;
        do { 
            if (control == 0){
                std::cout<<buffer<<" - 서버에서 온 메세지, 어플리케이션 차원에서 폐기"<<std::endl;
                control++;
            } else{
                valread = read(sock, buffer, 1024);
                std::cout<<buffer<<" - 서버에서 온 메세지, 어플리케이션 차원에서 폐기"<<std::endl;
                i++;
            }
        }while (i<5);

        /* //do-while문으로 변형

        std::cout<<buffer<<" - 서버에서 온 메세지, 어플리케이션 차원에서 폐기"<<std::endl;
        
        int i = 0;
        while(i<4){
        valread = read(sock, buffer, 1024);
        std::cout<<buffer<<" - 서버에서 온 메세지, 어플리케이션 차원에서 폐기"<<std::endl;
        i++;
        }

        */
    } else if (strcmp(buffer, monitor)==0){
        std::cout << "file network" << std::endl;

        std::cout << "cpu, mem : complete" << std::endl;
        system("top -b -n 1 > FILE1");

        std::cout << "disk : complete" << std::endl;
        system("du > FILE2");

        system("cat FILE1 FILE2 > FILE");

        std::cout << "모니터링 파일 작성 완료" << std::endl;
        std::cout << "서버로 파일 제출" << std::endl;

        size_t file_size;
        char buffer[1024];
        FILE *fp;

        fp = fopen("./FILE", "rb");
        // file size
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        size_t size = htonl(file_size);
        
        //1024씩 전송
        int nsize=0;
        while(nsize != file_size){
            int fpsize = fread(buffer, 1, 1024, fp);
            nsize += fpsize;
            send(sock, buffer, fpsize, 0);
        }
        fclose(fp);

        std::cout << "파일 제출 완료" << std::endl;
    } else if (strcmp(buffer, complete)==0){
        std::cout << "2차 과제 연동 완료" << std::endl;
    } else {
        std::cerr << "network content errorw" << std::endl; 
        return -1;
    }

    return 0;
}