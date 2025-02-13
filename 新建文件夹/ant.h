#ifndef ANT_H
#define ANT_H
#include<QList>
#include "common.h"
class Tps;
class Ant
{
public:
    friend class Tps;
    Ant();
    ~Ant();
    Ant& operator=(Ant& ant);
    int   ChooseNextCity();
    void  Init();
    void  Move();
    void  Search();
    void  CalPathLength();
    QList<int> Path() const;
    double WalkLength() const
    {
        return m_walk_length;
    }
private:
    int m_city_num; // ��������
    int m_Path[N_CITY_NUM]; //�����߹���·��
    int m_never_been_to_cites[N_CITY_NUM];

    double m_walk_length;//�����߹���·������
    int m_cur_ctiy; //��ǰ���ڳ��б��
    int m_haveBeenToCitiesCount; // �Ѿ�ȥ���ĳ�������;
};

#endif // ANT_H
