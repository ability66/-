#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QPushButton>
#include <QDesktopWidget>
#include <QParallelAnimationGroup>
#include "playerfirst.h"
#include "robotfirst.h"
#include "playerplayer.h"
#include <QSound>
class mainmenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainmenu(QWidget *parent = nullptr);
    ~mainmenu();

protected:
    void paintEvent(QPaintEvent* event);

private:
    void setBackground();
    void openOrCloseFunction();

private:
    QPushButton *btnPlayerFirst,*btnRobotFirst,*btnPlayerPlayer;

    //功能模块
    PlayerFirst m_playerfirst ;
    RobotFirst m_robotfirst ;
    PlayerPlayer m_playerplayer ;
    QSound *sound;
    void ChangeWindow_to_playerfirst();
    void ChangeWindow_to_robotfirst();
    void ChangeWindow_to_playerplayer();
    void appear_playerfirst();
    void appear_robotfirst();
    void appear_playerplayer();
    void run();
};

#endif // MAINMENU_H
