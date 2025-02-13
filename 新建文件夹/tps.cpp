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
    //�����������м����

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

    //��ʼ��������Ϣ�أ���ʼʱ���Ȱ���Ϣ�����ó�һ���Ƚϴ��ֵ
    //��һ�ε�����ɺ󣬻���Qmax(1)��Ȼ���Ժ�ͻ�������������Сֵ֮��
    for (int i=0; i<N_CITY_NUM; i++)
    {
        for (int j=0; j<N_CITY_NUM; j++)
        {
            g_Pheromone[i][j]=10000.0;
            g_Prob[i][j]=pow(g_Pheromone[i][j], m_alpha) * g_DistanceBeta[i][j];
        }
    }


    double dbTemp=0.0;
    //����������С��Ϣ��֮��ı�ֵ
    dbTemp=exp(log(Pbest)/N_CITY_NUM); //��Pbest��N_CITY_COUNT�η�
    m_dbRate=(2.0/dbTemp-2.0)/(N_CITY_NUM-2.0);

    //��Ϊ��һ�ε���ʱ����û��ȫ�����Ž⣬���м��㲻��������Сֵ�������ó�0.0
//    m_tmax=0.0;
//    m_tmin=0.0;
    m_bestAnt = new Ant;
    m_IterationBestAnt = new Ant;

    // ��ʼ����Ⱥ
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
        //ÿֻ��������һ��
        for (int j=0; j< m_ant_count; j++)
        {
            ants[j].Search();
        }


        //������ѽ��
        for (int j=0; j<m_ant_count; j++)
        {
            //�����������Ž�
            if (ants[j].m_walk_length < m_bestAnt->m_walk_length)
            {
                *m_bestAnt = ants[j];
                should_update = true;
            }

            //����������Ž�
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
        // ���»�����Ϣ�أ�ʹ��ȫ�����ź͵������Ž�����µĲ���
        // ÿ��5�ε���ʹ��һ��ȫ���������ϸ�����Ϣ��
        // ������������������Χ
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
    if (flag == 1) //ʹ��ȫ�����Ž�
    {
        m_cTempAnt = m_bestAnt;
    }
    else //ʹ�õ������Ž�
    {
        m_cTempAnt = m_IterationBestAnt;
    }

    //��ʱ������Ϣ��
    double dbTempAry[N_CITY_NUM][N_CITY_NUM];
    memset(dbTempAry, 0, sizeof(dbTempAry)); //��ȫ������Ϊ0

    //���������ӵ���Ϣ��,���浽��ʱ������
    int m=0;
    int n=0;
    //����Ŀǰ�����������µ���Ϣ��
    double dbTemp=1.0/m_cTempAnt->m_walk_length;
    for (int j=1; j<N_CITY_NUM;j++)
    {
        m = m_cTempAnt->m_Path[j];
        n = m_cTempAnt->m_Path[j-1];
        dbTempAry[n][m] = dbTempAry[n][m]+dbTemp;
        dbTempAry[m][n] = dbTempAry[n][m];
    }

    //�����кͿ�ʼ����֮�����Ϣ��
    n = m_cTempAnt->m_Path[0];
    dbTempAry[n][m]=dbTempAry[n][m]+dbTemp;
    dbTempAry[m][n]=dbTempAry[n][m];


    //==================================================================
    //���»�����Ϣ��
    for (int i=0;i<N_CITY_NUM;i++)
    {
        for (int j=0;j<N_CITY_NUM;j++)
        {
            g_Pheromone[i][j]=g_Pheromone[i][j] * m_rho + dbTempAry[i][j];  //���µĻ�����Ϣ�� = �������Ϣ�� + �����µ���Ϣ��
        }
    }

    //==================================================================
    //��黷����Ϣ�أ��������С�����ֵ�����棬�������µ���
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
    //�����������м��ѡ�����ֵ
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
