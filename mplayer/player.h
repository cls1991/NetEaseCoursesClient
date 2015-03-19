#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = 0);
    ~Player();

public slots:
    void pushButton_clicked();

private:
    Ui::Player *ui;
};

#endif // PLAYER_H
