#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
using std::cout;
using std::endl;
using std::thread;

#define PORT 9999

/*
각 함수에 대하여
https://blog.naver.com/PostView.naver?blogId=ifkiller&logNo=70081005002

예를 들면,

4. 클라이언트로부터 접속이 되면 accept()함수를 호출해 
   클라이언트 연결에 대한 새로운 소켓을 생성한다. 
*/

// 2인 접속
void counter(int server_fd, struct sockaddr_in address){
    int new_socket;
    int addrlen = sizeof(address);
    new_socket=accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    const char* complete = "연동 완료 메세지";
    send(new_socket, complete, strlen(complete), 0);
}
int main(){
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    cout<<"소켓 디스크립터:"<<server_fd<<endl;

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    cout<<opt<<endl;

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    cout<<"complete"<<endl;

    // 소켓에 액세스하는 파일 디스크립터를 포트번호를 가진 애플리케이션 주소 포인터와 바인딩
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    cout<<"binding complete"<<endl;

    listen(server_fd, 2);

    /*
    //클라이언트로부터 접속이 되면 accept()함수를 호출해 클라이언트 연결에 대한 새로운 소켓을 생성한다.
    //여기서부턴 스레드로 묶을 수 있는지 확인.
    int new_socket;
    int addrlen = sizeof(address);
    new_socket=accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    const char* complete = "연동 완료 메세지";
    send(new_socket, complete, strlen(complete), 0);
    */

    thread t1(counter, server_fd, address);
    thread t2(counter, server_fd, address);

    t1.join();
    t2.join();

    return 0;
}