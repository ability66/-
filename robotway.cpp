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

Robotway::Robotway(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

Robotway::~Robotway() {
    delete ui;
}
int Robotway::judge_score(std::vector<int>chess) {
    int mycolor = chess[4];
        int hiscolor;

        int left, right;//开始和中心线断开的位置
        int colorleft, colorright;//开始和中心线断开的颜色，NOTHING或者hiscolor
        int count = 1;//中心线有多少个，初始化

        if (mycolor == BLACKFLAG)
            hiscolor = WHITEFLAG;
        else
            hiscolor = BLACKFLAG;

        for (int i = 1;i <= 4;i++) {
            if (chess[4 - i] == mycolor)
                count++;//同色
            else {
                left = 4 - i;//保存断开位置
                colorleft = chess[4 - i];//保存断开颜色
                break;
            }
        }
        for (int i = 1;i <= 4;i++) {
            if (chess[4 + i] == mycolor)
                count++;//同色
            else {
                right = 4 + i;//保存断开位置
                colorright = chess[4 + i];//保存断开颜色
                break;
            }
        }


        if (count >= 5)//中心线5连
            return WIN5;//5连珠

        if (count == 4)//中心线4连
        {
            if (colorleft == EMPTYFLAG && colorright == EMPTYFLAG)//两边断开位置均空
                return ALIVE4;//活四
            else if (colorleft == hiscolor && colorright == hiscolor)//两边断开位置均非空
                return NOTHREAT;//没有威胁
            else if (colorleft == EMPTYFLAG || colorright == EMPTYFLAG)//两边断开位置只有一个空
                return DIE4;//死四
        }

        if (count == 3) {//中心线3连
            int colorleft1 = chess[left - 1];
            int colorright1 = chess[right + 1];

            if (colorleft == EMPTYFLAG && colorright == EMPTYFLAG)//两边断开位置均空
            {

                if (colorleft1 == hiscolor && colorright1 == hiscolor)//均为对手棋子
                    return DIE3;
                else if (colorleft1 == mycolor || colorright1 == mycolor)//只要一个为自己的棋子
                    return LOWDIE4;
                else if (colorleft1 == EMPTYFLAG || colorright1 == EMPTYFLAG)//只要有一个空
                    return ALIVE3;

            }
            else if (colorleft == hiscolor && colorright == hiscolor)//两边断开位置均非空
            {
                return NOTHREAT;//没有威胁
            }
            else if (colorleft == EMPTYFLAG || colorright == EMPTYFLAG)//两边断开位置只有一个空
            {

                if (colorleft == hiscolor) {//左边被对方堵住
                    if (colorright1 == hiscolor)//右边也被对方堵住
                        return NOTHREAT;
                    if (colorright1 == EMPTYFLAG)//右边均空
                        return DIE3;
                    if (colorright1 == mycolor)
                        return LOWDIE4;

                }
                if (colorright == hiscolor) {//右边被对方堵住
                    if (colorleft1 == hiscolor)//左边也被对方堵住
                        return NOTHREAT;
                    if (colorleft1 == EMPTYFLAG)//左边均空
                        return DIE3;
                    if (colorleft1 == mycolor)//左边还有自己的棋子
                        return LOWDIE4;
                }
            }
        }

        if (count == 2) {//中心线2连
            int colorleft1 = chess[left - 1];
            int colorright1 = chess[right + 1];
            int colorleft2 = chess[left - 2];
            int colorright2 = chess[right + 2];

            if (colorleft == EMPTYFLAG && colorright == EMPTYFLAG)//两边断开位置均空
            {
                if ((colorright1 == EMPTYFLAG && colorright2 == mycolor) ||
                    (colorleft1 == EMPTYFLAG && colorleft2 == mycolor))
                    return DIE3;//死3
                else if (colorleft1 == EMPTYFLAG && colorright1 == EMPTYFLAG)
                    return ALIVE2;//活2

                if ((colorright1 == mycolor && colorright2 == hiscolor) ||
                    (colorleft1 == mycolor && colorleft2 == hiscolor))
                    return DIE3;//死3

                if ((colorright1 == mycolor && colorright2 == mycolor) ||
                    (colorleft1 == mycolor && colorleft2 == mycolor))
                    return LOWDIE4;//死4

                if ((colorright1 == mycolor && colorright2 == EMPTYFLAG) ||
                    (colorleft1 == mycolor && colorleft2 == EMPTYFLAG))
                    return TIAO3;//跳活3
                //其他情况在下边返回NOTHREAT
            }
            else if (colorleft == hiscolor && colorright == hiscolor)//两边断开位置均非空
            {
                return NOTHREAT;
            }
            else if (colorleft == EMPTYFLAG || colorright == EMPTYFLAG)//两边断开位置只有一个空
            {
                if (colorleft == hiscolor) {//左边被对方堵住
                    if (colorright1 == hiscolor || colorright2 == hiscolor) {//只要有对方的一个棋子
                        return NOTHREAT;//没有威胁
                    }
                    else if (colorright1 == EMPTYFLAG && colorright2 == EMPTYFLAG) {//均空
                        return DIE2;//死2
                    }
                    else if (colorright1 == mycolor && colorright2 == mycolor) {//均为自己的棋子
                        return LOWDIE4;//死4
                    }
                    else if (colorright1 == mycolor || colorright2 == mycolor) {//只有一个自己的棋子
                        return DIE3;//死3
                    }
                }
                if (colorright == hiscolor) {//右边被对方堵住
                    if (colorleft1 == hiscolor || colorleft2 == hiscolor) {//只要有对方的一个棋子
                        return NOTHREAT;//没有威胁
                    }
                    else if (colorleft1 == EMPTYFLAG && colorleft2 == EMPTYFLAG) {//均空
                        return DIE2;//死2
                    }
                    else if (colorleft1 == mycolor && colorleft2 == mycolor) {//均为自己的棋子
                        return LOWDIE4;//死4
                    }
                    else if (colorleft1 == mycolor || colorleft2 == mycolor) {//只有一个自己的棋子
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
                return LOWDIE4;
            if (colorright == EMPTYFLAG && colorright1 == mycolor &&
                colorright2 == mycolor && colorright3 == mycolor)
                return LOWDIE4;

            if (colorleft == EMPTYFLAG && colorleft1 == mycolor &&
                colorleft2 == mycolor && colorleft3 == EMPTYFLAG && colorright == EMPTYFLAG)
                return TIAO3;
            if (colorright == EMPTYFLAG && colorright1 == mycolor &&
                colorright2 == mycolor && colorright3 == EMPTYFLAG && colorleft == EMPTYFLAG)
                return TIAO3;

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
                return LOWALIVE2;
            if (colorright == EMPTYFLAG && colorright1 == mycolor &&
                colorright2 == EMPTYFLAG && colorright3 == EMPTYFLAG && colorleft == EMPTYFLAG)
                return LOWALIVE2;

            if (colorleft == EMPTYFLAG && colorleft1 == EMPTYFLAG &&
                colorleft2 == mycolor && colorleft3 == EMPTYFLAG && colorright == EMPTYFLAG)
                return LOWALIVE2;
            if (colorright == EMPTYFLAG && colorright1 == EMPTYFLAG &&
                colorright2 == mycolor && colorright3 == EMPTYFLAG && colorleft == EMPTYFLAG)
                return LOWALIVE2;

            //其余在下边返回没有威胁

        }
        return NOTHREAT;//返回没有威胁
}

int Robotway::judgeChessSituation(std::vector<std::vector<int>>dir){
    Situation situation = { 0 };//记录当前形势变量
    for (int i=0;i<4;i++) {//四个方向,0横，1竖，2左上右下，3右上左下
        int type;
        type = judge_score(dir[i]);//取得类型（死四，活四等）

        switch (type) {//根据类型对situation设置
            case WIN5:
                situation.win5++;
                break;
            case ALIVE4:
                situation.alive4++;
                break;
            case DIE4:
                situation.die4++;
                break;
            case LOWDIE4:
                situation.lowdie4++;
                break;
            case ALIVE3:
                situation.alive3++;
                break;
            case TIAO3:
                situation.tiao3++;
                break;
            case DIE3:
                situation.die3++;
                break;
            case ALIVE2:
                situation.alive2++;
                break;
            case LOWALIVE2:
                situation.lowalive2++;
                break;
            case DIE2:
                situation.die2++;
                break;
            case NOTHREAT:
                situation.nothreat++;
                break;
            default:
                //差错控制
                break;
        }
    }

    return giveScore(situation);//根据形势得出分数
}

int Robotway::giveScore(Situation situation) {

    int die4 = situation.die4 + situation.lowdie4;
    int alive3 = situation.alive3 + situation.tiao3;
    int alive2 = situation.alive2 + situation.lowalive2;

    if (situation.win5 >= 1)
        return LevelOne;//赢5

    if ((die4 >= 1 && alive3 >= 1)||situation.alive4 >= 1)
        return Leveltwo+1;//活4  死4活3

    if ( die4 >= 2)
        return Leveltwo;//双死4

    if (alive3 >= 2)
        return Levelthree;//双活3

    if (situation.die3 >= 1 && situation.alive3 >= 1)
        return Levelfour;//死3高级活3

    if (situation.die4 >= 1)
        return Levelfive;//高级死4

    if (situation.lowdie4 >= 1)
        return Levelsix;//低级死4

    if (situation.alive3 >= 1)
        return Levelseven;//单活3

    if (situation.tiao3 >= 1)
        return LevelEight;//跳活3

    if (alive2 >= 2)
        return LevelNight;//双活2

    if (situation.alive2 >= 1)
        return LevelTen;//活2

    if (situation.lowalive2 >= 1)
        return LevelEleven;//低级活2

    if (situation.die3 >= 1)
        return LevelTwelve;//死3

    if (situation.die2 >= 1)
        return LevelThirteen;//死2

    return LevelFourteen;//没有威胁
}
