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
    int nSelectedCity=-1; //���ؽ��������ʱ��������Ϊ-1
    double dbTotal = 0.0;
    double prob[N_CITY_NUM]; // ������б�ѡ�еĸ���
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
        dbTemp=rnd(0.0,dbTotal); //ȡһ�������

        for (int i=0; i<N_CITY_NUM; i++)
        {
            if (m_never_been_to_cites[i] == 1) //����ûȥ��
            {
                dbTemp=dbTemp-prob[i]; //��������൱��ת�����̣����������ѡ����Ϥ����ϸ����һ��
                if (dbTemp < 0.0) //����ֹͣת�������³��б�ţ�ֱ������ѭ��
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
            if (m_never_been_to_cites[i] == 1) //����ûȥ��
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
    m_Path[m_haveBeenToCitiesCount]=city_no; //�����ߵ�·��
    m_never_been_to_cites[city_no]=0;//ûȥ���ĳ���
    m_cur_ctiy=city_no; //��ǰ���ڳ��б��
    m_haveBeenToCitiesCount++; //�Ѿ�ȥ���ĳ�������
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
