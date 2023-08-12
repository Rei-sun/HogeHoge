# HogeHoge
GUIの描画にQt5を使用。

## ビルド
HogeGen1のMainをビルド/実行する場合

```
git clone https://github.com/Rei-sun/HogeHoge.git
cd HogeHoge/HogeGen1/Main/build
cmake ..
make
./Application/Main
```

HogeGen2のMainをビルド/実行する場合

```
git clone https://github.com/Rei-sun/HogeHoge.git
cd HogeHoge/HogeGen2/Main/build
cmake ..
make
./App/Main
```

## ディレクトリ構造
```
HogeHoge
  ├ HogeGen1/ ←はじめのやつ
  │   ├ ESP32_Serial_Test/ ←通信確認用
  │   ├ GUIApplication/ ←GUIアプリケーションのコード
  │   └ Main/ ←本体
  └ HogeGen2/ ←クラス構造を変更したやつ
      ├ Common/ ←共通クラス
      ├ ESP32_Serial_Test_Gen2/ ←通信確認用
      ├ GUI/ ←GUIアプリケーションのコード
      └ Main/ ←本体
```

## その他
以下のコードを使用
- nlohmann : [json](https://github.com/nlohmann/json/tree/develop)