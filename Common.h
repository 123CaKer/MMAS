#pragma once
#include<limits>
#include <QGraphicsItemGroup>
#include<QList>
#include "city.h"
#define STR_CONV(s) QString::fromLocal8Bit(s)
const int N_CITY_COUNT=50; //��������
const double Pbest=0.05; //����һ�������ҵ�����·���ĸ���
const double DB_MAX=std::numeric_limits<double>::max(); //һ����־��

extern double g_Pheromone[N_CITY_COUNT][N_CITY_COUNT]; //�������м���Ϣ��
extern double g_Distance[N_CITY_COUNT][N_CITY_COUNT]; //�������м����

extern double g_Prob[N_CITY_COUNT][N_CITY_COUNT];
extern double g_DistanceBeta[N_CITY_COUNT][N_CITY_COUNT];

extern int rnd(int nLow,int nUpper); //���������
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

