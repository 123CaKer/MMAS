#ifndef COMMON_H
#define COMMON_H
#include <cstdlib>
#include<limits>
#include <QGraphicsItemGroup>
#include<QList>
#include "city.h"
const int N_CITY_NUM = 50;
extern double g_Pheromone[N_CITY_NUM][N_CITY_NUM]; //两两城市间信息素
extern double g_Distance[N_CITY_NUM][N_CITY_NUM]; //两两城市间距离
extern double g_Prob[N_CITY_NUM][N_CITY_NUM];
extern double g_DistanceBeta[N_CITY_NUM][N_CITY_NUM];
const double  Pbest=0.05; //蚂蚁一次搜索找到最优路径的概率
const double  DB_MAX = std::numeric_limits<double>::max();
static int rnd(int nLow, int nUpper)
{
     return nLow+(nUpper-nLow)*rand()/(RAND_MAX+1);
}

static double ROUND(double dbA)
{
    return (double)((int)(dbA+0.5));
}

static void draw_path(QGraphicsItemGroup* pathGroup, QList<City>& cities, QList<int>& path)
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
#endif // COMMON_H
