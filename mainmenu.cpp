#include "mainmenu.h"
#include <QPainter>
#include <QPixmap>
#include <QApplication>
#include <QDebug>
#include <QWidget>
#include <QMouseEvent>
#include <QSound>

mainmenu::mainmenu(QWidget *parent) :
    QMainWindow(parent)
{

    setWindowTitle("五子棋-主菜单");

    setBackground();
    openOrCloseFunction();
}
mainmenu::~mainmenu() {

}
void mainmenu::setBackground()

{
    const int w = 1400,  h = 740;
    const int x = (QApplication::desktop()->width() - w)/2;
    const int y = (QApplication::desktop()->height() - h)/2;
    this->setGeometry(x, y, w, h);
    //不允许最大化最小化
    this->setMaximumSize(QSize(w, h));
    this->setMinimumSize(QSize(w, h));

    //设置按钮位置大小样式
    btnPlayerFirst= new QPushButton(this);
    btnPlayerFirst->setGeometry(900,400,300,90);
    btnPlayerFirst->setStyleSheet("QPushButton{border-image:url(:/222/images/PlayerFirst.png)}");

    btnRobotFirst = new QPushButton(this);
    btnRobotFirst->setGeometry(950,500,300,90);
    btnRobotFirst->setStyleSheet("QPushButton{border-image:url(:/222/images/robotfirst.png)}");

    btnPlayerPlayer = new QPushButton(this);
    btnPlayerPlayer->setGeometry(1000,600,300,80);
    btnPlayerPlayer->setStyleSheet("QPushButton{border-image:url(:/222/images/pvp1.png)}");

}

void mainmenu::openOrCloseFunction()
{
    connect(btnPlayerFirst,&QPushButton::released,this,&mainmenu::ChangeWindow_to_playerfirst);
    connect(btnRobotFirst,&QPushButton::released,this,&mainmenu::ChangeWindow_to_robotfirst);
    connect(btnPlayerPlayer,&QPushButton::released,this,&mainmenu::ChangeWindow_to_playerplayer);

    connect(&m_playerfirst,&PlayerFirst::closed,this,&mainmenu::appear_playerfirst);
    connect(&m_robotfirst,&RobotFirst::closed,this,&mainmenu::appear_robotfirst);
    connect(&m_playerplayer,&PlayerPlayer::closed,this,&mainmenu::appear_playerplayer);
}
void mainmenu::appear_playerfirst() {
    m_playerfirst.hide();
    m_playerfirst.initGame();
    this->show();
}
void mainmenu::appear_robotfirst() {
    m_robotfirst.hide();
    m_robotfirst.initGame();
    this->show();
}
void mainmenu::appear_playerplayer() {
    m_playerplayer.hide();
    m_playerplayer.initGame();
    this->show();
}
void mainmenu::ChangeWindow_to_playerfirst() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    this->hide();
    m_playerfirst.show();
}
void mainmenu::ChangeWindow_to_robotfirst() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    this->hide();
    m_robotfirst.show();
}
void mainmenu::ChangeWindow_to_playerplayer() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    this->hide();
    m_playerplayer.show();
}
void mainmenu::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    painter.drawPixmap(QRect(0,0,1400,740),QPixmap(":/222/images/bg2.png"));
}
