#include "player.h"
#include "ui_player.h"
#include <QProcess>

Player::Player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButton_clicked()));
}

Player::~Player()
{
    delete ui;
}

void Player::pushButton_clicked() {
    QProcess *process = new QProcess(this);
    QString fileName = "/home/tao/facebook.rmvb";
    const QString mplayer("/usr/bin/mplayer");
    QStringList args;
    args << "-slave";
    args << "-quiet";
    args << "-ac" << "mad";
    args << "-zoom";
    args << "-wid" << QString::number(this->winId());
    args << fileName;
    process->start(mplayer, args);
}
