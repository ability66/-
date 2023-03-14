#include "playerplayer.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QBrush>
#include <QDebug>
#include <QWidget>
#include <cmath>
#include <QMessageBox>
#include <stdio.h>
#include <windows.h>
#include <fstream>
#include <QSound>
PlayerPlayer::PlayerPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //关联返回按钮点击事件与窗体关闭事件
    bt = new QPushButton(this);
    bt->setGeometry(1200, 0, 250, 50);
    bt->setStyleSheet("QPushButton{border-image:url(:/222/images/back_to_menu.png)}");
    connect(bt, &QPushButton::clicked, this, &PlayerPlayer::sendSlot);
    bt->show();

    //触发bt 然后本窗体的槽函数接受触发信息 然后槽函数函数体执行 发出信号

    rule = new QPushButton(this);
    rule->setGeometry(0, 0, 250, 50);
    rule->setStyleSheet("QPushButton{border-image:url(:/222/images/rule.png)}");
    connect(rule, &QPushButton::clicked, this, &PlayerPlayer::show_rule);

    stopgame = new QPushButton(this);
    stopgame->setGeometry(1090, 430, 250, 50);
    stopgame->setStyleSheet("QPushButton{border-image:url(:/222/images/defeat.png)}");
    connect(stopgame, &QPushButton::clicked, this, &PlayerPlayer::defeat);

    regret = new QPushButton(this);
    regret->setGeometry(1090, 500, 250, 50);
    regret->setStyleSheet("QPushButton{border-image:url(:/222/images/regret.png)}");
    connect(regret, &QPushButton::clicked, this, &PlayerPlayer::try_regret);

    draw = new QPushButton(this);
    draw->setGeometry(1090, 570, 250, 50);
    draw->setStyleSheet("QPushButton{border-image:url(:/222/images/draw.png)}");
    connect(draw, &QPushButton::clicked, this, &PlayerPlayer::try_draw);

    stopgame1 = new QPushButton(this);
    stopgame1->setGeometry(40, 430, 250, 50);
    stopgame1->setStyleSheet("QPushButton{border-image:url(:/222/images/defeat.png)}");
    connect(stopgame1, &QPushButton::clicked, this, &PlayerPlayer::defeat1);

    regret1 = new QPushButton(this);
    regret1->setGeometry(40, 500, 250, 50);
    regret1->setStyleSheet("QPushButton{border-image:url(:/222/images/regret.png)}");
    connect(regret1, &QPushButton::clicked, this, &PlayerPlayer::try_regret1);

    draw1 = new QPushButton(this);
    draw1->setGeometry(40, 570, 250, 50);
    draw1->setStyleSheet("QPushButton{border-image:url(:/222/images/draw.png)}");
    connect(draw1, &QPushButton::clicked, this, &PlayerPlayer::try_draw1);
    //鼠标监听
    setMouseTracking(true);
    //设置窗口大小
    this->setFixedSize(1400, 740); //设置窗口大小
    setWindowTitle("五子棋-人人对战");
    initGame();
}

void PlayerPlayer::sendSlot() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    emit closed();
}
PlayerPlayer::~PlayerPlayer() {
    delete ui;
}
void PlayerPlayer::defeat() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    QSound *sound = new QSound(":/222/sound/win.wav");
    sound ->play();//播放函数
    QMessageBox::StandardButton btnValue = QMessageBox::information(this, "游戏结束", "黑棋胜利! ");
    if(btnValue==QMessageBox::Ok) initGame();
}
void PlayerPlayer::try_draw() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Question, tr("请求"), tr("玩家2请求求和"), QMessageBox::Yes | QMessageBox::No);
    int res = msgBox->exec();
    if(res==QMessageBox::Yes) {
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "成功", "平局 ");
        if(btnValue==QMessageBox::Ok) initGame();
    }
    else QMessageBox::information(this, "失败", "玩家1拒绝与你求和 ");
}
void PlayerPlayer::try_regret() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    if(step.size()>=2) {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Question, tr("请求"), tr("玩家2请求悔棋"), QMessageBox::Yes | QMessageBox::No);
        int res = msgBox->exec();
        if(res==QMessageBox::Yes) {
            chess_board[step.top().first][step.top().second]=EMPTYFLAG;
            step.pop();
            chess_board[step.top().first][step.top().second]=EMPTYFLAG;
            step.pop();
            repaint();
        }
        else {
            QMessageBox::information(this, "失败", "玩家1拒绝你的请求");
        }
    }
    else {
        QMessageBox::information(this, "失败", "悔棋失败 ");
    }
}

void PlayerPlayer::defeat1() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    QSound *sound = new QSound(":/222/sound/win.wav");
    sound ->play();//播放函数
    QMessageBox::StandardButton btnValue = QMessageBox::information(this, "游戏结束", "白棋胜利! ");
    if(QMessageBox::Ok==btnValue) initGame();
}
void PlayerPlayer::try_draw1() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Question, tr("请求"), tr("玩家1请求求和"), QMessageBox::Yes | QMessageBox::No);
    int res = msgBox->exec();
    if(res==QMessageBox::Yes) {
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "成功", "平局 ");
        if(btnValue==QMessageBox::Ok) initGame();
    }
    else QMessageBox::information(this, "失败", "玩家2拒绝与你求和 ");
}
void PlayerPlayer::try_regret1() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    if(step.size()>=2) {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Question, tr("请求"), tr("玩家1请求悔棋"), QMessageBox::Yes | QMessageBox::No);
        int res = msgBox->exec();
        if(res==QMessageBox::Yes) {
            chess_board[step.top().first][step.top().second]=EMPTYFLAG;
            step.pop();
            chess_board[step.top().first][step.top().second]=EMPTYFLAG;
            step.pop();
            repaint();
        }
        else {
            QMessageBox::information(this, "失败", "玩家2拒绝你的请求");
        }
    }
    else {
        QMessageBox::information(this, "失败", "悔棋失败 ");
    }
}
void PlayerPlayer::show_rule() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    QMessageBox::information(this, "规则", "规则要求：每人一次下一子，黑棋先下（为先手），白棋后下，黑棋第一步必须走天元（图中黑点），黑白双方轮流交替下子，直到黑白中任意一方获胜。\n获胜的判定：只要黑白方中任意一方的棋子有五子连在一起（即五子连珠），可以为横连、纵连、斜连，则该方获胜，游戏结束。\n延伸知识：黑棋虽先行，但有禁手：黑方不能在一步之内形成两个“活三”“活四”或一步之内形成“长连”（指一步形成超过五子连珠）。白方自由，无禁手。 ");
}
//绘制事件
void PlayerPlayer::paintEvent(QPaintEvent *) {
    //绘制棋盘边界边框，棋盘内颜色
    QPainter painter(this);
    painter.drawPixmap(QRect(0, 0, 1400, 740), QPixmap(":/222/images/bg_pvp.png"));

    painter.setRenderHint(QPainter::Antialiasing, true); //抗锯齿
    centralWidget()->setMouseTracking(true);//接受鼠标的移动
    setMouseTracking(true);

    QPen pen = painter.pen(); //画笔
    pen.setColor(QColor("#8D5822")); //棋盘边界边框
    pen.setWidth(7);
    painter.setPen(pen);

    int abs_x=370,abs_y=50;
    QBrush brush; //画刷,棋盘内颜色
    brush.setColor(QColor("#EEC085"));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(20+abs_x, 40+abs_y, 600, 600);

    //绘制棋盘
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen); //黑线

    for (int i = 0; i < 15; ++i) {
        painter.drawLine(40 + i * 40+abs_x, 60+abs_y, 40 + i * 40+abs_x, 620+abs_y); //纵线，QT的坐标60最上，620最下
        painter.drawLine(40+abs_x, 60 + i * 40+abs_y, 600+abs_x, 60 + i * 40+abs_y); //横线，QT的坐标40最左，600最右
    }

    //绘制棋盘中间的五个黑点
    brush.setColor(Qt::black); //五个小黑点
    painter.setBrush(brush);
    painter.drawRect(155+abs_x, 175+abs_y, 10, 10);
    painter.drawRect(475+abs_x, 175+abs_y, 10, 10);
    painter.drawRect(155+abs_x, 495+abs_y, 10, 10);
    painter.drawRect(475+abs_x, 495+abs_y, 10, 10);
    painter.drawRect(315+abs_x, 335+abs_y, 10, 10);

    //循环遍历，画棋子
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if (chess_board[i][j] == BLACKFLAG) { //黑子,电脑或玩家1
                painter.drawPixmap(QRect((j + 1) * 40+342,(i + 1) * 40 + 20+20,60,60),QPixmap(":/222/images/black.png"));
            } else if (chess_board[i][j] == WHITEFLAG) { //白子，玩家2
                painter.drawPixmap(QRect((j + 1) * 40+342,(i + 1) * 40 + 20+20,60,60),QPixmap(":/222/images/white.png"));
            }
        }
    }

    pen.setColor(Qt::red);
    pen.setWidth(1);
    painter.setPen(pen);
    abs_x=50,abs_y=370;
    //鼠标移动后，当前红方框标识
    if ((moveX * 40 + 110) >= 110 && (moveX * 40 + 110) <= 670 && (moveY * 40 + 410) >= 410 && (moveY * 40 + 410) <= 970) {
        painter.drawLine((moveY + 1) * 40 - 20+abs_y, (moveX + 1) * 40+abs_x, (moveY + 1) * 40 - 10+abs_y, (moveX + 1) * 40+abs_x);
        painter.drawLine((moveY + 1) * 40 + 20+abs_y, (moveX + 1) * 40+abs_x, (moveY + 1) * 40 + 10+abs_y, (moveX + 1) * 40+abs_x);
        painter.drawLine((moveY + 1) * 40 - 20+abs_y, (moveX + 1) * 40 + 40+abs_x, (moveY + 1) * 40 - 10+abs_y, (moveX + 1) * 40 + 40+abs_x);
        painter.drawLine((moveY + 1) * 40 + 20+abs_y, (moveX + 1) * 40 + 40+abs_x, (moveY + 1) * 40 + 10+abs_y, (moveX + 1) * 40 + 40+abs_x);
        painter.drawLine((moveY + 1) * 40 - 20+abs_y, (moveX + 1) * 40+abs_x, (moveY + 1) * 40 - 20+abs_y, (moveX + 1) * 40 + 10+abs_x);
        painter.drawLine((moveY + 1) * 40 + 20+abs_y, (moveX + 1) * 40+abs_x, (moveY + 1) * 40 + 20+abs_y, (moveX + 1) * 40 + 10+abs_x);
        painter.drawLine((moveY + 1) * 40 - 20+abs_y, (moveX + 1) * 40 + 40+abs_x, (moveY + 1) * 40 - 20+abs_y, (moveX + 1) * 40 + 30+abs_x);
        painter.drawLine((moveY + 1) * 40 + 20+abs_y, (moveX + 1) * 40 + 40+abs_x, (moveY + 1) * 40 + 20+abs_y, (moveX + 1) * 40 + 30+abs_x);
    }
    //lastx,lasty记录最后一个落子的位置
    //画最后一个落子位置
}

//鼠标移动事件
void PlayerPlayer::mouseMoveEvent(QMouseEvent *event) {
    selectPos = 0;
    moveX = (event->y() - 40-50) / 40;
    moveY = (event->x() - 20-370) / 40;
    if(moveX>=0&&moveX<=14&&moveY>=0&&moveY<=14)selectPos = 1;
    repaint(); //更新后就重画
}

//鼠标点击事件
void PlayerPlayer::mouseReleaseEvent(QMouseEvent *event) {

    Q_UNUSED(event);
    clickPosRow = moveX;
    clickPosCol = moveY;
    if (selectPos == false || chess_board[clickPosRow][clickPosCol] != 0) {
        return ;
    } else {
        //在落子前，把落子标记再设置为false
        selectPos = false;
    }
    //下棋
    chess_board[clickPosRow][clickPosCol] = player;
    QSound *cl = new QSound(":/222/sound/chess_down.wav");
    cl ->play();//播放函数
    step.push({clickPosRow, clickPosCol});
    repaint();
    if (clickPosRow >= 0 && clickPosRow <= boardSize && clickPosCol >= 0 && clickPosCol <= boardSize && (chess_board[clickPosRow][clickPosCol] == BLACKFLAG || chess_board[clickPosRow][clickPosCol] == WHITEFLAG)) {
        if (judge_pre_forbiden(clickPosRow, clickPosCol, player) && player == BLACKFLAG) {
            QString str;
            QSound *sound = new QSound(":/222/sound/lose.wav");
            sound ->play();//播放函数
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "游戏结束", "禁手! ");
            if (btnValue == QMessageBox::Ok) {
                initGame();
            }
        } else if (judge_winner(clickPosRow, clickPosCol, chess_board[clickPosRow][clickPosCol])) {
            QString str;
            if (player == BLACKFLAG)str = "黑棋";
            else str = "白棋" ;
            QSound *sound = new QSound(":/222/sound/win.wav");
            sound ->play();//播放函数
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "游戏结束", str + "胜利! ");
            if (btnValue == QMessageBox::Ok) {
                initGame();
            }
        } else {
            if (player == BLACKFLAG)player = WHITEFLAG;
            else player = BLACKFLAG;
        }
    }
}


void PlayerPlayer::initGame() {
    player = 1;
    chess_board.resize(boardSize + 1);
    for (int i = 0; i < boardSize + 1; i++)chess_board[i].resize(boardSize + 1);
    for (int i = 0; i <= boardSize; i++) {
        for (int j = 0; j <= boardSize; j++) {
            chess_board[i][j] = EMPTYFLAG;
        }
    }
    while(step.size()) step.pop();
}


//落子合法性判断
bool PlayerPlayer::judge_out_of_board(int x, int y) {
    if (x >= 15 || x < 0 || y >= 15 || y < 0) return 1;
    return 0;
}//是否越界

int PlayerPlayer::judge_pre_forbiden(int x, int y, int type) {
    if (type == BLACKFLAG) {
        bool pre_val = chess_board[x][y];
        chess_board[x][y] = BLACKFLAG;
        int LIVE3_CNT = 0;
        if (main_diagonal(x, y, type) == LIVE3) LIVE3_CNT++;
        if (counter_diagonal(x, y, type) == LIVE3) LIVE3_CNT++;
        if (column(x, y, type) == LIVE3) LIVE3_CNT++;
        if (row(x, y, type) == LIVE3) LIVE3_CNT++;
        if (LIVE3_CNT >= 2) {
            chess_board[x][y] = pre_val;
            return 1;
        }//先手落在该位置含有双三则为禁手

        int LIVE4_CNT = 0;
        if (main_diagonal(x, y, type) == LIVE4 || main_diagonal(x, y, type) == ASIDE4) LIVE4_CNT++;
        if (counter_diagonal(x, y, type) == LIVE4 || counter_diagonal(x, y, type) == ASIDE4) LIVE4_CNT++;
        if (column(x, y, type) == LIVE4 || column(x, y, type) == ASIDE4) LIVE4_CNT++;
        if (row(x, y, type) == LIVE4 || row(x, y, type) == ASIDE4) LIVE4_CNT++;
        if (LIVE4_CNT >= 2) {
            chess_board[x][y] = pre_val;
            return 2;
        }//先手落在该位置含有双四则为禁手

        int WINWIN_CNT = 0;

        //hang
        int i = x, l = y;
        while (!judge_out_of_board(i, l) && chess_board[i][l] == type) {
            l--;
        }
        int r = y;
        while (!judge_out_of_board(i, r) && chess_board[i][r] == type) {
            r++;
        }
        if (r - 1 - l >= 6) WINWIN_CNT++;
        //lie
        int j = y, u = x;
        while (!judge_out_of_board(u, j) && chess_board[u][j] == type) {
            u--;
        }
        int d = x;
        while (!judge_out_of_board(d, j) && chess_board[d][j] == type) {
            d++;
        }
        if (d - 1 - u >= 6) WINWIN_CNT++;
        //xie
        u = x, r = y;
        while (!judge_out_of_board(u, r) && chess_board[u][r] == type) {
            r++, u--;
        }
        d = x, l = y;
        while (!judge_out_of_board(d, l) && chess_board[d][l] == type) {
            d++, l--;
        }
        if (r - 1 - l >= 6) WINWIN_CNT++;
        //xie
        u = x, l = y;
        while (!judge_out_of_board(u, l) && chess_board[u][l] == type) {
            l--, u--;
        }
        d = x, r = y;
        while (!judge_out_of_board(d, r) && chess_board[d][r] == type) {
            d++, r++;
        }
        if (r - 1 - l >= 6) WINWIN_CNT++;
        if (WINWIN_CNT) {
            chess_board[x][y] = pre_val;
            return 3;
        }//先手落在该位置含有双四则为禁手
        chess_board[x][y] = pre_val;
        return 0;
    } else return 0; //白子为后手不是禁手
}//判断是否为禁手


//-------------------估值函数

int PlayerPlayer::judge_score(std::vector<int>chess) {
    int mycolor = chess[4];
    int hiscolor;

    int left, right;//开始和中心线断开的位置
    int colorleft, colorright;//开始和中心线断开的颜色，NOTHING或者hiscolor
    int count = 1;//中心线有多少个，初始化

    if (mycolor == BLACKFLAG)
        hiscolor = WHITEFLAG;
    else
        hiscolor = BLACKFLAG;//判断棋子

    for (int i = 1; i <= 4; i++) {
        if (chess[4 - i] == mycolor)
            count++;//同色
        else {
            left = 4 - i;//保存断开位置
            colorleft = chess[4 - i];//保存断开颜色
            break;
        }
    }
    for (int i = 1; i <= 4; i++) {
        if (chess[4 + i] == mycolor)
            count++;//同色
        else {
            right = 4 + i;//保存断开位置
            colorright = chess[4 + i];//保存断开颜色
            break;
        }
    }
    if (count > 5) //长连
        return WINWIN;

    if (count == 5)//中心线5连
        return WIN5;//5连

    if (count == 4) { //中心线4连
        if (colorleft == 0 && colorright == 0)//两边断开位置均空
            return LIVE4;//活四
        else if (colorleft == hiscolor && colorright == hiscolor)//两边断开位置均非空
            return NOTHREAT;//没有威胁
        else if (colorleft == EMPTYFLAG || colorright == EMPTYFLAG)//两边断开位置只有一个空
            return ASIDE4;//死四
    }

    if (count == 3) {//中心线3连
        int colorleft1 = chess[left - 1];
        int colorright1 = chess[right + 1];

        if (colorleft == EMPTYFLAG && colorright == EMPTYFLAG) { //两边断开位置均空

            if (colorleft1 == hiscolor && colorright1 == hiscolor)//均为对手棋子
                return DIE3;//X.OOO.X
            else if (colorleft1 == mycolor || colorright1 == mycolor)//只要一个为自己的棋子
                return ASIDE4;//X.OOO.O
            else if (colorleft1 == EMPTYFLAG || colorright1 == EMPTYFLAG)//只要有一个空
                return LIVE3;//..OOO..

        } else if (colorleft == hiscolor && colorright == hiscolor) { //两边断开位置均非空
            return NOTHREAT;//没有威胁
        } else if (colorleft == EMPTYFLAG || colorright == EMPTYFLAG) { //两边断开位置只有一个空
            if (colorleft == hiscolor) {//左边被对方堵住
                if (colorright1 == hiscolor)//右边也被对方堵住
                    return NOTHREAT;
                if (colorright1 == EMPTYFLAG)//右边空
                    return DIE3;//XOOO..
                if (colorright1 == mycolor)
                    return DIE4;//XOOO.O

            }
            if (colorright == hiscolor) {//右边被对方堵住
                if (colorleft1 == hiscolor)//左边也被对方堵住
                    return NOTHREAT;
                if (colorleft1 == EMPTYFLAG)//左边均空
                    return DIE3;
                if (colorleft1 == mycolor)//左边还有自己的棋子
                    return DIE4;
            }
        }
    }

    if (count == 2) {//中心线2连
        int colorleft1 = chess[left - 1];
        int colorright1 = chess[right + 1];
        int colorleft2 = chess[left - 2];
        int colorright2 = chess[right + 2];

        if (colorleft == EMPTYFLAG && colorright == EMPTYFLAG) { //两边断开位置均空
            if ((colorright1 == EMPTYFLAG && colorright2 == mycolor) ||
                (colorleft1 == EMPTYFLAG && colorleft2 == mycolor))//O.OO.
                return DIE3;//死3
            else if (colorleft1 == EMPTYFLAG && colorright1 == EMPTYFLAG)
                return LIVE2;//活2

            if ((colorright1 == mycolor && colorright2 == hiscolor) ||
                (colorleft1 == mycolor && colorleft2 == hiscolor))
                return DIE3;//死3

            if ((colorright1 == mycolor && colorright2 == mycolor) ||
                (colorleft1 == mycolor && colorleft2 == mycolor))
                return DIE4;//死4

            if ((colorright1 == mycolor && colorright2 == EMPTYFLAG) ||
                (colorleft1 == mycolor && colorleft2 == EMPTYFLAG))
                return LIVE3;//跳活3
            return NOTHREAT;
        } else if (colorleft == hiscolor && colorright == hiscolor) { //两边断开位置均非空
            return NOTHREAT;
        } else if (colorleft == EMPTYFLAG || colorright == EMPTYFLAG) { //两边断开位置只有一个空
            if (colorleft == hiscolor) {//左边被对方堵住
                if (colorright1 == hiscolor || colorright2 == hiscolor) {//只要有对方的一个棋子
                    return NOTHREAT;//没有威胁
                } else if (colorright1 == EMPTYFLAG && colorright2 == EMPTYFLAG) { //均空XOO..
                    return DIE2;//死2
                } else if (colorright1 == mycolor && colorright2 == mycolor) { //均为自己的棋子
                    return DIE4;//死4
                } else if (colorright1 == mycolor || colorright2 == mycolor) { //只有一个自己的棋子
                    return DIE3;//死3
                }
            }
            if (colorright == hiscolor) {//右边被对方堵住
                if (colorleft1 == hiscolor || colorleft2 == hiscolor) {//只要有对方的一个棋子
                    return NOTHREAT;//没有威胁
                } else if (colorleft1 == EMPTYFLAG && colorleft2 == EMPTYFLAG) { //均空
                    return DIE2;//死2
                } else if (colorleft1 == mycolor && colorleft2 == mycolor) { //均为自己的棋子
                    return DIE4;//死4
                } else if (colorleft1 == mycolor || colorleft2 == mycolor) { //只有一个自己的棋子
                    return DIE3;//死3
                }
            }
        }
    }

    if (count == 1) {//中心线1连
        int colorleft1 = chess[left - 1];
        int colorright1 = chess[right + 1];
        int colorleft2 = chess[left - 2];
        int colorright2 = chess[right + 2];
        int colorleft3 = chess[left - 3];
        int colorright3 = chess[right + 3];

        if (colorleft == EMPTYFLAG && colorleft1 == mycolor &&
            colorleft2 == mycolor && colorleft3 == mycolor)
            return DIE4;
        if (colorright == EMPTYFLAG && colorright1 == mycolor &&
            colorright2 == mycolor && colorright3 == mycolor)
            return DIE4;//判断死4

        if (colorleft == EMPTYFLAG && colorleft1 == mycolor &&
            colorleft2 == mycolor && colorleft3 == EMPTYFLAG && colorright == EMPTYFLAG)
            return LIVE3;
        if (colorright == EMPTYFLAG && colorright1 == mycolor &&
            colorright2 == mycolor && colorright3 == EMPTYFLAG && colorleft == EMPTYFLAG)
            return LIVE3;

        if (colorleft == EMPTYFLAG && colorleft1 == mycolor &&
            colorleft2 == mycolor && colorleft3 == hiscolor && colorright == EMPTYFLAG)
            return DIE3;
        if (colorright == EMPTYFLAG && colorright1 == mycolor &&
            colorright2 == mycolor && colorright3 == hiscolor && colorleft == EMPTYFLAG)
            return DIE3;

        if (colorleft == EMPTYFLAG && colorleft1 == EMPTYFLAG &&
            colorleft2 == mycolor && colorleft3 == mycolor)
            return DIE3;
        if (colorright == EMPTYFLAG && colorright1 == EMPTYFLAG &&
            colorright2 == mycolor && colorright3 == mycolor)
            return DIE3;

        if (colorleft == EMPTYFLAG && colorleft1 == mycolor &&
            colorleft2 == EMPTYFLAG && colorleft3 == mycolor)
            return DIE3;
        if (colorright == EMPTYFLAG && colorright1 == mycolor &&
            colorright2 == EMPTYFLAG && colorright3 == mycolor)
            return DIE3;

        if (colorleft == EMPTYFLAG && colorleft1 == mycolor &&
            colorleft2 == EMPTYFLAG && colorleft3 == EMPTYFLAG && colorright == EMPTYFLAG)
            return LIVE2;
        if (colorright == EMPTYFLAG && colorright1 == mycolor &&
            colorright2 == EMPTYFLAG && colorright3 == EMPTYFLAG && colorleft == EMPTYFLAG)
            return LIVE2;

        if (colorleft == EMPTYFLAG && colorleft1 == EMPTYFLAG &&
            colorleft2 == mycolor && colorleft3 == EMPTYFLAG && colorright == EMPTYFLAG)
            return LIVE2;
        if (colorright == EMPTYFLAG && colorright1 == EMPTYFLAG &&
            colorright2 == mycolor && colorright3 == EMPTYFLAG && colorleft == EMPTYFLAG)
            return LIVE2;

        return NOTHREAT;

    }
    return NOTHREAT;//返回没有威胁
}


//-----------------判断行，列，对角线是否有威胁
//判断主对角线
int PlayerPlayer::main_diagonal(int x, int y, int type) {
    std::vector<int>chess_type(9, 3);
    chess_type[4] = type;
    for (int i = 1; i <= 4; i++) {
        if (judge_out_of_board(x - i, y - i)) {
            for (int j = i; j <= 4; j++) {
                chess_type[4 - j] = type % 2 + 1;
            }
            break;
        } else {
            chess_type[4 - i] = chess_board[x - i][y - i];
        }
    }

    for (int i = 1; i <= 4; i++) {
        if (judge_out_of_board(x + i, y + i)) {
            for (int j = i; j <= 4; j++) {
                chess_type[4 + j] = type % 2 + 1;
            }
            break;
        } else {
            chess_type[4 + i] = chess_board[x + i][y + i];
        }
    }

    return judge_score(chess_type);
}

//判断副对角线
int PlayerPlayer::counter_diagonal(int x, int y, int type) {
    std::vector<int>chess_type(9, 3);
    chess_type[4] = type;
    for (int i = 1; i <= 4; i++) {
        if (judge_out_of_board(x + i, y - i)) {
            for (int j = i; j <= 4; j++) {
                chess_type[4 - j] = type % 2 + 1;
            }
            break;
        } else {
            chess_type[4 - i] = chess_board[x + i][y - i];
        }
    }

    for (int i = 1; i <= 4; i++) {
        if (judge_out_of_board(x - i, y + i)) {
            for (int j = i; j <= 4; j++) {
                chess_type[4 + j] = type % 2 + 1;
            }
            break;
        } else {
            chess_type[4 + i] = chess_board[x - i][y + i];
        }
    }

    return judge_score(chess_type);
}

//判断行
int PlayerPlayer::row(int x, int y, int type) {
    std::vector<int>chess_type(9, 3);
    chess_type[4] = type;
    for (int i = 1; i <= 4; i++) {
        if (judge_out_of_board(x, y - i)) {
            for (int j = i; j <= 4; j++) {
                chess_type[4 - j] = type % 2 + 1;
            }
            break;
        } else {
            chess_type[4 - i] = chess_board[x][y - i];
        }
    }

    for (int i = 1; i <= 4; i++) {
        if (judge_out_of_board(x, y + i)) {
            for (int j = i; j <= 4; j++) {
                chess_type[4 + j] = type % 2 + 1;
            }
            break;
        } else {
            chess_type[4 + i] = chess_board[x][y + i];
        }
    }

    return judge_score(chess_type);
}

//判断列
int PlayerPlayer::column(int x, int y, int type) {
    std::vector<int>chess_type(9, 3);
    chess_type[4] = type;
    for (int i = 1; i <= 4; i++) {
        if (judge_out_of_board(x - i, y)) {
            for (int j = i; j <= 4; j++) {
                chess_type[4 - j] = type % 2 + 1;
            }
            break;
        } else {
            chess_type[4 - i] = chess_board[x - i][y];
        }
    }

    for (int i = 1; i <= 4; i++) {
        if (judge_out_of_board(x + i, y)) {
            for (int j = i; j <= 4; j++) {
                chess_type[4 + j] = type % 2 + 1;
            }
            break;
        } else {
            chess_type[4 + i] = chess_board[x + i][y];
        }
    }

    return judge_score(chess_type);
}

bool PlayerPlayer::judge_winner(int x, int y, int type) {
    if (column(x, y, type) >= WIN5) return 1;
    if (row(x, y, type) >= WIN5) return 1;
    if (main_diagonal(x, y, type) >= WIN5) return 1;
    if (counter_diagonal(x, y, type) >= WIN5) return 1;
    return 0;
}//判断获胜者

