#include <openssl/md5.h>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

#define QUEUE_SIZE 5

map<string,int> dictionary;

string get_work(int number){
    stringstream i2s;
    i2s<<number;
    string input;
    i2s>>input;
    unsigned char buf[MD5_DIGEST_LENGTH];
    MD5((const unsigned char*)input.c_str(),input.size(),buf);
    stringstream ss;
    for(int i=0;i<4;++i) ss<<hex<<setw(2)<<setfill('0')<<(int)buf[i];
    cout<<number<<" "<<ss.str()<<endl;
    return ss.str();
}

int main(){
    // Create dictionary
    for(int i=0;i<16777216;++i) dictionary[get_work(i)] = i;
    cout<<"Init Fine\n";
    // Create Socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd <0){
        cout<<"Error"<<endl;
        return -1;
    }
    cout<<sockfd<<endl;
    // Bind config
    sockaddr_in config;
    config.sin_family = AF_INET;
    config.sin_addr.s_addr = INADDR_ANY;
    config.sin_port = htons(8877);
    if (bind(sockfd, (sockaddr*) &config,sizeof(config)) <0){
        cout<<"Bind Error"<<endl;
        return -1;
    }
    // Listen on port 88771
    if (listen(sockfd, QUEUE_SIZE) < 0){
        cout<<"Listen Error"<<endl;
        return -1;
    }
    // wait-loop
    auto addr_len = sizeof(config);
    cout<<"Start Listening on "<<8877<<endl;
    for(;;){
        int clientfd = accept(sockfd,(sockaddr*)&config,(socklen_t*)&addr_len);
        cout<<"Have Connection, fd="<<clientfd<<endl;
        char buffer[10];
        recv(clientfd, buffer, sizeof(buffer),0);
        string query = "";
        for(int i =0;i<8;++i){
            char c = buffer[i];
            query += c;
        }
        cout<<"Get query="<<query<<", answer= "<<dictionary[query]<<endl;
        string ret = to_string(dictionary[query]);
        send(clientfd,ret.c_str(),ret.size(),0);
        shutdown(clientfd,SHUT_RDWR);
        close(clientfd);
        cout<<"Close connection"<<endl;
    }
    cout<<"88"<<endl;
    close(sockfd);
    return 0;
}