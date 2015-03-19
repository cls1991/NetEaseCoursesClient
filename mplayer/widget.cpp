#include "widget.h"
#include "ui_widget.h"
#include <QProcess>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
//    QProcess *process = new QProcess();
//    process->setProcessChannelMode(QProcess::MergedChannels);
////    QStringList args;
////    args << " -slave";
////    args << " -quiet";
////    args << " -wid " << QString::number(this->winId());
////    args << " -zoom";
////    args << " -vo";
////    args << " x11";
////    args << " ./test.mp3";

////    QString s1 = "mplayer -slave -quiet -zoom -wid " + QString::number(this->winId());
//    QString s1 = "/usr/local/bin/mplayer";
//    s1.append(" ./test.mp3");
//    qDebug(qPrintable(s1));
////    int i;
////    for (i=0; i< args.length(); i++) {
////        QString str = args[i];
////            qDebug(qPrintable(str));
////    }
//    process->start("mplayer");
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButton_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::pushButton_clicked() {
    if (ui->label->text() == "kk")
        ui->label->setText("hello");
    else ui->label->setText("kk");

    QProcess *process = new QProcess;
    process->start("open \"/Applications/Music.163.app\"");
    QString result = process->readAll();
    qDebug(qPrintable(result.append("zzz")));
}
