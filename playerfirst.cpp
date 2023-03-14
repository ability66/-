#include "playerfirst.h"
#include "robotway.h"
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
PlayerFirst::PlayerFirst(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //关联返回按钮点击事件与窗体关闭事件
    bt = new QPushButton(this);
    bt->setGeometry(1200, 0, 250, 50);
    bt->setStyleSheet("QPushButton{border-image:url(:/222/images/back_to_menu.png)}");
    connect(bt, &QPushButton::clicked, this, &PlayerFirst::sendSlot);
    bt->show();

    //触发bt 然后本窗体的槽函数接受触发信息 然后槽函数函数体执行 发出信号

    rule = new QPushButton(this);
    rule->setGeometry(0, 0, 250, 50);
    rule->setStyleSheet("QPushButton{border-image:url(:/222/images/rule.png)}");
    connect(rule, &QPushButton::clicked, this, &PlayerFirst::show_rule);

    stopgame = new QPushButton(this);
    stopgame->setGeometry(1090, 430, 250, 50);
    stopgame->setStyleSheet("QPushButton{border-image:url(:/222/images/defeat.png)}");
    connect(stopgame, &QPushButton::clicked, this, &PlayerFirst::defeat);

    regret = new QPushButton(this);
    regret->setGeometry(1090, 500, 250, 50);
    regret->setStyleSheet("QPushButton{border-image:url(:/222/images/regret.png)}");
    connect(regret, &QPushButton::clicked, this, &PlayerFirst::try_regret);

    draw = new QPushButton(this);
    draw->setGeometry(1090, 570, 250, 50);
    draw->setStyleSheet("QPushButton{border-image:url(:/222/images/draw.png)}");
    connect(draw, &QPushButton::clicked, this, &PlayerFirst::try_draw);
    //鼠标监听
    setMouseTracking(true);
    //设置窗口大小
    this->setFixedSize(1400, 740); //设置窗口大小
    setWindowTitle("五子棋-玩家先手");
    initGame();
}

void PlayerFirst::sendSlot() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    emit closed();
}
void PlayerFirst::defeat() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    QSound *sound = new QSound(":/222/sound/lose.wav");
    sound ->play();//播放函数
    QMessageBox::StandardButton btnValue = QMessageBox::information(this, "游戏结束", "白棋胜利! ");
    if(btnValue==QMessageBox::Ok) initGame();
}
void PlayerFirst::try_draw() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    QMessageBox::information(this, "失败", "电脑拒绝与你求和 ");
}
void PlayerFirst::try_regret() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    if(player==BLACKFLAG&&step.size()) {
        chess_board[step.top().first][step.top().second]=EMPTYFLAG;
        step.pop();
        chess_board[step.top().first][step.top().second]=EMPTYFLAG;
        step.pop();
        repaint();
    }
    else {
        QMessageBox::information(this, "失败", "悔棋失败 ");
    }
}
void PlayerFirst::show_rule() {
    QSound *cl = new QSound(":/222/sound/click.wav");
    cl ->play();//播放函数
    QMessageBox::information(this, "规则", "规则要求：每人一次下一子，黑棋先下（为先手），白棋后下，黑棋第一步必须走天元（图中黑点），黑白双方轮流交替下子，直到黑白中任意一方获胜。\n获胜的判定：只要黑白方中任意一方的棋子有五子连在一起（即五子连珠），可以为横连、纵连、斜连，则该方获胜，游戏结束。\n延伸知识：黑棋虽先行，但有禁手：黑方不能在一步之内形成两个“活三”“活四”或一步之内形成“长连”（指一步形成超过五子连珠）。白方自由，无禁手。 ");
}
PlayerFirst::~PlayerFirst() {
    delete ui;
}

//绘制事件
void PlayerFirst::paintEvent(QPaintEvent *) {
    //绘制棋盘边界边框，棋盘内颜色
    QPainter painter(this);
    painter.drawPixmap(QRect(0, 0, 1400, 740), QPixmap(":/222/images/playerfirst_bg.png"));

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
void PlayerFirst::mouseMoveEvent(QMouseEvent *event) {
    selectPos = 0;
    moveX = (event->y() - 40-50) / 40;
    moveY = (event->x() - 20-370) / 40;
    if(moveX>=0&&moveX<=14&&moveY>=0&&moveY<=14)selectPos = 1;
    repaint(); //更新后就重画
}

//鼠标点击事件
void PlayerFirst::mouseReleaseEvent(QMouseEvent *event) {

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
    count[clickPosRow + 4][clickPosCol + 4] = player;
    QSound *cl = new QSound(":/222/sound/chess_down.wav");
    cl ->play();//播放函数
    step.push({clickPosRow, clickPosCol});
    repaint();
    QString text_player = "人坐标：" + QString::number(clickPosRow + 1) + "," + QString::number(clickPosCol + 1);
    pos_player.push_back(text_player);
    //判断输赢
    add_search_set(clickPosRow, clickPosCol);
    std::vector<std::vector<int>>dir;
    dir.push_back(counter_diagonal(clickPosRow, clickPosCol, player));
    dir.push_back(main_diagonal(clickPosRow, clickPosCol, player));
    dir.push_back(row(clickPosRow, clickPosCol, player));
    dir.push_back(column(clickPosRow, clickPosCol, player));
    if (clickPosRow >= 0 && clickPosRow <= boardSize && clickPosCol >= 0 && clickPosCol <= boardSize && (chess_board[clickPosRow][clickPosCol] == BLACKFLAG || chess_board[clickPosRow][clickPosCol] == WHITEFLAG)) {
        if (judge_pre_forbiden(clickPosRow, clickPosCol, player) && player == BLACKFLAG) {
            QString str;
            QSound *sound = new QSound(":/222/sound/lose.wav");
            sound ->play();//播放函数
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "游戏结束", "禁手! ");           
            std::ofstream out("feed_back.out");
            out << "PLAYER:\n";
            for (auto s : pos_player) {
                std::string str = s.toStdString();
                out << str << '\n';
            }
            out << "AI:\n";
            for (auto s : pos_ai) {
                std::string str = s.toStdString();
                out << str << '\n';
            }
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

            std::ofstream out("feed_back.out");
            out << "PLAYER:\n";
            for (auto s : pos_player) {
                std::string str = s.toStdString();
                out << str << '\n';
            }
            out << "AI:\n";
            for (auto s : pos_ai) {
                std::string str = s.toStdString();
                out << str << '\n';
            }
            if (btnValue == QMessageBox::Ok) {
                initGame();
            }
        } else {
            sum_step++;
            if (player == BLACKFLAG)player = WHITEFLAG;
            else player = BLACKFLAG;
            playByAI();
        }
    }
}


void PlayerFirst::initGame() {
    sum_step=0;
    player = 1;
    chess_board.resize(boardSize + 1);
    for (int i = 0; i < boardSize + 1; i++)chess_board[i].resize(boardSize + 1);
    for (int i = 0; i <= boardSize; i++) {
        for (int j = 0; j <= boardSize; j++) {
            chess_board[i][j] = EMPTYFLAG;
        }
    }
    search_set.clear();
    score.atk_val = 0;
    score.def_val = 0;
    while(step.size()) step.pop();
    for (int i = 0; i < 23; i++)
        {
            for (int j = 0; j < 23 ;j++)
            {
                if ((i < 4 || i >= 19) && (j < 4 || j >= 19))
                {
                    count[i][j] = -1;
                }
                else count[i][j]=0;
            }
        }
}


//落子合法性判断
bool PlayerFirst::judge_out_of_board(int x, int y) {
    if (x >= 15 || x < 0 || y >= 15 || y < 0) return 1;
    return 0;
}//是否越界

int PlayerFirst::judge_pre_forbiden(int x, int y, int type) {
    if (type == BLACKFLAG) {
        std::vector<std::vector<int>>dir;
        dir.push_back(counter_diagonal(x,y, type));
        dir.push_back(main_diagonal(x,y, type));
        dir.push_back(row(x,y, type));
        dir.push_back(column(x,y, type));
        if(Robotway::judgeChessSituation(dir)==Robotway::Leveltwo) {
            return 1;
        }
        if(Robotway::judgeChessSituation(dir)==Robotway::Levelthree) {
            return 2;
        }
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
            return 3;
        }//先手落在该位置含有双四则为禁手
        return 0;
    } else return 0; //白子为后手不是禁手
}//判断是否为禁手

//-----------------判断行，列，对角线是否有威胁
//判断主对角线
std::vector<int> PlayerFirst::main_diagonal(int x, int y, int type) {
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

    return chess_type;
}

//判断副对角线
std::vector<int> PlayerFirst::counter_diagonal(int x, int y, int type) {
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

    return chess_type;
}

//判断行
std::vector<int> PlayerFirst::row(int x, int y, int type) {
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

    return chess_type;
}

//判断列
std::vector<int> PlayerFirst::column(int x, int y, int type) {
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

    return chess_type;
}

//分别计算攻击分数和防守分数
int PlayerFirst::atk_val(int i, int j, int type) {
    std::vector<std::vector<int>>dir;
    dir.push_back(counter_diagonal(i, j, type));
    dir.push_back(main_diagonal(i, j, type));
    dir.push_back(row(i, j, type));
    dir.push_back(column(i, j, type));
    return Robotway::judgeChessSituation(dir);
}
int PlayerFirst::def_val(int i, int j, int type) {
    type = type % 2 + 1;
    std::vector<std::vector<int>>dir;
    dir.push_back(counter_diagonal(i, j, type));
    dir.push_back(main_diagonal(i, j, type));
    dir.push_back(row(i, j, type));
    dir.push_back(column(i, j, type));
    return Robotway::judgeChessSituation(dir);
}

//下一步得分和当前步加权计算
PlayerFirst::value PlayerFirst::cal_score(PlayerFirst::value nxt, PlayerFirst::value now) {
    return now + nxt;
}

void PlayerFirst::Grade()//计算所有位置的得分
{
    int i, j, m, n;
    int num1, num2;
    for (i = 4; i < 19; i++)
    {
        for (j = 4; j < 19; j++)
        {
            if (chess_board[i - 4][j - 4]==EMPTYFLAG)
            {
                sum[i][j] = 0;//循环初始化sum[i][j]
                Base(i, j);//坐标为(i-4,j-4)的所有五连珠情况
                for (m = 0; m < 20; m++)
                {
                    num1 = 0; num2 = 0;
                    for (n = 0; n < 5; n++)
                    {
                        if (p[m][n] == player){
                            if(i==9&&j==14) qDebug()<<n<<"**********";
                            num1++;}
                        else if (p[m][n] == player%2+1)
                            num2++;
                        else if (p[m][n] == -1)
                        {
                            num1 = -1; num2 = -1;
                            break;
                        }
                    }
                    if(i==9&&j==14) qDebug()<<sum[i][j];
                    sum[i][j] += Assessment(num1, num2);
                }
            }
        }
    }
}

void PlayerFirst::Base(int x, int y)//坐标为(x-4,y-4)的所有五连珠情况
{
    //左上->右下
    for(int i=0;i<=4;i++) {
        for(int j=4;j>=0;j--) {
            p[i][4-j]=count[x-j+i][y-j+i];
        }
    }
    //上->下
    for(int i=5;i<=9;i++) {
        for(int j=4;j>=0;j--) {
            p[i][4-j]=count[x-j+(i-5)][y];
        }
    }
    //右上->左下
    for(int i=10;i<=14;i++) {
        for(int j=4;j>=0;j--) {
            p[i][4-j]=count[x-j+(i-10)][y+j-(i-10)];
        }
    }
    //左->右
    for(int i=15;i<=19;i++) {
        for(int j=4;j>=0;j--) {
            p[i][4-j]=count[x][y-j+(i-15)];
        }
    }
}

int PlayerFirst::Assessment(int num1, int num2)//评分标准
{
    if (num2 == 0)//判断电脑五元组得分
    {
        switch (num1)
        {
            case 0: return 7;
            case 1: return 35;
            case 2: return 800;
            case 3: return 15000;
            case 4: return 800000;
        }
    }
    if (num2 != 0 && num1 == 0)//判断玩家五元组得分
    {
        switch (num2)
        {
            case 1: return 15;
            case 2: return 400;
            case 3: return 1800;
            case 4: return 100000;
        }
    }
    return 0;
}

void PlayerFirst::Find_point(int *pp, int *qq)//找到最佳性最优的下子位置
{
    int i, j, flag = 0;//flag为结束标志
    int t = 0;
    int r, c = 0, sumed[225][2];
    //寻找第一个未下子位置对应的sum值
    for (i = 4; i < 19 && flag == 0; i++)
    {
        for (j = 4; j < 19 && flag == 0; j++)
        {
            if (chess_board[i - 4][j - 4]==EMPTYFLAG&&judge_pre_forbiden(i-4,j-4,player)==0)
            {
                t = sum[i][j];
                flag = 1;
            }
        }
    }
    //寻找最大的sum值
    for (i = 4; i < 19; i++)
    {
        for (j = 4; j < 19; j++)
        {
            if (chess_board[i - 4][j - 4]==EMPTYFLAG && sum[i][j] > t&&judge_pre_forbiden(i-4,j-4,player)==0)
            {
                t = sum[i][j];
            }
        }
    }
    //存储所有的最大sum值
    for (i = 4; i < 19; i++)
    {
        for (j = 4; j < 19; j++)
        {
            if (chess_board[i - 4][j - 4]==EMPTYFLAG && sum[i][j] == t&&judge_pre_forbiden(i-4,j-4,player)==0)
            {
                sumed[c][0] = i;
                sumed[c][1] = j;
                c++;
            }
        }
    }
    *pp = sumed[0][0];
    *qq = sumed[0][1];
}

//深度优先搜索
std::pair<PlayerFirst::value, std::pair<int, int>> PlayerFirst::bfs(int type, int step, value now_score,value alpha,value beta) {
    std::vector<std::pair<PlayerFirst::value, std::pair<int, int>>>res;
    bool if_rush = 0;
    bool if_die = 0;
    for (auto [p, _] : search_set) {
        int i = p.first;
        int j = p.second;
        if (chess_board[i][j] == 0 && judge_pre_forbiden(i, j, type) == 0) {
            value tot;
            tot.def_val = def_val(i, j, type);
            tot.atk_val = atk_val(i, j, type);
            if (tot.atk_val >= Robotway::LevelOne) {
                if (type == player % 2 + 1) {
                    return {{-INF,-INF},{i,j}};
                } else {
                    return {{INF, INF}, {i, j}};
                }
            }
            if (tot.atk_val >= Robotway::Levelfive) {
                if_rush = 1;
            }
            if (tot.def_val >= Robotway::LevelEight&&step==0) {
                int Cp = 15 / 2, Cq = 15 / 2;
                Grade();//计算所有位置的得分
                Find_point(&Cp, &Cq);//找到最佳性最优的下子位置
                return {{0,0},{Cp - 4,Cq - 4}};
            }
            if (type == player) {
                tot.atk_val = now_score.atk_val + tot.atk_val;
                tot.def_val = now_score.def_val + tot.def_val;
            } else {
                tot.atk_val = now_score.atk_val - tot.def_val;
                tot.def_val = now_score.def_val - tot.atk_val;
            }
            res.push_back({tot, {i, j}});
        }
    }//枚举每个可以下棋的位置，分别判断进攻防守分数
    if (type == player) sort(res.begin(), res.end(), std::greater<std::pair<value, std::pair<int, int>>>());
    else sort(res.begin(), res.end());
    //按从大到小顺序给进攻防守分加权排序
    int to_x = res.begin()->second.first, to_y = res.begin()->second.second;
    if (step == 0) {
        //if (!if_die) {
            if (if_rush) {
                int x, y;
                long long now_atk_score = -INF;
                long long now_def_score = -INF;
                for (auto [p, _] : search_set) {
                    int i = p.first;
                    int j = p.second;
                    if (chess_board[i][j] == 0 && judge_pre_forbiden(i, j, type) == 0) {
                        value tot;
                        tot.def_val = def_val(i, j, type);
                        tot.atk_val = atk_val(i, j, type);
                        if (now_atk_score < tot.atk_val) {
                            x = i;
                            y = j;
                            now_atk_score = tot.atk_val;
                            now_def_score = tot.def_val;
                        }
                        else if(now_atk_score == tot.atk_val&&now_def_score < tot.def_val) {
                            x = i;
                            y = j;
                            now_atk_score = tot.atk_val;
                            now_def_score = tot.def_val;
                        }
                    }
                }//枚举每个可以下棋的位置，分别判断进攻防守分数
                return {{0, 0}, {x, y}};
            }
        //}
    }
    if (step != 3 && step + sum_step <=225) {
//        std::vector<std::pair<value, std::pair<int, int>>>next_dep;
//        for (int i = 0; i < res.size() && i < 5; i++) {
//            to_x = res[i].second.first, to_y = res[i].second.second;
//            chess_board[to_x][to_y] = type;//假设下在这步，思考下一步情况
//            std::pair<value, std::pair<int, int>>tmp;
//            ifcut = 0;
//            if (type == player % 2 + 1) tmp = bfs(type % 2 + 1, step + 1, res[i].first);
//            else tmp = bfs(type % 2 + 1, step + 1, res[i].first);
//            chess_board[to_x][to_y] = 0;
//            std::pair<value, std::pair<int, int>>nxt;
//            nxt.second = {to_x, to_y};
//            nxt.first =  cal_score(nxt.first, res[i].first);
//            if (!ifcut) next_dep.push_back(nxt);
//            ifcut = 0;
//        }
//        sort(next_dep.begin(), next_dep.end());//按得分从大到小排序
//        if (type == player)return *prev(next_dep.end()); //返回得分可能的最大值
//        else return *next_dep.begin();
        if(type==player) {//max
            int x=to_x,y=to_y;
            for(int i=0;i<res.size();i++) {
                to_x = res[i].second.first, to_y = res[i].second.second;
                chess_board[to_x][to_y] = type;//假设下在这步，思考下一步情况
                std::pair<value, std::pair<int, int>>tmp;
                tmp = bfs(type % 2 + 1, step + 1, res[i].first,alpha,beta);
                chess_board[to_x][to_y] = 0;
                if(alpha<tmp.first) {
                    alpha=tmp.first;
                    x=res[i].second.first;
                    y=res[i].second.second;
                }
                if(beta<alpha) return {alpha,{x,y}};
            }
            return {alpha,{x,y}};
        }
        else {//min
            int x=to_x,y=to_y;
            for(int i=0;i<res.size();i++) {
                to_x = res[i].second.first, to_y = res[i].second.second;
                chess_board[to_x][to_y] = type;//假设下在这步，思考下一步情况
                std::pair<value, std::pair<int, int>>tmp;
                tmp = bfs(type % 2 + 1, step + 1, res[i].first,alpha,beta);
                chess_board[to_x][to_y] = 0;
                if(tmp.first<beta) {
                    beta=tmp.first;
                    x=res[i].second.first;
                    y=res[i].second.second;
                }
                if(beta<alpha) return {beta,{x,y}};
            }
            return {beta,{x,y}};
        }

    } else return *res.begin();//如果已经搜到底层，则返回最大可能得分情况
}

void PlayerFirst::playByAI() {
    ifcut = 0;
    std::pair<PlayerFirst::value, std::pair<int, int>>computer = bfs(player, 0, {0, 0},{-INF,-INF},{INF,INF});
    chess_board[computer.second.first][computer.second.second] = player;
    count[computer.second.first + 4][computer.second.second + 4] = player;
    QSound *cl = new QSound(":/222/sound/chess_down.wav");
    cl ->play();//播放函数
    step.push({computer.second.first, computer.second.second});
    add_search_set(computer.second.first, computer.second.second);
    QString text_ai = "ai坐标：" + QString::number(computer.second.first + 1) + "," + QString::number(computer.second.second + 1);
    pos_ai.push_back(text_ai);
    this->statusBar()->showMessage(text_ai);
    repaint();
    if (judge_pre_forbiden(computer.second.first, computer.second.second, player) && player == BLACKFLAG) {
        QString str;
        QSound *sound = new QSound(":/222/sound/win.wav");
        sound ->play();//播放函数
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "游戏结束", "禁手! ");      
        std::ofstream out("feed_back.out");
        out << "PLAYER:\n";
        for (auto s : pos_player) {
            std::string str = s.toStdString();
            out << str << '\n';
        }
        out << "AI:\n";
        for (auto s : pos_ai) {
            std::string str = s.toStdString();
            out << str << '\n';
        }
        if (btnValue == QMessageBox::Ok) {
            initGame();
        }
    } else if (judge_winner(computer.second.first, computer.second.second, player)) {
        QString str;
        if (player == BLACKFLAG)str = "黑棋";
        else str = "白棋" ;
        QSound *sound = new QSound(":/222/sound/lose.wav");
        sound ->play();//播放函数
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "游戏结束", str + "胜利! ");

        std::ofstream out("feed_back.out");
        out << "PLAYER:\n";
        for (auto s : pos_player) {
            std::string str = s.toStdString();
            out << str << '\n';
        }
        out << "AI:\n";
        for (auto s : pos_ai) {
            std::string str = s.toStdString();
            out << str << '\n';
        }
        if (btnValue == QMessageBox::Ok) {
            initGame();
        }
    } else {
        sum_step++;
        if (player == BLACKFLAG)player = WHITEFLAG;
        else player = BLACKFLAG;
    }
}
bool PlayerFirst::judge_winner(int x, int y, int type) {
    if (Robotway::judge_score(column(x, y, type)) <= Robotway::WIN5) return 1;
    if (Robotway::judge_score(row(x, y, type)) <= Robotway::WIN5) return 1;
    if (Robotway::judge_score(main_diagonal(x, y, type)) <= Robotway::WIN5) return 1;
    if (Robotway::judge_score(counter_diagonal(x, y, type)) <= Robotway::WIN5) return 1;
    return 0;
}//判断获胜者

void PlayerFirst::add_search_set(int x, int y) {
    if (search_set.find({x, y}) != search_set.end()) search_set.erase({x, y});
    int xx = x, yy = y;
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (!judge_out_of_board(xx + i, yy + j) && chess_board[xx + i][yy + j] == EMPTYFLAG) {
                search_set[ {xx + i, yy + j}]++;
            }
        }
    }
}
