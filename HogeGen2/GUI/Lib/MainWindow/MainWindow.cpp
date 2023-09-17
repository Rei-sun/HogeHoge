#include "MainWindow.h"

using namespace HogeGen2;

QGroupBox *MainWindow::CreateModuleGroupBox(QString str) {
    // ScrollArea で表示する Widget 作成
    // レイアウト作成
    auto layout_vbox = new QVBoxLayout();
    layout_vbox->setSpacing(0);
    layout_vbox->setContentsMargins(0,0,0,0);
    layout_vbox->setAlignment(Qt::AlignTop);

    // スクロール用ウィジェットにレイアウトをセット
    auto scroll_widget = new QWidget();
    scroll_widget->setLayout(layout_vbox);

    // スクロールエリアを作成
    QScrollArea *scrollarea = new QScrollArea();
    scrollarea->setWidget(scroll_widget);
    scrollarea->setContentsMargins(0,0,0,0);
    scrollarea->setObjectName(str);
    scrollarea->setWidgetResizable(true);

    // グループボックス生成
    auto group = new QGroupBox(str);
    group->setObjectName(str);
    // グループボックス用レイアウト生成
    auto group_layout = new QVBoxLayout(group);
    group_layout->setContentsMargins(0,0,0,0);
    
    // スクロールエリアをグループボックスにセット
    group->layout()->addWidget(scrollarea);

    return group;
}

void MainWindow::AddModuleWidget(QString str, std::vector<IGroupBox*> modules) {
    QScrollArea *scrollarea = centralWidget()->findChild<QScrollArea*>(str);
    auto scroll_widget = scrollarea->takeWidget();
    QVBoxLayout *layout_vbox = dynamic_cast<QVBoxLayout*>(scroll_widget->layout());
    // レイアウトにグループボックスを追加する
    for (auto module : modules) {
        layout_vbox->addWidget(module->GetGroupBox());
    }
    scrollarea->setWidget(scroll_widget);
}

template<typename T> void MainWindow::SetupModuleWidget(QString str, std::vector<T*> modules) {
    std::vector<IGroupBox*> igroup_boxs(std::begin(modules), std::end(modules));
    AddModuleWidget(str, igroup_boxs);
}

void MainWindow::UnsetModuleWidget(QString str) {
    QScrollArea *scrollarea = centralWidget()->findChild<QScrollArea*>(str);
    auto scroll_widget = scrollarea->takeWidget();
    QVBoxLayout *layout_vbox = dynamic_cast<QVBoxLayout*>(scroll_widget->layout());
    // レイアウトのグループボックスを削除する
    QLayoutItem *child;
    while ((child = layout_vbox->takeAt(0)) != nullptr) {
        delete child->widget(); // delete the widget
        delete child;   // delete the layout item
    }
    scrollarea->setWidget(scroll_widget);
}

void MainWindow::SetupModuleGroupBoxWidget() {
    ip_communication.SendBar({
        ModuleID::EncoderModule, 
        ModuleID::SensorModule,
        ModuleID::MotorModule,
        ModuleID::SolenoidModule
    });

    ModuleManagerGUI::SetModule<EncoderModuleGUI>(module_composition.GetCount(ModuleID::EncoderModule), &ip_communication);
    SetupModuleWidget<EncoderModuleGUI>("EncoderModule", ModuleManagerGUI::encoderModules);

    ModuleManagerGUI::SetModule<SensorModuleGUI>(module_composition.GetCount(ModuleID::SensorModule), &ip_communication);
    SetupModuleWidget<SensorModuleGUI>("SensorModule", ModuleManagerGUI::sensorModules);

    ModuleManagerGUI::SetModule<MotorModuleGUI>(module_composition.GetCount(ModuleID::MotorModule), &ip_communication);
    SetupModuleWidget<MotorModuleGUI>("MotorModule", ModuleManagerGUI::motorModules);

    ModuleManagerGUI::SetModule<SolenoidModuleGUI>(module_composition.GetCount(ModuleID::SolenoidModule), &ip_communication);
    SetupModuleWidget<SolenoidModuleGUI>("SolenoidModule", ModuleManagerGUI::solenoidModules);
}

void MainWindow::UnsetModuleGroupBoxWidget() {
    ModuleManagerGUI::UnsetModules();
    UnsetModuleWidget("EncoderModule");
    UnsetModuleWidget("SensorModule");
    UnsetModuleWidget("MotorModule");
    UnsetModuleWidget("SolenoidModule");
}

void MainWindow::SetTimer() {
    Timer.setInterval(20);
    
    QObject::connect(&Timer, &QTimer::timeout,
        [&]()
        {
            // TODO: 非同期処理
            std::future<void> fut1 = std::async(std::launch::async, TimerUpdate);
            
        }
    );
}

void MainWindow::TimerStart() {
    Timer.start();
}

void MainWindow::TimerStop() {
    Timer.stop();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    IsMainClose = true;
}

MainWindow::MainWindow():
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

    // 操作用グループボックス
    auto main_group_box = new QGroupBox();
    centralWidget()->layout()->addWidget(main_group_box);
    
    auto main_group_layout = new QHBoxLayout(main_group_box);
    main_group_layout->setContentsMargins(0,0,0,0);
    main_group_layout->setAlignment(Qt::AlignRight);

    connect_label  = new QLabel("Disconnected");
    main_group_layout->addWidget(connect_label);

    connect_button = new QPushButton("Connect");
    main_group_layout->addWidget(connect_button);
    connect_button->setFixedSize(80,30);
    QObject::connect(connect_button, &QPushButton::clicked, [&]() {
           ConnectButton_OnClicked();
    });

    // 各モジュールの情報を表示する領域を作成する 
    SetupGroupBox();

    // モードによって動作を変更する
    if(mode != 0) {
        ConnectButton_OnClicked();
        connect_button->setEnabled(false);
    }

    // モジュールを作成する。

    // タイトル設定
    auto title = QString("HogeHoge");
    title += (mode ? QString(" (ONLINE mode)") : QString(" (OFFLINE mode)"));
    setWindowTitle(title);

    // サイズ変更
    resize(700,500);

    // 表示する
    show();

    SetTimer();
}

void MainWindow::SetupGroupBox() {

    auto encoder_module_group = CreateModuleGroupBox("EncoderModule");
    centralWidget()->layout()->addWidget(encoder_module_group);

    auto sensor_module_group = CreateModuleGroupBox("SensorModule");
    centralWidget()->layout()->addWidget(sensor_module_group);

    auto motor_module_group = CreateModuleGroupBox("MotorModule");
    centralWidget()->layout()->addWidget(motor_module_group);
    
    auto solenoid_module_group = CreateModuleGroupBox("SolenoidModule");
    centralWidget()->layout()->addWidget(solenoid_module_group);

}

bool MainWindow::CommandLineAnalyze(int argc, QStringList argv) {
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

void MainWindow::TimerUpdate() {
    static int cnt = 0;
    cnt++;
    if(cnt > 50) {
        std::cout << "UpdateTimer\n";
        cnt = 0;
    }

    for (auto module : ModuleManagerGUI::encoderModules) {
        ip_communication.SendHoge(*module);
    }
    for (auto module : ModuleManagerGUI::sensorModules) {
        ip_communication.SendHoge(*module);
    }
    for (auto module : ModuleManagerGUI::motorModules) {
        ip_communication.SendHoge(*module);
    }
    for (auto module : ModuleManagerGUI::solenoidModules) {
        ip_communication.SendHoge(*module);
    }
}

void MainWindow::ConnectButton_OnClicked() {
    // 接続していた場合は切断
    // 接続していない場合は接続
    if (ip_communication.IsConnected()) {
        TimerStop();
        ip_communication.Stop();
    } else {
        ip_communication.Start("127.0.0.1", port);
    }
    
    // 接続の状態によってラベルとボタンのテキストを変更する
    if (ip_communication.IsConnected()) {

        connect_label->setText("Connected");
        connect_button->setText("Disconnect");
        SetupModuleGroupBoxWidget();
        TimerStart();
    
    } else {
    
        connect_label->setText("Disconnected");
        connect_button->setText("Connect");
        UnsetModuleGroupBoxWidget();
    
    }
}