#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QStringListModel>
#include <QListView>

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
    void searchButton_clicked();
    void listItem_clicked(QModelIndex index);

private:
    Ui::Player *ui;
    QStringListModel *model;
};

#endif // PLAYER_H
