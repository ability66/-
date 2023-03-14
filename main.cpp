#include "mainmenu.h"
#include <QApplication>
#include <QSound>

int main(int argc, char *argv[])
{
    QSound* sound = new QSound(":/222/sound/background.wav");
    sound ->play();//播放函数
    sound->setLoops(1000);//设置循环次数
    QApplication a(argc, argv);
    mainmenu w;
    w.show();

    return a.exec();
}
