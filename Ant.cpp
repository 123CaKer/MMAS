 #include "Ant.h"

CAnt::CAnt()
    :m_id(-1)
{

}

CAnt::CAnt(int id)
    :m_id(id)
{
}

CAnt::~CAnt(void)
{
}


void CAnt::Init()
{

	for (int i=0;i<N_CITY_COUNT;i++)
	{
		m_nAllowedCity[i]=1; //设置全部城市为没有去过
		m_nPath[i]=0; //蚂蚁走的路径全部设置为0
	}

	//蚂蚁走过的路径长度设置为0
	m_dbPathLength=0.0; 

	//随机选择一个出发城市
	m_nCurCityNo=rnd(0,N_CITY_COUNT);

	//设置出发城市
	m_nPath[0]=m_nCurCityNo;

	//标识出发城市为已经去过了
	m_nAllowedCity[m_nCurCityNo]=0; 


	//已经去过的城市数量设置为1
	m_nMovedCityCount=1; 


}


//选择下一个城市
//返回值 为城市编号
int CAnt::ChooseNextCity()
{

	int nSelectedCity=-1; //返回结果，先暂时把其设置为-1

	//==============================================================================
	//计算当前城市和没去过的城市之间的信息素总和
	double dbTotal=0.0;	
    double  prob[N_CITY_COUNT]; // 保存城市被选中的概率

	for (int i=0;i<N_CITY_COUNT;i++)
	{
		if (m_nAllowedCity[i] == 1) //城市没去过
		{
//			prob[i]=pow(g_Pheromone[m_nCurCityNo][i],ALPHA)*pow(1.0/g_Distance[m_nCurCityNo][i],BETA); //该城市和当前城市间的信息素
			prob[i]=g_Prob[m_nCurCityNo][i];
			dbTotal=dbTotal+prob[i]; //累加信息素，得到总和
		}
		else
		{
			prob[i]=0.0;
		}
	}

	//==============================================================================
	//轮盘选择
	double dbTemp=0.0;
	if (dbTotal > 0.0) //总的信息素值大于0
	{
		dbTemp=rnd(0.0,dbTotal); //取一个随机数

		for (int i=0;i<N_CITY_COUNT;i++)
		{
			if (m_nAllowedCity[i] == 1) //城市没去过
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

	//==============================================================================
	//如果城市间的信息素非常小 ( 小到比double能够表示的最小的数字还要小 )
	//那么由于浮点运算的误差原因，上面计算的概率总和可能为0
	//会出现经过上述操作，没有城市被选择出来
	//出现这种情况，就把第一个没去过的城市作为返回结果
	if (nSelectedCity == -1)
	{
		for (int i=0;i<N_CITY_COUNT;i++)
		{
			if (m_nAllowedCity[i] == 1) //城市没去过
			{
				nSelectedCity=i;
				break;
			}
		}
	}

	//==============================================================================
	//返回结果
	return nSelectedCity;
}

void CAnt::Move()
{
	int nCityNo=ChooseNextCity(); //选择下一个城市

	m_nPath[m_nMovedCityCount]=nCityNo; //蚂蚁走的路径
	m_nAllowedCity[nCityNo]=0;//没去过的城市
	m_nCurCityNo=nCityNo; //当前所在城市编号
	m_nMovedCityCount++; //已经去过的城市数量
}

//搜索一次
void CAnt::Search()
{
	Init();

	while (m_nMovedCityCount < N_CITY_COUNT)
	{
		Move();
	}

	CalPathLength();
}


//计算蚂蚁走过的路径长度
void CAnt::CalPathLength()
{
	m_dbPathLength=0.0;
	int m=0;
	int n=0;
	for (int i=1;i<N_CITY_COUNT;i++)
	{
		m=m_nPath[i];
		n=m_nPath[i-1];
		m_dbPathLength=m_dbPathLength+g_Distance[m][n];
	}

	//加上从最后城市返回出发城市的距离
	n=m_nPath[0];
	m_dbPathLength=m_dbPathLength+g_Distance[m][n];	

}

QList<int> CAnt::Path() const
{
    QList<int> path;
    for(int i = 0; i<N_CITY_COUNT;i++)
    {
        path.push_back(m_nPath[i]);
    }
    return path;
}
