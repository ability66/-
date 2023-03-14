#ifndef ROBOTFIRST_H
#define ROBOTFIRST_H
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


class RobotFirst : public QMainWindow
{
    Q_OBJECT

public:
    RobotFirst(QWidget *parent = nullptr);
    ~RobotFirst();

private:
    Ui::MainWindow *ui;
    const int INF = 0x3f3f3f3f;
    const long long WIN5=10000000;
    const long long LIVE4=100000;
    const long long DIE4=0;
    const long long DIE3=0;
    const long long DIE2=0;
    const long long ASIDE4=5000;
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
    int gameModel;//0:人人 1:人机 人执黑 2:人机 人执白
    std::map<std::pair<int,int>,int>search_set;
    std::vector<std::vector<int>>chess_board;//0无子 1黑子 2白子
    QVector<QString> pos_ai; //ai坐标记录
    QVector<QString> pos_player;//玩家坐标记录
   std::stack<std::pair<int,int>>step;
    struct value {
        long long atk_val;//进攻分数
        long long def_val;//防守分数
        bool operator<(const value& o) const {
            if(def_val!=o.def_val) return def_val<o.def_val;
            return atk_val<o.atk_val;
        }//重载比较方法
        bool operator<=(const value& o) const {
            if(def_val!=o.def_val) return def_val<=o.def_val;
            return atk_val<=o.atk_val;
        }//重载比较方法
        value operator+(value& o) {
            value res;
            res.atk_val = atk_val + o.atk_val / 5;
            res.def_val = def_val + o.def_val / 5;
            return res;
        }//重载加法（下一层+这一层，下一层权值较小）
    };
    bool ifcut;
    value score;
    int moveX, moveY, lastx, lasty;
    int sum_step;
public:
    void initGame();
private:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    int judge_score(std::vector<int>chess);
    std::vector<int> main_diagonal(int x, int y, int type);
    std::vector<int> counter_diagonal(int x, int y, int type);
    std::vector<int> row(int x, int y, int type);
    std::vector<int> column(int x, int y, int type);
    int atk_val(int i, int j, int type);
    int def_val(int i, int j, int type);
    void playByAI();
    void add_search_set(int x,int y);
    void cal_now_score(int x,int y,int player);
    value cal_score(value nxt, value now);
    std::pair<value, std::pair<int, int>> bfs(int type, int step,value now_score,value alpha,value beta);
    bool judge_winner(int x, int y, int type);
    std::pair<value, std::pair<int, int>> cal_player(int type, std::pair<value, std::pair<int, int>>nxt);
    int judge_pre_forbiden(int x, int y, int type);
    bool judge_out_of_board(int x, int y);

    void Find_point(int *pp, int *qq);//找到最佳性最优的下子位置
    void Grade();//计算所有位置的得分
    void Base(int i, int j);//坐标为（i-4,j-4）的所有五连珠情况
    int Assessment(int num1, int num2);//评分标准
    int  count[23][23];//数据棋盘，用于储存下子位置和盘外数据
    int sum[23][23];//得分棋盘，储存得分情况
    int  p[20][5];//储存该位置的所有可能的五连珠情况

    void sendSlot();
    void defeat();
    void try_draw();
    void try_regret();
    void show_rule();
    QPushButton * bt,* rule,* stopgame,* draw,* regret;
    QSound *sound;
signals:
    void closed();
};
#endif // ROBOTFIRST_H
