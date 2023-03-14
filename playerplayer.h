#ifndef PLAYERPLAYER_H
#define PLAYERPLAYER_H
#include <QMainWindow>
#include <bits/stdc++.h>
#include <QPushButton>
#include <QSound>
#define BLACKFLAG 1
#define WHITEFLAG 2
#define EMPTYFLAG 0



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class PlayerPlayer : public QMainWindow
{
    Q_OBJECT

public:
    PlayerPlayer(QWidget *parent = nullptr);
    ~PlayerPlayer();

private:
    Ui::MainWindow *ui;
    const int INF = 0x3f3f3f3f;
    const long long WIN5=10000000;
    const long long LIVE4=100000;
    const long long DIE4=0;
    const long long DIE3=0;
    const long long DIE2=0;
    const long long ASIDE4=11000;
    const long long LIVE3=10000;
    const long long LIVE2=1000;
    const long long WINWIN=100000000;
    const long long NOTHREAT=0;
    const int MARGIN=30;//棋盘边缘空隙
    const int blockSize=40;//网格大小
    const int boardSize=14;//棋盘大小
    const int chessRadis=15;//棋子半径
    const int posOffset=blockSize*0.4;//鼠标点击模糊距离
    const int markSize=6;       //落子标记边长
    int clickPosRow,clickPosCol;
    bool selectPos=false;
    int player=1;//1轮到黑子 2轮到白子
    std::vector<std::vector<int>>chess_board;//0无子 1黑子 2白子
        std::stack<std::pair<int,int>>step;
    int moveX, moveY, lastx, lasty;
public:
    void initGame();
private:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    int judge_score(std::vector<int>chess);
    int main_diagonal(int x, int y, int type);
    int counter_diagonal(int x, int y, int type);
    int row(int x, int y, int type);
    int column(int x, int y, int type);
    bool judge_winner(int x, int y, int type);
    int judge_pre_forbiden(int x, int y, int type);
    bool judge_out_of_board(int x, int y);

    void sendSlot();
    void defeat();
    void try_draw();
    void try_regret();
    void defeat1();
    void try_draw1();
    void try_regret1();
    void show_rule();
    QPushButton * bt,* rule,* stopgame,* draw,* regret;
    QPushButton * stopgame1,* draw1,* regret1;
    QSound *sound;
signals:
    void closed();
};
#endif // PLAYERPLAYER_H
