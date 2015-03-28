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
    void playButton_clicked();
    void pauseButton_clicked();
    void searchButton_clicked();
    void listItem_clicked(QModelIndex index);
    void slideBar_moved(int);
    void get_time_slots();
    void back_message_slots();


    QTime get_time_from_seconds(int second);

private:
    Ui::Player *ui;
    QStringListModel *model;

    QProcess *process;
    QTimer *timer;
    int curTime;
    int totalTime;
};

#endif // PLAYER_H
