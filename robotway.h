#ifndef ROBOTWAY_H
#define ROBOTWAY_H

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


class Robotway : public QMainWindow
{
    Q_OBJECT

public:
    Robotway(QWidget *parent = nullptr);
    ~Robotway();
    struct Situation {//当前位置的形式，打分根据这个来打
        int win5;//5连珠
        int alive4;//活4
        int die4;//死4
        int lowdie4;//死4低级版本
        int alive3;//活3
        int tiao3;//跳3
        int die3;//死3
        int alive2;//活2
        int lowalive2;//低级活2
        int die2;//死2
        int nothreat;//没有威胁
    };

    struct Position{
        int x;//0到14，行
        int y;//0到14，列
    };
    //棋局类型
    static const int WIN5 = 0;//0->5连珠
    static const int ALIVE4 = 1;//1->活4
    static const int DIE4 = 2;//2->死4
    static const int LOWDIE4 = 3;//3->死4的低级版本
    static const int ALIVE3 = 4;//3->活3
    static const int TIAO3 = 5;//5->跳3
    static const int DIE3 = 6;//6->死3
    static const int ALIVE2 = 7;//7->活2
    static const int LOWALIVE2 = 8;//8->低级活2
    static const int DIE2 = 9;//9->死2
    static const int NOTHREAT = 10;//10->没有威胁

    //打分等级
    static const int LevelOne = 100000;//成五
    static const int Leveltwo = 10000;//成活4 或 双死4 或 死4活3
    static const int Levelthree = 5000;//双活3
    static const int Levelfour = 1000;//死3高级活3
    static const int Levelfive = 500;//死四
    static const int Levelsix = 400;//低级死四
    static const int Levelseven = 100;//单活3
    static const int LevelEight = 90;//跳活3
    static const int LevelNight = 50;//双活2
    static const int LevelTen = 10;//活2
    static const int LevelEleven = 9;//低级活2
    static const int LevelTwelve = 5;//死3
    static const int LevelThirteen = 2;//死2
    static const int LevelFourteen = 1;//没有威胁
    static const int LevelFiveteen = 0;//不能下
private:
    Ui::MainWindow *ui;
    const int INF = 0x3f3f3f3f;
public:
    static int judge_score(std::vector<int>chess);
    static int giveScore(Situation situation);
    static int judgeChessSituation(std::vector<std::vector<int>>dir);
};
#endif // ROBOTWAY_H
