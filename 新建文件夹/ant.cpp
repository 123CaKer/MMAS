#include "ant.h"
#include<QDebug>
Ant::Ant()
    :m_city_num(N_CITY_NUM)
    ,m_walk_length(0.0)
{
    Init();
}

Ant::~Ant()
{

}

Ant &Ant::operator=(Ant &ant)
{
    this->m_city_num = ant.m_city_num;
    this->m_walk_length = ant.m_walk_length;
    this->m_cur_ctiy = ant.m_cur_ctiy;
    this->m_haveBeenToCitiesCount = ant.m_haveBeenToCitiesCount;
    memcpy(this->m_never_been_to_cites, ant.m_never_been_to_cites, sizeof (ant.m_never_been_to_cites));
    memcpy(this->m_Path, ant.m_Path, sizeof(ant.m_Path));
    return *this;
}

int Ant::ChooseNextCity()
{
    int nSelectedCity=-1; //返回结果，先暂时把其设置为-1
    double dbTotal = 0.0;
    double prob[N_CITY_NUM]; // 保存城市被选中的概率
    for(int i = 0; i<N_CITY_NUM; i++)
    {
        if(m_never_been_to_cites[i] == 1)
        {
            prob[i] = g_Prob[m_cur_ctiy][i];
            dbTotal += prob[i];
        }else{
            prob[i] = 0.0;
        }
    }

    double dbTemp = 0.0;
    if(dbTotal > 0.0)
    {
        dbTemp=rnd(0.0,dbTotal); //取一个随机数

        for (int i=0; i<N_CITY_NUM; i++)
        {
            if (m_never_been_to_cites[i] == 1) //城市没去过
            {
                dbTemp=dbTemp-prob[i]; //这个操作相当于转动轮盘，如果对轮盘选择不熟悉，仔细考虑一下
                if (dbTemp < 0.0) //轮盘停止转动，记下城市编号，直接跳出循环
                {
                    nSelectedCity=i;
                    break;
                }
            }
        }
    }

    if (nSelectedCity == -1)
    {
        for (int i=0; i<N_CITY_NUM; i++)
        {
            if (m_never_been_to_cites[i] == 1) //城市没去过
            {
                nSelectedCity=i;
                break;
            }
        }
    }
    return nSelectedCity;
}

void Ant::Init()
{
    for(int i=0; i<N_CITY_NUM; i++)
    {
        m_Path[i] = 0;
        m_never_been_to_cites[i] = 1;
    }
    m_walk_length = 0.0;
    m_cur_ctiy = rnd(0, N_CITY_NUM);
    m_Path[0] = m_cur_ctiy;
    m_never_been_to_cites[m_cur_ctiy] = 0;
    m_haveBeenToCitiesCount = 1;
}

void Ant::Move()
{
    int city_no = ChooseNextCity();
    m_Path[m_haveBeenToCitiesCount]=city_no; //蚂蚁走的路径
    m_never_been_to_cites[city_no]=0;//没去过的城市
    m_cur_ctiy=city_no; //当前所在城市编号
    m_haveBeenToCitiesCount++; //已经去过的城市数量
}

void Ant::Search()
{
    Init();
    while(m_haveBeenToCitiesCount < N_CITY_NUM)
    {
        Move();
    }
    CalPathLength();
}

void Ant::CalPathLength()
{
    m_walk_length = 0.0;
    int m = 0, n = 0;
    for(int i = 1; i<N_CITY_NUM; i++)
    {
        m = m_Path[i];
        n = m_Path[i-1];
        m_walk_length += g_Distance[m][n];
    }

    n = m_Path[0];
    m_walk_length += g_Distance[m][n];
}

QList<int> Ant::Path() const
{
    QList<int> path;
    for(int i=0;i<m_city_num;i++)
    {
        path.push_back(m_Path[i]);
    }
    return path;
}
