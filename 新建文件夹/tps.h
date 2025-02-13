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
    int m_city_num; // ��������
    int m_it_count; // ��������
    int m_ant_count; // ��Ⱥ����
    QList<City> m_cities;
    Ant* m_bestAnt;// ȫ�����Ž��
    Ant* m_IterationBestAnt; //����������Ž��
    QList<Ant> ants;//
    double m_tmax; //��Ϣ������
    double m_tmin;  //��Ϣ������
    double m_rho; // �ӷ�ϵ��
    double m_alpha;
    double m_beta;
    double m_dbRate; //�����Ϣ�غ���С��Ϣ�صı�ֵ

    QGraphicsScene* m_scene;
    QGraphicsItemGroup* m_group;
    QList<QPair<int, double>> m_stepResult;
private:
    void initCities();
};

#endif // TPS_H
