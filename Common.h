#pragma once
#include<limits>
#include <QGraphicsItemGroup>
#include<QList>
#include "city.h"
#define STR_CONV(s) QString::fromLocal8Bit(s)
const int N_CITY_COUNT=50; //城市数量
const double Pbest=0.05; //蚂蚁一次搜索找到最优路径的概率
const double DB_MAX=std::numeric_limits<double>::max(); //一个标志数

extern double g_Pheromone[N_CITY_COUNT][N_CITY_COUNT]; //两两城市间信息素
extern double g_Distance[N_CITY_COUNT][N_CITY_COUNT]; //两两城市间距离

extern double g_Prob[N_CITY_COUNT][N_CITY_COUNT];
extern double g_DistanceBeta[N_CITY_COUNT][N_CITY_COUNT];

extern int rnd(int nLow,int nUpper); //返回随机数
extern double rnd(double dbLow,double dbUpper);
extern double ROUND(double dbA);

static void draw_path(QGraphicsItemGroup* pathGroup, const QList<City>& cities, QList<int>& path)
{
    auto start  = cities[path[0]];
    auto last   = start;

    foreach(auto line, pathGroup->childItems())
    {
        pathGroup->removeFromGroup(line);
        delete line;
    }

    for(int i = 1; i < path.size(); i++)
    {
        auto cur = cities[path[i]];
        QGraphicsLineItem *line = new QGraphicsLineItem(last.X(), last.Y(), cur.X(), cur.Y());
        pathGroup->addToGroup(line);
        last = cur;
    }
    QGraphicsLineItem *line = new QGraphicsLineItem(last.X(), last.Y(), start.X(), start.Y());
    pathGroup->addToGroup(line);
}

