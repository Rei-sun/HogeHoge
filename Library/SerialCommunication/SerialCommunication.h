#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <functional>
#include <thread>
#include <memory>
#include <sys/ioctl.h>
#include <mutex>

/**
 * @namespace HogeHoge
*/
namespace HogeHoge {
    /**
     * @brief シリアル通信を行うクラス
     * @details 接続中、インスタンスが破棄される時に接続を終了する
    */
    class SerialCommunication {
        // ターゲットデバイス
        std::string device_name;
        
        // 接続したファイルディスクリプタ
        int fd;

        // 接続時コールバック
        std::function<void(void)> on_connect;

        // 切断時コールバック
        std::function<void(void)> on_disconnect;

        // 受信コールバック
        std::function<void(void*, size_t)> on_receive;

        // 接続切れ時コールバック
        std::function<void(void)> on_disconnected;

        // 再接続時コールバック
        std::function<void(void)> on_reconnected;

        // 再接続時コールバック
        std::function<void(void)> on_timeout;

        // 通信用スレッド
        std::shared_ptr<std::thread> thread;

        // スレッド制御用フラグ
        bool thread_continue;

        // 接続フラグ
        bool is_connected;

        // 最終送信失敗カウント
        uint32_t transmit_failure_count;

        // タイムアウト [ms]
        uint32_t timeout;

        // タイムアウト [s]
        time_t timeout_sec;

        // タイムアウト [us]
        suseconds_t timeout_usec;

        // 再接続インターバル [s]
        uint32_t reconnect_interval;

        // 排他制御用
        std::mutex io_mutex;

    private:
        /**
         * @brief スレッド開始関数
        */
        void StartCommunicationThread();

        /**
         * @brief スレッド停止関数
        */
        void StopCommunicationThread();

        /**
         * @brief 通信用スレッド
        */
        void CommunicationProcess();

        /**
         * @brief 内部用の通信を開始する関数
         * @param device_name 通信するデバイス名
         * @param reconnect 再接続かどうか
         * @return 成功/失敗
        */
        bool OpenSerialPort(const char *device_name, bool reconnect);

        /**
         * @brief 内部用の通信を終了する関数
         * @param error エラーに呼び出しかどうか
        */
        void CloseSerialPort(bool error);

    public:
        /**
         * @brief コンストラクタ
        */
        SerialCommunication();

        /**
         * @brief デストラクタ
        */
        ~SerialCommunication();

        /**
         * @brief 通信を開始する関数
         * @param device_name 通信するデバイス名
         * @return 成功/失敗
        */
        bool Open(const char *device_name, bool thread_start);

        /**
         * @brief 通信を開始する関数
         * @param device_name 通信するデバイス名
         * @return 成功/失敗
        */
        bool Open(std::string device_name, bool thread_start);

        /**
         * @brief 通信を終了する関数
        */
        void Close();

        /**
         * @brief 送信関数
         * @param data 送信するデータ
         * @param size 送信するデータサイズ
        */
        void Transmit(uint8_t *data, size_t size);

        /**
         * @brief 送信関数
         * @param msg 送信するデータ
         * @param size 送信するデータサイズ
        */
        void Transmit(const char* msg, size_t size);
        
        /**
         * @brief 送信関数
         * @param data 送信するデータ
         * @param size 送信するデータサイズ
        */
        void Transmit(std::string msg);

        /**
         * @brief OnConnect用コールバック登録関数
         * @param callback コールバック
        */
        void RegisterCallbackOnConnect(std::function<void(void)> callback);

        /**
         * @brief OnDisconnect用コールバック登録関数
         * @param callback コールバック
        */
        void RegisterCallbackOnDisconnect(std::function<void(void)> callback);

        /**
         * @brief OnReceive用コールバック登録関数
         * @param callback コールバック
        */
        void RegisterCallbackOnReceive(std::function<void(void*, size_t)> callback);

        /**
         * @brief OnDisconnected用コールバック登録関数
         * @param callback コールバック
        */
        void RegisterCallbackOnDisconnected(std::function<void(void)> callback);

        /**
         * @brief OnReconnected用コールバック登録関数
         * @param callback コールバック
        */
        void RegisterCallbackOnReconnected(std::function<void(void)> callback);

        /**
         * @brief OnTimeout用コールバック登録関数
         * @param callback コールバック
        */
        void RegisterCallbackOnTimeout(std::function<void(void)> callback);
    };
}