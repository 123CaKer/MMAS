#include "tps.h"
#include<random>
#include<QDebug>
#include<QGraphicsScene>
#include<QElapsedTimer>
#include<QCoreApplication>
const double min_x = 10.0;
const double max_x = 500.0;
const double min_y = 10.0;
const double max_y = 500.0;
using UNIFORM_DIS=std::uniform_real_distribution<double>;
Tps::Tps(int itCount, int antCount, double tmax, double tmin, double rho, double alpha, double beta)
    :m_city_num(N_CITY_NUM)
    ,m_it_count(itCount)
    ,m_ant_count(antCount)
    ,m_tmax(tmax)
    ,m_tmin(tmin)
    ,m_rho(rho)
    ,m_alpha(alpha)
    ,m_beta(beta)
    ,m_scene(nullptr)
{

}

Tps::Tps()
{

}

Tps::~Tps()
{

}

void Tps::InitData()
{
    initCities();
    //计算两两城市间距离

    for (int i=0; i<N_CITY_NUM; i++)
    {
        for (int j=0; j<N_CITY_NUM; j++)
        {
            double disance = 0.0;
            if(i != j){
                disance = CalCityDistance(m_cities[i], m_cities[j]);
                disance = ROUND(disance);
            }
            g_Distance[i][j] = disance;

            g_DistanceBeta[i][j]=pow(1.0/g_Distance[i][j], m_beta);
        }
    }

    //初始化环境信息素，开始时，先把信息素设置成一个比较大的值
    //第一次迭代完成后，会变成Qmax(1)，然后以后就会限制在最大和最小值之间
    for (int i=0; i<N_CITY_NUM; i++)
    {
        for (int j=0; j<N_CITY_NUM; j++)
        {
            g_Pheromone[i][j]=10000.0;
            g_Prob[i][j]=pow(g_Pheromone[i][j], m_alpha) * g_DistanceBeta[i][j];
        }
    }


    double dbTemp=0.0;
    //计算最大和最小信息素之间的比值
    dbTemp=exp(log(Pbest)/N_CITY_NUM); //对Pbest开N_CITY_COUNT次方
    m_dbRate=(2.0/dbTemp-2.0)/(N_CITY_NUM-2.0);

    //因为第一次迭代时，还没有全局最优解，所有计算不出最大和最小值，先设置成0.0
//    m_tmax=0.0;
//    m_tmin=0.0;
    m_bestAnt = new Ant;
    m_IterationBestAnt = new Ant;

    // 初始化蚁群
    for(int i = 0; i<m_ant_count; i++)
    {
        Ant ant;
        ants.push_back(ant);
    }
}

void Tps::Search()
{
    m_bestAnt->m_walk_length = DB_MAX;
    m_stepResult.clear();
    for (int i=0; i< m_it_count; i++)
    {
        m_IterationBestAnt->m_walk_length = DB_MAX;
        bool should_update = false;
        //每只蚂蚁搜索一遍
        for (int j=0; j< m_ant_count; j++)
        {
            ants[j].Search();
        }


        //保存最佳结果
        for (int j=0; j<m_ant_count; j++)
        {
            //保存搜索最优解
            if (ants[j].m_walk_length < m_bestAnt->m_walk_length)
            {
                *m_bestAnt = ants[j];
                should_update = true;
            }

            //保存迭代最优解
            if (ants[j].m_walk_length < m_IterationBestAnt->m_walk_length)
            {
                *m_IterationBestAnt = ants[j];

            }

        }

        if(should_update){
            m_stepResult.push_back(QPair<int, double>(i, m_bestAnt->m_walk_length));
            if(m_scene && m_group){
                qDebug()<<"draw path by step:"<<m_bestAnt->m_walk_length;
                auto path = m_bestAnt->Path();
                draw_path(m_group, m_cities, path);
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

void Tps::UpdatePheromone(int flag)
{
    Ant*  m_cTempAnt;
    if (flag == 1) //使用全局最优解
    {
        m_cTempAnt = m_bestAnt;
    }
    else //使用迭代最优解
    {
        m_cTempAnt = m_IterationBestAnt;
    }

    //临时保存信息素
    double dbTempAry[N_CITY_NUM][N_CITY_NUM];
    memset(dbTempAry, 0, sizeof(dbTempAry)); //先全部设置为0

    //计算新增加的信息素,保存到临时数组里
    int m=0;
    int n=0;
    //计算目前最优蚂蚁留下的信息素
    double dbTemp=1.0/m_cTempAnt->m_walk_length;
    for (int j=1; j<N_CITY_NUM;j++)
    {
        m = m_cTempAnt->m_Path[j];
        n = m_cTempAnt->m_Path[j-1];
        dbTempAry[n][m] = dbTempAry[n][m]+dbTemp;
        dbTempAry[m][n] = dbTempAry[n][m];
    }

    //最后城市和开始城市之间的信息素
    n = m_cTempAnt->m_Path[0];
    dbTempAry[n][m]=dbTempAry[n][m]+dbTemp;
    dbTempAry[m][n]=dbTempAry[n][m];


    //==================================================================
    //更新环境信息素
    for (int i=0;i<N_CITY_NUM;i++)
    {
        for (int j=0;j<N_CITY_NUM;j++)
        {
            g_Pheromone[i][j]=g_Pheromone[i][j] * m_rho + dbTempAry[i][j];  //最新的环境信息素 = 留存的信息素 + 新留下的信息素
        }
    }

    //==================================================================
    //检查环境信息素，如果在最小和最大值的外面，则将其重新调整
    m_tmax = 1.0/(m_bestAnt->m_walk_length*(1-m_rho));
    m_tmin = m_tmax * m_dbRate;

    for (int i=0;i<N_CITY_NUM;i++)
    {
        for (int j=0;j<N_CITY_NUM;j++)
        {
            if (g_Pheromone[i][j] < m_tmin)
            {
                g_Pheromone[i][j] = m_tmin;
            }

            if (g_Pheromone[i][j] > m_tmax)
            {
                g_Pheromone[i][j] = m_tmax;
            }

        }
    }

    //==================================================================
    //计算两两城市间的选择概率值
    for (int i=0;i<N_CITY_NUM;i++)
    {
        for (int j=0;j<N_CITY_NUM;j++)
        {
            g_Prob[i][j]=pow(g_Pheromone[i][j],m_alpha)*g_DistanceBeta[i][j];
        }
    }
}

QList<City> Tps::Cities()
{
    return m_cities;
}

void Tps::setSence(QGraphicsScene* s,  QGraphicsItemGroup* group)
{
    m_scene = s;
    m_group = group;
}

void Tps::initCities()
{
    UNIFORM_DIS disX(min_x, max_x);
    UNIFORM_DIS disY(min_y, max_y);
    std::mt19937 gen((unsigned int)time(nullptr));
    for(int i=0;i<m_city_num; ++i)
    {
        double x = disX(gen);
        double y = disY(gen);

        City city(x, y, QString::number(i+1));
        m_cities.push_back(city) ;
    }
}
