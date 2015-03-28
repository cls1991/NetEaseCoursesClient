#include <QProcess>
#include <QDebug>
#include <QList>

#include "player.h"
#include "ui_player.h"
#include "videoItem.h"

Player::Player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);
    model = new QStringListModel(this);

    this->ui->listView->setModel(model);

    // 槽函数
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(playButton_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(searchButton_clicked()));
    connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(listItem_clicked(QModelIndex)));
}

Player::~Player()
{
    delete ui;
}

void Player::playButton_clicked() {
    QProcess *process = new QProcess;
//    QString fileName = "/home/tao/facebook.rmvb";
    QString fileName = "http://mov.bn.netease.com/open-movie/nos/mp4/2015/03/02/SAIPMBN3I_shd.mp4";
    const QString mplayer("/usr/bin/mplayer");
    QStringList args;
    args << "-slave";
    args << "-quiet";
    args << "-zoom";
    args << "-wid" << QString::number(this->ui->frame->winId());
    args << fileName;
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



