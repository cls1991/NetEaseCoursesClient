#include <QDebug>
#include <QList>

#include "player.h"
#include "ui_player.h"
#include "videoItem.h"

Player::Player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Player),
    curTime(0), totalTime(1)
{
    ui->setupUi(this);
    model = new QStringListModel(this);

    this->ui->listView->setModel(model);

    process = new QProcess;

    timer = new QTimer;

    // 槽函数
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(playButton_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(searchButton_clicked()));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(listItem_clicked(QModelIndex)));
    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(slideBar_moved(int)));
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(back_message_slots()));
    connect(timer, SIGNAL(timeout()), this, SLOT(get_time_slots()));

    timer->start(1000);
}

Player::~Player()
{
    delete ui;
}

void Player::playButton_clicked() {
    QString fileName = "http://mov.bn.netease.com/open-movie/nos/mp4/2015/03/02/SAIPMBN3I_shd.mp4";
    const QString mplayer("/usr/bin/mplayer");
    QStringList args;
    args << "-slave";
    args << "-quiet";
    args << "-zoom";
    args << "-wid" << QString::number(this->ui->frame->winId());
    args << fileName;
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start(mplayer, args);
}

void Player::searchButton_clicked() {
    // 测试listview是否可用
    if (this->ui->plainTextEdit->toPlainText() != "") {
        QStringList args;
        args << this->ui->plainTextEdit->toPlainText();
        // 从python后台获取数据
        QList<VideoItem *> itemList;
        itemList.append(new VideoItem(1, "ios8_1", "http://mov.bn.netease.com/open-movie/nos/mp4/2015/03/02/SAIPMBN3I_shd.mp4"));
        qDebug() << itemList.at(0)->getVideoUrl();
        this->model->setStringList(args);
    }
}

void Player::listItem_clicked(QModelIndex index) {
    qDebug() << index.data().toString();
}

void Player::slideBar_moved(int) {
    int pos = this->ui->horizontalSlider->value();
    qDebug() << QString("%1").arg(pos);
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

void Player::pauseButton_clicked() {

}





