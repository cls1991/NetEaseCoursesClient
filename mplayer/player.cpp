#include <QDebug>
#include <QList>
#include <jsoncpp/json/json.h>

#include "player.h"
#include "ui_player.h"
#include "videoItem.h"
#include "httpClient.h"


Player::Player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Player),
    curTime(0), totalTime(1),
    isPlay(false), isStop(true), isMute(false),
    filename("")
{
    ui->setupUi(this);

    ui->label->setText("00:00:00");
    ui->label_2->setText("00:00:00");

    model = new QStringListModel(this);

    ui->listView->setModel(model);

    process = new QProcess;
    process->setProcessChannelMode(QProcess::MergedChannels);

    timer = new QTimer;

    // 槽函数
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(play_or_pause_clicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(exit_play()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(volum_slots(int)));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(mute_slots()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(searchButton_clicked()));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(listItem_clicked(QModelIndex)));
    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(seek_slots(int)));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(back_message_slots()));
    connect(timer, SIGNAL(timeout()), this, SLOT(get_time_slots()));
    timer->start(1000);
}

Player::~Player()
{
    delete ui;
}

void Player::start_play() {
    const QString mplayer("/usr/bin/mplayer");
    QStringList args;
    args << "-slave";
    args << "-quiet";
    args << "-zoom";
    args << "-wid" << QString::number(this->ui->frame->winId());
    args << filename;

    volum_slots(30);
    process->start(mplayer, args);
    isStop = false;
    isPlay = true;
}

void Player::play_or_pause_clicked() {
    if (filename == "") return;
    if (isStop) {
        // 如果是退出状态, 开始播放
        start_play();
    }
    else {
        // 切换按钮状态
        if (isPlay) {
            timer->stop();
            process->write("pause\n");
            this->ui->pushButton->setText("pause");
            isPlay = false;
        }
        else {
            process->write("pause\n");
            this->ui->pushButton->setText("play");
            isPlay = true;
            timer->start(1000);
        }
    }
}

void Player::searchButton_clicked() {
    // 测试listview是否可用
    if (this->ui->plainTextEdit->toPlainText() != "") {
        // example: http://open.163.com/special/opencourse/algorithms.html
        std::string search = this->ui->plainTextEdit->toPlainText().toUtf8().constData();
        QStringList args;
        // 模拟httpclient获取后台数据
        std::string buffer;
        HttpClient().get("http://localhost:9001/showLinks?html_url=" + search, buffer);
        Json::Reader reader;
        Json::Value root;
        if (reader.parse(buffer, root)) {
            // 获取视频链接集合
            const Json::Value arrayObj = root["data"];
            for (unsigned int i=0; i< arrayObj.size(); i++) {
                std::string url = arrayObj[i].asString();
                // convert std::string to QString
                args << QString::fromUtf8(url.c_str());
            }
        }
        this->model->setStringList(args);
    }
}

void Player::listItem_clicked(QModelIndex index) {
    filename = index.data().toString();
    exit_play();
    process->write("quit\n");
    process = new QProcess(this);
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(back_message_slots()));
    start_play();
}

void Player::seek_slots(int value) {
    // seek <value> [type]
    // 这里采取的是百分比形式，拖拽进度条
    if (isStop) return;
    process->write(QString("seek %1 1\n").arg(qMin(value, 100) + 1).toUtf8());
}

void Player::get_time_slots() {
    process->write("get_time_pos\n");
    process->write("get_time_length\n");
}

void Player::back_message_slots() {
    while (process->canReadLine()) {
        QString message(process->readLine());
        QStringList messageList = message.split("=");
        if (messageList[0] == "ANS_TIME_POSITION") {
            curTime = messageList[1].toDouble();
            QTime time = get_time_from_seconds(curTime);
            this->ui->label->setText(time.toString("hh:mm:ss"));
            this->ui->horizontalSlider->setValue(100 * curTime / totalTime);
        }
        else if (messageList[0] == "ANS_LENGTH") {
            totalTime = messageList[1].toDouble();
            QTime time = get_time_from_seconds(totalTime);
            this->ui->label_2->setText(time.toString("hh:mm:ss"));
        }
    }
}

QTime Player::get_time_from_seconds(int second) {
    QTime time;
    int hour =0, min = 0, sec = 0;
    if (second < 60) {
        sec = second;
        min = hour = 0;
    }
    else if (second >= 60 && second < 3600) {
        sec = second % 60;
        min = second / 60;
        hour = 0;
    }
    else {
        sec = second % 60;
        min = (second / 60) % 60;
        hour = second / 3600;
    }
    time .setHMS(hour, min, sec);
    return time;
}

void Player::exit_play() {
    if (isStop) return;
    process->write("quit\n");
    this->ui->label->setText("00:00:00");
    this->ui->label_2->setText("00:00:00");
    this->ui->horizontalSlider->setValue(0);
    isStop = true;
}

void Player::volum_slots(int value) {
    process->write(QString("volume %1 1\n").arg(value).toUtf8());
}

void Player::mute_slots() {
    if (isMute) {
        process->write("mute 0\n");
        this->ui->pushButton_4->setText("normal");
        isMute = false;
    }
    else {
        process->write("mute 1\n");
        this->ui->pushButton_4->setText("mute");
        isMute = true;
    }
}


