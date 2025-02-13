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
	CAnt m_cGlobalBestAnt; //保存全局最优结果
	CAnt m_cIterationBestAnt; //保存迭代最优结果

	double Qmax; //信息素上限
	double Qmin;  //信息素下限

public:
    // 重置参数
    void ResetData(int itCount, int antNum, double alpha, double beta, double rou, double tmin, double tmax);

	//初始化数据
	void InitData(); 

	//开始搜索
	void Search(); 

	//更新环境信息素
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
    int m_it_count; // 迭代次数
    int m_ant_count; // 蚁群数量
    double m_alpha;
    double m_beta;
    double m_rou; //信息素残留系数，(1-ROU)就是信息素的挥发速度
    double m_dbRate; //最大信息素和最小信息素的比值
    QGraphicsScene *m_scene;
    QGraphicsItemGroup* m_pathGroup;
    QList<QPair<int, double>> m_StepResult;
    QList<City> m_cities;
};
