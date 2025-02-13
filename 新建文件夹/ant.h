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
    int m_city_num; // 城市数量
    int m_Path[N_CITY_NUM]; //蚂蚁走过的路径
    int m_never_been_to_cites[N_CITY_NUM];

    double m_walk_length;//蚂蚁走过的路径长度
    int m_cur_ctiy; //当前所在城市编号
    int m_haveBeenToCitiesCount; // 已经去过的城市数量;
};

#endif // ANT_H
