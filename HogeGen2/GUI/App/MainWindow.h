#pragma once

#include <SubWindow.h>

#include <ModuleManagerGUI.h>

#include <QMainWindow>
#include <QCoreApplication>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QCommandLineParser>
#include <QTextStream>

#include <iostream>

using namespace HogeGen2;

class MainWindow : public QMainWindow {
    int port;
    int mode;   // 0: OFFLINE, 1:ONLINE
    QScrollArea *CreateBaseScrollArea() {
        // ScrollArea で表示する Widget 作成
        // レイアウト作成
        auto layout_vbox = new QVBoxLayout();
        layout_vbox->setObjectName("Contents");
        layout_vbox->setSpacing(0);
        layout_vbox->setContentsMargins(0,0,0,0);
        // Widget にセット
        auto widget = new QWidget();
        widget->setLayout(layout_vbox);
        widget->setObjectName("Contents Widget");

        // ScrollArea を作成
        QScrollArea *scrollarea = new QScrollArea();
        scrollarea->setWidget(widget);
        scrollarea->setContentsMargins(0,0,0,0);

        return scrollarea;
    }

    QGroupBox *CreateGroupBox(QString str, std::vector<IGroupBox*> modules) {
        // ScrollArea で表示する Widget 作成
        // レイアウト作成
        auto layout_vbox = new QVBoxLayout();
        layout_vbox->setSpacing(0);
        layout_vbox->setContentsMargins(0,0,0,0);
        // レイアウトにグループボックスを追加する
        for (auto module : modules) {
            layout_vbox->addWidget(module->GetGroupBox());
        }
        // スクロール用ウィジェットにレイアウトをセット
        auto scroll_widget = new QWidget();
        scroll_widget->setLayout(layout_vbox);

        // スクロールエリアを作成
        QScrollArea *scrollarea = new QScrollArea();
        scrollarea->setWidget(scroll_widget);
        scrollarea->setContentsMargins(0,0,0,0);

        // グループボックス生成
        auto group = new QGroupBox(str);
        // グループボックス用レイアウト生成
        auto group_layout = new QVBoxLayout(group);
        group_layout->setContentsMargins(0,0,0,0);
        
        // スクロールエリアをグループボックスにセット
        group->layout()->addWidget(scrollarea);

        return group;
    }

    template<typename T> QGroupBox *CreateModuleGroupBox(QString str, std::vector<T*> modules) {
        std::vector<IGroupBox*> igroup_boxs(std::begin(modules), std::end(modules));
        return CreateGroupBox(str, igroup_boxs);
    }

    void SetTimer() {
        QTimer Timer;
        Timer.setInterval(20);
        
        QObject::connect(&Timer, &QTimer::timeout,
            [&]()
            {
                //ValueWithLabel<float>::UpdateAll();
            }
        );
        Timer.start();
    }
public:
    MainWindow():
        QMainWindow()
    {
        auto arguments = QCoreApplication::arguments();
        CommandLineAnalyze(arguments.count(), arguments);
        
        // セントラルとなるウィジェットを生成
        QWidget *central = new QWidget(this);
        // ウィジェットにセットする VBOX レイアウトを生成
        auto main_layout = new QVBoxLayout(central);
        // セントラルウィジェットに設定
        setCentralWidget(central);

        // 一時的
        Setup();

        // タイトル設定
        auto title = QString("HogeHoge");
        title += (mode ? QString(" (ONLINE mode)") : QString(" (OFFLINE mode)"));
        setWindowTitle(title);

        // サイズ変更
        resize(700,500);

        // 表示する
        show();
    }

    void Setup() {
        if (mode != 0) return;

        ModuleManagerGUI::SetModule<EncoderModuleGUI>(3);
        auto encoder_module_group = CreateModuleGroupBox("Encoder Module", ModuleManagerGUI::encoderModules);
        centralWidget()->layout()->addWidget(encoder_module_group);

        ModuleManagerGUI::SetModule<SensorModuleGUI>(3);
        auto sensor_module_group = CreateModuleGroupBox("Sensor Module", ModuleManagerGUI::sensorModules);
        centralWidget()->layout()->addWidget(sensor_module_group);

        ModuleManagerGUI::SetModule<MotorModuleGUI>(3);
        auto motor_module_group = CreateModuleGroupBox("Motor Module", ModuleManagerGUI::motorModules);
        centralWidget()->layout()->addWidget(motor_module_group);
        
        ModuleManagerGUI::SetModule<SolenoidModuleGUI>(3);
        auto solenoid_module_group = CreateModuleGroupBox("Solenoid Module", ModuleManagerGUI::solenoidModules);
        centralWidget()->layout()->addWidget(solenoid_module_group);

        QTextStream(stdout) << "Setup";

    }

    bool CommandLineAnalyze(int argc, QStringList argv) {
        // 参考: https://kengosawa2.hatenablog.com/entry/2014/12/16/135101
        int argerr = 0;         //コマンドエラーとして終了する際のリターン値

        //コマンドオプション
        QString MODE_STR       = "mode";
        QString PORT_STR       = "port";

        QCommandLineParser parser;

        //--help,-hをQCommandLineParserで自動で処理するように依頼
        parser.addHelpOption();

        //オプション以外の純粋な引数についての指定と、解説欄の設定
        //parser.addPositionalArgument("path1 path2 ...","Source path to copy.");

        //--modeオプションの情報設定
        QCommandLineOption modeOpt(QStringList() << MODE_STR,           //オプション文字列
                                "Select launch mode",                   //help表示の際のオプション説明
                                "0|1",                                  //オプション指定例
                                "0");                                   //デフォルトの指定

        //--portオプションの情報設定
        QCommandLineOption portOpt(QStringList() << PORT_STR,           //オプション文字列
                                "Port number of server to connect to.", //help表示の際のオプション説明
                                "20000",                                //オプション指定例
                                "20000");                               //デフォルトの指定


        //設定したオプション情報をparserに追加
        parser.addOption(modeOpt);
        parser.addOption(portOpt);

        //コマンドラインの事前解析
        //実際のパース処理を行うprocess()では情報設定済以外のオプションが
        //存在すると内部でexit()をコールしてしまうため、事前にparse()してチェックしています。

        //parseがfalse=不明なオプションが設定されている
        if(!parser.parse(argv)){
            //知らないオプションが指定されていたので、対象のエラーオプションを取り出して表示。
            QString error_str("illegal option --");
            for(int i=0;i<parser.unknownOptionNames().count();i++){
                error_str.append("\"" + parser.unknownOptionNames().at(i) + "\"");
            }
            error_str.append("\n");
            QTextStream(stderr) << error_str;
            argerr = -1;
            goto end;
        }
        //コマンドライン解析の実行
        parser.process(argv);

        bool result;
        {
            // --mode
            QString modeopt = parser.value(modeOpt);
            if (parser.isSet(modeOpt)) {
                mode = modeopt.toInt(&result);
                if (!result || mode > 1) mode = 0;  // default
            }
            
            // --port
            QString portopt = parser.value(portOpt);
            if (parser.isSet(portOpt)) {
                port = portopt.toUInt(&result);
                if (!result || port > 65535) port = 20000;  // default
            }
        }

    end:
        //ここまでのコマンド解析でエラーが一つでもあれば終了
        if(argerr == -1){
            //helpを表示してexit()する
            parser.showHelp(argerr);
        }
        return(true);
    }
};