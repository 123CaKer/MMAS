#ifndef TPS_H
#define TPS_H
#include "ant.h"
#include<QList>
#include "city.h"
#include "common.h"
#include <QPair>
class QGraphicsScene;
class Tps
{    
public:
    Tps(int itCount, int antCount, double tmax, double tmin, double rho, double alpha, double beta);
    Tps();
    ~Tps();
    void InitData();
    void Search();
    void UpdatePheromone(int flag);
    QList<City> Cities();
    void setSence(QGraphicsScene* s, QGraphicsItemGroup* group=nullptr);
    Ant* BestAnt() const{
        return m_bestAnt;
    }
    Ant* IterationBestAnt() const{
       return m_IterationBestAnt;
    }
    QList<QPair<int, double>> StepResult()const
    {
        return m_stepResult;
    }
private:
    int m_city_num; // 城市数量
    int m_it_count; // 迭代次数
    int m_ant_count; // 蚁群数量
    QList<City> m_cities;
    Ant* m_bestAnt;// 全局最优结果
    Ant* m_IterationBestAnt; //保存迭代最优结果
    QList<Ant> ants;//
    double m_tmax; //信息素上限
    double m_tmin;  //信息素下限
    double m_rho; // 挥发系数
    double m_alpha;
    double m_beta;
    double m_dbRate; //最大信息素和最小信息素的比值

    QGraphicsScene* m_scene;
    QGraphicsItemGroup* m_group;
    QList<QPair<int, double>> m_stepResult;
private:
    void initCities();
};

#endif // TPS_H
