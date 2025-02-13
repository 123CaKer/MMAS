#pragma once
#include "Common.h"
#include "Ant.h"
#include "city.h"
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QList>
#include <QPair>
#include <vector>
class CTsp
{
public:
    CTsp(int itCount, int antNum, double alpha, double beta, double rou, double tmin, double tmax);
	~CTsp(void);

public:
    std::vector<CAnt> m_cAntAry;
	CAnt m_cGlobalBestAnt; //����ȫ�����Ž��
	CAnt m_cIterationBestAnt; //����������Ž��

	double Qmax; //��Ϣ������
	double Qmin;  //��Ϣ������

public:
    // ���ò���
    void ResetData(int itCount, int antNum, double alpha, double beta, double rou, double tmin, double tmax);

	//��ʼ������
	void InitData(); 

	//��ʼ����
	void Search(); 

	//���»�����Ϣ��
	void UpdatePheromone(int nFlag);
    CAnt BestAnt() const
    {
        return m_cGlobalBestAnt;
    }

    void setSence(QGraphicsScene *scene, QGraphicsItemGroup* group);
    QList<QPair<int, double>> StepResult() const
    {
        return m_StepResult;
    }
    QList<City> Cities() const
    {
        return m_cities;
    }
private:
    void initCities();
    void initAnts();
private:
    int m_it_count; // ��������
    int m_ant_count; // ��Ⱥ����
    double m_alpha;
    double m_beta;
    double m_rou; //��Ϣ�ز���ϵ����(1-ROU)������Ϣ�صĻӷ��ٶ�
    double m_dbRate; //�����Ϣ�غ���С��Ϣ�صı�ֵ
    QGraphicsScene *m_scene;
    QGraphicsItemGroup* m_pathGroup;
    QList<QPair<int, double>> m_StepResult;
    QList<City> m_cities;
};
