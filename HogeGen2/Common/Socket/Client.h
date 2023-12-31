#pragma once

#include <../MessageOutputter/MessageOutputter.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <vector>
#include <functional>

namespace HogeGen2 {

/// @brief クライアントクラス
class Client {
    inline static const int RECV_BUFF = 1024;
    inline static const int SEND_BUFF = 1024;

    inline static bool continueFlag = false;
    
    int server_fd;
    
    struct sockaddr_in server_addr;
    
    // void (*receive_proc)(Client*, char*, int);
    std::function<void(Client*, uint8_t*, int)> receive_proc;
    
    char tx_buff[SEND_BUFF];
    
    pthread_t temp_thread;
    
    bool f_init;
    bool f_connect;

    /// @brief 送信関数
    /// @param tx_data 送信バッファ
    /// @param tx_size 送信バッファサイズ
    /// @return 送信した文字数
    /// @details データの送信を行う関数。呼ばないであげて
    int Transmit(const char* tx_data, size_t tx_size) {
        int ret;
        if(f_init) {
            if((ret = send(this->server_fd, tx_data, tx_size, 0)) <= 0) {
                log_output.ErrorMessage("send: %s", std::strerror(errno));
                return -1;
            }
            return ret;
        }else{
            return -1;
        }
    }

public:
    /// @brief 送受信スレッド関数
    /// @param p クライアントのインスタンス
    /// @details ソケットのファイルディスクリプタに変更があるかどうかをselect関数を使用して判定、送信処理、受信処理を行う。呼ばないであげて
    static void* CommThread(void* p){
        Client* client = (Client*)p;    // サーバーのインスタンス
        uint8_t buf[RECV_BUFF];         // 受信バッファ
        int max_fd;                     // ディスクリプタの最大値
        fd_set  client_fds;             // 接続待ち、受信待ちをするディスクリプタの集合
        struct timeval  tv;             // タイムアウト時間
        int cnt;

        while (continueFlag){
            // 受信待ちのディスクリプタをディスクリプタ集合に設定する
            FD_ZERO(&client_fds);
            FD_SET(client->server_fd, &client_fds);
            max_fd = client->server_fd;

            // タイムアウト時間を0に指定する。
            tv.tv_sec = 0;
            tv.tv_usec = 1000;

            // 受信を待ち受ける
            cnt = select(max_fd+1, &client_fds, NULL, NULL, &tv);
            if (cnt < 0){
                // シグナル受信によるselect終了の場合、再度待ち受けに戻る
                if (errno == EINTR) continue;
                // その他のエラーの場合、終了する。
                goto end;
            } else if (cnt == 0) {
                // タイムアウトした場合、再度待ち受けに戻る
                continue;
            } else {
                // 受信待ちディスクリプタにデータがあるかを調べる
                if ( FD_ISSET(client->server_fd, &client_fds)){
                    // データがあるならパケット受信する
                    cnt = recv(client->server_fd, buf, sizeof(buf), 0);
                    if (cnt > 0) {
                        // パケット受信成功の場合
                        if(client->receive_proc != nullptr){
                            client->receive_proc(client, buf, cnt);
                            memset(buf, 0, sizeof(buf));
                        }
                    } else {
                        goto end;
                    }
                }
            }
        }
        end:
        log_output.InfoMessage("socket:%d  disconnected.", client->server_fd);
        close(client->server_fd);
        client->server_fd = -1;
        return nullptr;
    }

    /// @brief コンストラクタ
    Client() : receive_proc(nullptr) { this->receive_proc = nullptr; }

    /// @brief デストラクタ
    ~Client() {}
    
    /// @brief 初期化関数
    /// @param host 接続するIPアドレス
    /// @param port 接続するポート番号
    /// @return サーバーのファイルディスクリプタ
    /// @details ソケットを生成する
    int Init(const char* host, int port) {
        if((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            log_output.ErrorMessage("socket: %s", std::strerror(errno));
            return -1;
        }
        f_init = true;
        this->server_addr.sin_family = AF_INET;
        this->server_addr.sin_port = htons(port);
        this->server_addr.sin_addr.s_addr = inet_addr(host);
        log_output.InfoMessage("Init Client. %s, %d", host, port);
        return server_fd;
    }
    
    /// @brief コネクト関数
    /// @param f 受信ハンドラ
    /// @return エラーナンバー
    /// @details 引数に受信したときの処理を行う関数ポインタを入れる。接続に成功したらスレッドを生成する。ブロッキング関数
    int Connect(std::function<void(Client*, uint8_t*, int)> f = nullptr) {
        if(f_init) {
            if((connect(this->server_fd, (struct sockaddr *)&this->server_addr, sizeof(struct sockaddr_in))) < 0) {
                log_output.ErrorMessage("connect: %s", std::strerror(errno));
                return 1;
            }
            log_output.InfoMessage("Connected.");
            int thread_create = -1;
            if( (thread_create = pthread_create(&temp_thread, nullptr, Client::CommThread, this)) != 0) {
                log_output.ErrorMessage("pthread_create: %s", std::strerror(errno));
                return 2;
            }else{
                if (pthread_detach(temp_thread) != 0) {
                    log_output.ErrorMessage("pthread_detach: %s", std::strerror(errno));
                    return 3;
                }else{
                    this->receive_proc = f;
                    continueFlag = true;
                    log_output.InfoMessage("Client start.");
                }
            }
        }else{
            log_output.WarnMessage("Client has not been Init.");
            return -1;
        }
        return 0;
    }
    
    /// @brief 文字列送信関数
    /// @param format フォーマット指定子
    /// @param ... フォーマット指定子に対応する変数
    /// @return 送信した文字数
    /// @details 文字列を送信関数を通して送信する、printfのように使える
    int SendStr(const char* format, ...) {
        int ret;
        va_list ap;
        va_start( ap, format );
        vsnprintf(this->tx_buff, SEND_BUFF, format, ap);
        ret = this->Transmit(tx_buff, sizeof(tx_buff));
        memset(tx_buff, 0, sizeof(tx_buff));
        va_end( ap );
        return ret;
    }

    int SendData(std::vector<char> data){
        return this->Transmit(data.data(), data.size());
    }

    void Close() {
        continueFlag = false;
        pthread_join(temp_thread, nullptr);
        temp_thread = -1;
    }

    bool IsConnected() {
        return continueFlag;
    }
};

}