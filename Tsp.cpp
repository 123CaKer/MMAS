#include "Tsp.h"
#include<random>
#include<ctime>
#include<QElapsedTimer>
#include<QCoreApplication>
#include<QDebug>
using UTM = std::uniform_real_distribution<double>;
CTsp::CTsp(int itCount, int antNum, double alpha, double beta, double rou, double tmin, double tmax)
    :Qmax(tmax)
    ,Qmin(tmin)
    ,m_it_count(itCount)
    ,m_ant_count(antNum)
    ,m_alpha(alpha)
    ,m_beta(beta)
    ,m_rou(rou)
    ,m_scene(nullptr)
    ,m_pathGroup(nullptr)
{
	
}

CTsp::~CTsp(void)
{
}

void CTsp::ResetData(int itCount, int antNum, double alpha, double beta, double rou, double tmin, double tmax)
{
    m_it_count = itCount;
    m_ant_count = antNum;
    m_alpha = alpha;
    m_beta = beta;
    m_rou  = 1.000000-rou;
    Qmin = tmin;
    Qmax = tmax;

    // 初始化蚁群
    initAnts();

    m_StepResult.clear();


    //初始化环境信息素，开始时，先把信息素设置成一个比较大的值
    //第一次迭代完成后，会变成Qmax(1)，然后以后就会限制在最大和最小值之间
    for (int i=0;i<N_CITY_COUNT;i++)
    {
        for (int j=0;j<N_CITY_COUNT;j++)
        {
            g_Pheromone[i][j]=10000.0;
            g_Prob[i][j] = pow(g_Pheromone[i][j], m_alpha)*g_DistanceBeta[i][j];
        }
    }

    //计算最大和最小信息素之间的比值
    double dbN=(double)N_CITY_COUNT;
    double dbTemp=exp(log(Pbest)/dbN); //对Pbest开N_CITY_COUNT次方
    m_dbRate=(2.0/dbTemp-2.0)/(dbN-2.0);

    //因为第一次迭代时，还没有全局最优解，所有计算不出最大和最小值，先设置成0.0
//    Qmax=0.0;
//    Qmin=0.0;
}


//初始化数据
void CTsp::InitData() 
{
    // 初始城市数据
    initCities();

    // 初始化蚁群
    initAnts();

    m_StepResult.clear();


	//初始化环境信息素，开始时，先把信息素设置成一个比较大的值
	//第一次迭代完成后，会变成Qmax(1)，然后以后就会限制在最大和最小值之间
	for (int i=0;i<N_CITY_COUNT;i++)
	{
		for (int j=0;j<N_CITY_COUNT;j++)
		{
			g_Pheromone[i][j]=10000.0;
            g_Prob[i][j] = pow(g_Pheromone[i][j], m_alpha)*g_DistanceBeta[i][j];
		}
	}

	//计算最大和最小信息素之间的比值
	double dbN=(double)N_CITY_COUNT;
    double dbTemp=exp(log(Pbest)/dbN); //对Pbest开N_CITY_COUNT次方
	m_dbRate=(2.0/dbTemp-2.0)/(dbN-2.0);

	//因为第一次迭代时，还没有全局最优解，所有计算不出最大和最小值，先设置成0.0
//	Qmax=0.0;
//	Qmin=0.0;

}


//更新环境信息素，只用当前最好蚂蚁去更新
void CTsp::UpdatePheromone(int nFlag)
{
    CAnt m_cTempAnt;
	if (nFlag == 1) //使用全局最优解
	{
		m_cTempAnt=m_cGlobalBestAnt;
	}
	else //使用迭代最优解
	{
		m_cTempAnt=m_cIterationBestAnt;
	}

	//临时保存信息素
	double dbTempAry[N_CITY_COUNT][N_CITY_COUNT];
	memset(dbTempAry,0,sizeof(dbTempAry)); //先全部设置为0

	//计算新增加的信息素,保存到临时数组里
	int m=0;
	int n=0;
	//计算目前最优蚂蚁留下的信息素
	double dbTemp=1.0/m_cTempAnt.m_dbPathLength;
	for (int j=1;j<N_CITY_COUNT;j++)
	{
		m=m_cTempAnt.m_nPath[j];
		n=m_cTempAnt.m_nPath[j-1];
		dbTempAry[n][m]=dbTempAry[n][m]+dbTemp;
		dbTempAry[m][n]=dbTempAry[n][m];
	}

	//最后城市和开始城市之间的信息素
	n=m_cTempAnt.m_nPath[0];
	dbTempAry[n][m]=dbTempAry[n][m]+dbTemp;
	dbTempAry[m][n]=dbTempAry[n][m];


	//==================================================================
	//更新环境信息素
	for (int i=0;i<N_CITY_COUNT;i++)
	{
		for (int j=0;j<N_CITY_COUNT;j++)
		{
            g_Pheromone[i][j]=g_Pheromone[i][j]*m_rou+dbTempAry[i][j];  //最新的环境信息素 = 留存的信息素 + 新留下的信息素
		}
	}

	//==================================================================
	//检查环境信息素，如果在最小和最大值的外面，则将其重新调整
    Qmax=1.0/(m_cGlobalBestAnt.m_dbPathLength*(1.0-m_rou));
	Qmin=Qmax*m_dbRate;

	for (int i=0;i<N_CITY_COUNT;i++)
	{
		for (int j=0;j<N_CITY_COUNT;j++)
		{
			if (g_Pheromone[i][j] < Qmin)
			{
				g_Pheromone[i][j] = Qmin;
			}

			if (g_Pheromone[i][j] > Qmax)
			{
				g_Pheromone[i][j] = Qmax;
			}

		}
	}

	//==================================================================
	//计算两两城市间的选择概率值
	for (int i=0;i<N_CITY_COUNT;i++)
	{
		for (int j=0;j<N_CITY_COUNT;j++)
		{
            g_Prob[i][j]=pow(g_Pheromone[i][j], m_alpha)*g_DistanceBeta[i][j];
		}
	}

}

void CTsp::setSence(QGraphicsScene *scene, QGraphicsItemGroup *group)
{
    m_scene = scene;
    m_pathGroup = group;
}

void CTsp::initCities()
{
    UTM u(10, 500);
    std::mt19937 gen((unsigned int)time(nullptr));
    m_cities.clear();
    for (int i = 0; i < N_CITY_COUNT; i++)
    {
        double x = u(gen), y = u(gen);
        City city(x, y, QString::number(i));
        m_cities.push_back(city);
    }

    //计算两两城市间距离
    for (int i=0;i<N_CITY_COUNT;i++)
    {
        for (int j=0;j<N_CITY_COUNT;j++)
        {

            double distance = CalCityDistance(m_cities[i],m_cities[j]);
            g_Distance[i][j]=ROUND(distance);
            g_DistanceBeta[i][j]=pow(1.0/g_Distance[i][j], m_beta);
        }
    }
}

void CTsp::initAnts()
{
    m_cAntAry.clear();
    if(m_cAntAry.empty())
    {
        qDebug()<<"ants have been clean";
    }

    for(int i = 0; i<m_ant_count;i++)
    {
        CAnt ant(i);
        m_cAntAry.push_back(ant);
    }
}

void CTsp::Search()
{

	m_cGlobalBestAnt.m_dbPathLength=DB_MAX;

    for (int i=0;i<m_it_count;i++)
	{
        bool should_update = false;
		m_cIterationBestAnt.m_dbPathLength=DB_MAX;

		//每只蚂蚁搜索一遍
        for (int j=0;j<m_ant_count;j++)
		{
			m_cAntAry[j].Search(); 
		}


		//保存最佳结果
        for (int j=0;j<m_ant_count;j++)
		{
			//保存搜索最优解
			if (m_cAntAry[j].m_dbPathLength < m_cGlobalBestAnt.m_dbPathLength)
			{
                m_cGlobalBestAnt=m_cAntAry[j];
                should_update = true;
			}

			//保存迭代最优解
			if (m_cAntAry[j].m_dbPathLength < m_cIterationBestAnt.m_dbPathLength)
			{
				m_cIterationBestAnt=m_cAntAry[j];
			}

		}

        // 更新统计数据
        if(should_update || i == m_it_count-1){
            m_StepResult.push_back(QPair<int, double>(i+1, m_cGlobalBestAnt.WalkLength()));
            if(m_scene && m_pathGroup){
                auto path = m_cGlobalBestAnt.Path();
                draw_path(m_pathGroup, m_cities, path);
                QElapsedTimer t;
                t.start();
                while(t.elapsed()<10)
                    QCoreApplication::processEvents();
            }
            should_update = false;
        }

		// 更新环境信息素，使用全局最优和迭代最优交替更新的策略
		// 每过5次迭代使用一次全局最优蚂蚁更新信息素
		// 这样可以扩大搜索范围
		if ((i+1) % 5 == 0)
		{
			UpdatePheromone(1);
		}
		else
		{
			UpdatePheromone(0);
		}
	}

}

