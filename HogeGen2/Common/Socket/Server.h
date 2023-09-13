#pragma once

#include <../MessageOutputter/MessageOutputter.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <vector>
#include <functional>
#include <cstring>

/// @brief サーバークラス
/// @details 参考: http://www.eyes-software.co.jp/news/archives/7
namespace HogeGen2{

class Server {
    inline static const int SRV_SEND_BUFF = 1024;
    inline static const int SRV_RECV_BUFF = 1024;
    
    inline static bool continueFlag = false;
    
    int server_fd;
    
    std::vector<int> client_fds;
    
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    socklen_t sin_size = sizeof(struct sockaddr_in);
    
    std::function<void(Server*, char*, int, int)> receive_proc;

    char tx_buff[SRV_SEND_BUFF];

    
    pthread_t temp_thread;

    /// @brief 送信関数
    /// @param fd 送信対象のクライアントのファイルディスクリプタ
    /// @param tx_data 送信バッファ
    /// @param tx_size 送信バッファサイズ
    /// @return エラーの有無
    /// @details データの送信を行う関数
    int Transmit(int fd, const char* tx_data, size_t tx_size) {
        int ret;
        if((ret = send(fd, tx_data, tx_size, 0)) < 0) {
            log_output.ErrorMessage("send: %s", std::strerror(errno));
            log_output.InfoMessage("Server transmit fd = %d", fd);
            return -1;
        }else if(ret == 0){
            log_output.InfoMessage("Disconnected");
            return 1;
        }
        return 0;
    }

public:
    /// @brief 送受信スレッド関数
    /// @param p サーバーのインスタンス
    /// @details ソケットのファイルディスクリプタに変更があるかどうかをselect関数を使用して判定、送信処理、受信処理を行う。呼ばないであげて
    static void* CommThread(void* p) {
        Server* server = (Server*)p;    // サーバーのインスタンス
        char buf[SRV_RECV_BUFF];        // 受信バッファ
        int     max_fd;                 // ディスクリプタの最大値
        fd_set  rfds;                   // 接続待ち、受信待ちをするディスクリプタの集合
        struct timeval  tv;             // タイムアウト時間
        socklen_t len = sizeof(struct sockaddr_in);
        struct sockaddr_in from_addr;
        int cnt;

        while (continueFlag){
            // 接続待ちのディスクリプタをディスクリプタ集合に設定する
            FD_ZERO(&rfds);
            FD_SET(server->server_fd, &rfds);
            max_fd = server->server_fd;
            // 受信待ちのディスクリプタをディスクリプタ集合に設定する
            for (int i = 0; i < (int)server->client_fds.size(); i++){
                if (server->client_fds[i] != -1){
                    FD_SET(server->client_fds[i], &rfds);
                    if (server->client_fds[i] > max_fd) max_fd = server->client_fds[i];
                }
            }

            // タイムアウト時間を0に指定する。
            tv.tv_sec = 0;
            tv.tv_usec = 1000;

            // 接続＆受信を待ち受ける
            cnt = select(max_fd+1, &rfds, NULL, NULL, &tv);
            if (cnt < 0){
                // シグナル受信によるselect終了の場合、再度待ち受けに戻る
                if (errno == EINTR) continue;
                // その他のエラーの場合、終了する。
                goto end;
            } else if (cnt == 0) {
                // タイムアウトした場合、再度待ち受けに戻る
                continue;
            } else {
                // 接続待ちディスクリプタに接続があったかを調べる
                if ( FD_ISSET(server->server_fd, &rfds)){
                    // 接続されたならクライアントからの接続を確立する
                    int client_fd = -1;
                    if ((client_fd = accept(server->server_fd, (struct sockaddr *)&from_addr, &len)) < 0 ) {
                        goto end;
                    }
                    log_output.InfoMessage("socket:%d  connected.", client_fd);
                    server->client_fds.push_back(client_fd);
                }
                for (int i = 0; i < (int)server->client_fds.size(); i++){
                    // 受信待ちディスクリプタにデータがあるかを調べる
                    if ( FD_ISSET(server->client_fds[i], &rfds)){
                        // データがあるならパケット受信する
                        cnt = recv(server->client_fds[i], buf, sizeof(buf), 0);
                        if (cnt > 0) {
                            // パケット受信成功の場合
                            if(server->receive_proc != nullptr){
                                server->receive_proc(server, buf, cnt, server->client_fds[i]);
                                memset(buf, 0, sizeof(buf));
                            }
                        } else if (cnt == 0) {
                            // 切断された場合、クローズする
                            log_output.InfoMessage("socket:%d  disconnected.", server->client_fds[i]);
                            close(server->client_fds[i]);
                            server->client_fds.erase(server->client_fds.begin() + i);
                        } else {
                            log_output.ErrorMessage("read: %s", std::strerror(errno));
                            goto end;
                        }
                    }
                }
            }
        }
        end:
        // パケット送受信用ソケットクローズ
        for (int i = 0; i < (int)server->client_fds.size(); i++ ){
            close(server->client_fds[i]);
        }
        server->client_fds.clear();
        // 接続要求待ち受け用ソケットクローズ
        close(server->server_fd);
        server->server_fd = -1;
        log_output.InfoMessage("Server close.");
        return nullptr;
    }
    
    /// @brief コンストラクタ
    Server() {
        this->receive_proc = nullptr;
    }

    /// @brief デストラクタ
    ~Server() {

    }
    
    /// @brief 初期化関数
    /// @param f 受信ハンドラ
    /// @param port 接続させるポート番号
    /// @details ソケットを生成する、クライアンを待つスレッドを生成する
    int Init(int port, std::function<void(Server*, char*, int, int)> f) {
        if((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            log_output.ErrorMessage("socket: %s", std::strerror(errno));
            return 1;
        }
        this->server_addr.sin_family = AF_INET;
        this->server_addr.sin_port = htons(port);
        this->server_addr.sin_addr.s_addr = INADDR_ANY;
        if(bind(this->server_fd, (struct sockaddr *)&this->server_addr, sizeof(this->server_addr)) < 0) {
            log_output.ErrorMessage("bind: %s", std::strerror(errno));
            return 2;
        }
        if(listen(this->server_fd, 1) < 0) {
            log_output.ErrorMessage("listen: %s", std::strerror(errno));
            return 3;
        }

        log_output.InfoMessage("Init Server. port = %d", port);

        // 受信、acceptのスレッドを作成する
        // pthread_t temp_thread;
        int thread_create = 1;
        if( (thread_create = pthread_create(&temp_thread, nullptr, Server::CommThread, this)) != 0) {
            log_output.ErrorMessage("pthread_create: %s", std::strerror(errno));
            return 4;
        }else{
            // if (pthread_detach(temp_thread) != 0) {
            //     log_output.ErrorMessage("pthread_detach: %s", std::strerror(errno));
            //     return 5;
            // }else{
            //     this->receive_proc = f;
            //     continueFlag = true;
            //     log_output.InfoMessage("Server start.");
            // }
            this->receive_proc = f;
            continueFlag = true;
            log_output.InfoMessage("Server start.");
        }
        return 0;
    }
    
    /// @brief 文字列送信関数
    /// @param to_fd 送信対象のクライアントのファイルディスクリプタ
    /// @param format フォーマット指定子
    /// @param ... フォーマット指定子に対応する変数
    /// @return 送信した文字数
    /// @details to_fd が２以下のとき、全クライアントに送信する。文字列を送信関数を通して送信する、printfのように使える
    int SendStr(int to_fd,const char* format, ...) {
        int ret = 0;
        va_list ap;
        va_start( ap, format );
        vsnprintf(this->tx_buff, SRV_SEND_BUFF, format, ap);
        if(to_fd < 3){
            for (int i = 0; i < (int)client_fds.size(); i++){
                this->Transmit(client_fds[i], tx_buff, sizeof(tx_buff));
            }
            memset(tx_buff, 0, sizeof(tx_buff));
        }else{
            ret = this->Transmit(to_fd, tx_buff, sizeof(tx_buff));
            memset(tx_buff, 0, sizeof(tx_buff));
        }
        va_end( ap );
        return ret;
    }    

    int SendData(int to_fd, std::vector<char> data) {
        int ret = 0;
        if(to_fd < 3){
            for (int i = 0; i < (int)client_fds.size(); i++){
                this->Transmit(client_fds[i], data.data(), data.size());
            }
        }else{
            ret = this->Transmit(to_fd, data.data(), data.size());
        }
        return ret;
    }

    int SendData(int to_fd, uint8_t *data, int size) {
        int ret = 0;
        if(to_fd < 3){
            for (int i = 0; i < (int)client_fds.size(); i++){
                this->Transmit(client_fds[i], (char*)data, size);
            }
        }else{
            ret = this->Transmit(to_fd, (char*)data, size);
        }
        return ret;
    }
    
    void Close() {
        continueFlag = false;
        pthread_join(temp_thread, nullptr);
        temp_thread = -1;
    }
};

}