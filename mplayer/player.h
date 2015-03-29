#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QStringListModel>
#include <QListView>
#include <QProcess>
#include <QTimer>
#include <QTime>

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = 0);
    ~Player();

private slots:
    void play_or_pause_clicked();
    void exit_play();
    void seek_slots(int value);
    void volum_slots(int value);
    void mute_slots();
    void searchButton_clicked();
    void listItem_clicked(QModelIndex index);
    void get_time_slots();
    void back_message_slots();
    QTime get_time_from_seconds(int second);

private:
    void start_play();

private:
    Ui::Player *ui;
    QStringListModel *model;

    QProcess *process;
    QTimer *timer;
    int curTime;
    int totalTime;
    QString filename;

    bool isPlay;
    bool isStop;
    bool isMute;

};

#endif // PLAYER_H
