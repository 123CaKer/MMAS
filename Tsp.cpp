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

    // ��ʼ����Ⱥ
    initAnts();

    m_StepResult.clear();


    //��ʼ��������Ϣ�أ���ʼʱ���Ȱ���Ϣ�����ó�һ���Ƚϴ��ֵ
    //��һ�ε�����ɺ󣬻���Qmax(1)��Ȼ���Ժ�ͻ�������������Сֵ֮��
    for (int i=0;i<N_CITY_COUNT;i++)
    {
        for (int j=0;j<N_CITY_COUNT;j++)
        {
            g_Pheromone[i][j]=10000.0;
            g_Prob[i][j] = pow(g_Pheromone[i][j], m_alpha)*g_DistanceBeta[i][j];
        }
    }

    //����������С��Ϣ��֮��ı�ֵ
    double dbN=(double)N_CITY_COUNT;
    double dbTemp=exp(log(Pbest)/dbN); //��Pbest��N_CITY_COUNT�η�
    m_dbRate=(2.0/dbTemp-2.0)/(dbN-2.0);

    //��Ϊ��һ�ε���ʱ����û��ȫ�����Ž⣬���м��㲻��������Сֵ�������ó�0.0
//    Qmax=0.0;
//    Qmin=0.0;
}


//��ʼ������
void CTsp::InitData() 
{
    // ��ʼ��������
    initCities();

    // ��ʼ����Ⱥ
    initAnts();

    m_StepResult.clear();


	//��ʼ��������Ϣ�أ���ʼʱ���Ȱ���Ϣ�����ó�һ���Ƚϴ��ֵ
	//��һ�ε�����ɺ󣬻���Qmax(1)��Ȼ���Ժ�ͻ�������������Сֵ֮��
	for (int i=0;i<N_CITY_COUNT;i++)
	{
		for (int j=0;j<N_CITY_COUNT;j++)
		{
			g_Pheromone[i][j]=10000.0;
            g_Prob[i][j] = pow(g_Pheromone[i][j], m_alpha)*g_DistanceBeta[i][j];
		}
	}

	//����������С��Ϣ��֮��ı�ֵ
	double dbN=(double)N_CITY_COUNT;
    double dbTemp=exp(log(Pbest)/dbN); //��Pbest��N_CITY_COUNT�η�
	m_dbRate=(2.0/dbTemp-2.0)/(dbN-2.0);

	//��Ϊ��һ�ε���ʱ����û��ȫ�����Ž⣬���м��㲻��������Сֵ�������ó�0.0
//	Qmax=0.0;
//	Qmin=0.0;

}


//���»�����Ϣ�أ�ֻ�õ�ǰ�������ȥ����
void CTsp::UpdatePheromone(int nFlag)
{
    CAnt m_cTempAnt;
	if (nFlag == 1) //ʹ��ȫ�����Ž�
	{
		m_cTempAnt=m_cGlobalBestAnt;
	}
	else //ʹ�õ������Ž�
	{
		m_cTempAnt=m_cIterationBestAnt;
	}

	//��ʱ������Ϣ��
	double dbTempAry[N_CITY_COUNT][N_CITY_COUNT];
	memset(dbTempAry,0,sizeof(dbTempAry)); //��ȫ������Ϊ0

	//���������ӵ���Ϣ��,���浽��ʱ������
	int m=0;
	int n=0;
	//����Ŀǰ�����������µ���Ϣ��
	double dbTemp=1.0/m_cTempAnt.m_dbPathLength;
	for (int j=1;j<N_CITY_COUNT;j++)
	{
		m=m_cTempAnt.m_nPath[j];
		n=m_cTempAnt.m_nPath[j-1];
		dbTempAry[n][m]=dbTempAry[n][m]+dbTemp;
		dbTempAry[m][n]=dbTempAry[n][m];
	}

	//�����кͿ�ʼ����֮�����Ϣ��
	n=m_cTempAnt.m_nPath[0];
	dbTempAry[n][m]=dbTempAry[n][m]+dbTemp;
	dbTempAry[m][n]=dbTempAry[n][m];


	//==================================================================
	//���»�����Ϣ��
	for (int i=0;i<N_CITY_COUNT;i++)
	{
		for (int j=0;j<N_CITY_COUNT;j++)
		{
            g_Pheromone[i][j]=g_Pheromone[i][j]*m_rou+dbTempAry[i][j];  //���µĻ�����Ϣ�� = �������Ϣ�� + �����µ���Ϣ��
		}
	}

	//==================================================================
	//��黷����Ϣ�أ��������С�����ֵ�����棬�������µ���
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
	//�����������м��ѡ�����ֵ
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

    //�����������м����
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

		//ÿֻ��������һ��
        for (int j=0;j<m_ant_count;j++)
		{
			m_cAntAry[j].Search(); 
		}


		//������ѽ��
        for (int j=0;j<m_ant_count;j++)
		{
			//�����������Ž�
			if (m_cAntAry[j].m_dbPathLength < m_cGlobalBestAnt.m_dbPathLength)
			{
                m_cGlobalBestAnt=m_cAntAry[j];
                should_update = true;
			}

			//����������Ž�
			if (m_cAntAry[j].m_dbPathLength < m_cIterationBestAnt.m_dbPathLength)
			{
				m_cIterationBestAnt=m_cAntAry[j];
			}

		}

        // ����ͳ������
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

