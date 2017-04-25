# Maze
ds-project
>利用不相交集生成迷宫算法以及利用DFS寻找迷宫路径


###实验一
####1.实验名称：迷宫
####2.需求和规格说明：
#####2.1问题要求
题目要求使用 Union-Find 数据结构完成一个迷宫的生成。
1.输入：
&nbsp;&nbsp;&nbsp;输入两个整数，代表所生成迷宫对应矩阵的大小
2.输出：
&nbsp;&nbsp;&nbsp;1）用合理的方式展现生成的迷宫；
&nbsp;&nbsp;&nbsp;2）给出迷宫可以走通的路径。以 SEN…（代表向南，然后向东，然后再向北，等等）
的形式给出输出结果。
#####2.2问题分析
1.采用Union-find 不相交集来完成迷宫的生成。
2.需要采用适当数据结构存储迷宫及其路径，并将其表现出来。
####3.设计
#####3.1设计思想
主要思想：
要生成的迷宫可以看做一个$width * height$的节点矩阵，矩阵的相邻节点之间有边（无墙）或无边（被墙阻挡），这样的一个节点矩阵构成了一个图，规定左上角节点为入口，右下角节点为出口。若要使得每个节点与入口连通且边数最少，这样一个节点图为一棵树，因此问题可以转化为为寻找一棵随机生成树。
存储结构：
 - 定义结构体misRM存储每个节点所代表的房间的信息，其中的节点与父节点之间的操作采用了树结构
 - 定义类Maze存储迷宫的信息，搜索迷宫路径中用vector容器模拟栈的存储

主要算法：
1.首先处理出所有可能的边，将此边对应的位置初始化为墙，进行遍历，其中当前边连接的节点若

 - 未连通，则连通此边，将此边对应的墙拆开
 - 已连通，则跳过此边
 节点之间的连通性使用Union-Find数据结构进行判断与合并遍历完成后

2.将迷宫地图输出到控制台与一个txt文件中，再在迷宫地图上进行广度优先搜索寻找到起点到终点的路径。
#####3.2设计表示
1.迷宫房间信息 `struct misRM`
          `int rmset`
          代表Union-Find数据结构中本节点(本房间)的父节点
          `int rmpos`
          代表本房间的位置
          `int rmstate[N]`
          代表房间四面墙的开关状态，0代表关，1代表开
          `char rmdire`
          用于输出路径时的方向
2.迷宫类`class Maze`
`bool isConnect(misRM& m,misRM& n)`
查看两个房间是否相同，即两节点是否在同一等价类中
`void UnionRM(misRM& m,misRM& n)`
用于合并两个不在同一类的房间
`void CreateMaze()`
构造迷宫
`void FindPath(vector<misRM>& v)`
通过深度优先搜索搜索迷宫的路径
`void PathMsg(const string &s,vector<misRM>& v)`
输出迷宫路径信息
`void DisPlay(const string &s,vector<misRM>& v)`
输出迷宫信息并打印到txt文件中
#####3.3实现注释
各项功能已全部实现，新实现了在展示迷宫的同时展示通路的功能
#####3.4详细设计表示
```C++
#include"maze.h"

//初始化房间信息=>未形成迷宫
Maze::Maze(int m=2,int n=3):row(m),MyMaze(m*n)
{
	int s=MyMaze.size();
	if(s==0)
		return;

	for(int i=0;i<s;i++)
	{
		MyMaze[i].rmset=-1;                         //-1代表每个迷宫初始集合信息
		MyMaze[i].rmpos=i;                          //房间位置 一直保持不可改变
		for(int j=0;j<N;j++)                        //初始化每个房间墙的位置:0(关闭),1(打开)
			MyMaze[i].rmstate[j]=0;
	}
	
	MyMaze[0].rmstate[2]=1;                         //设置密宫入口及出口(左上进、右下出)
	MyMaze[s-1].rmstate[0]=1;
};

//查找集合并保留祖宗的位置
int Maze::FindSet(misRM& m,int& n)
{
	int s=m.rmset;
	n=m.rmpos;
	if(s>=0)
		s=FindSet(MyMaze[s],n);
	return s;
};

//查看两个房间是否连通
bool Maze::isConnect(misRM& m,misRM& n)
{
	int pm,pn;
	if(FindSet(m,pm)==FindSet(n,pn))
	{
		if(FindSet(m,pm)==-1)                        //-1代表两房间不再任何集合中
			return false;
		else
			if(pm==pn)                               //祖宗是一个
				return true;
		     else return false;
	}
	return false;

};


//合并房间生成路径
void Maze::UnionRM(misRM& m,misRM& n)
{
	int s1,s2,p1,p2;

	s1=FindSet(m,p1);
	s2=FindSet(n,p2);

	if(s1<=s2)                                         //m所在集合房间多
	{
		MyMaze[p1].rmset+=MyMaze[p2].rmset;            //房间数量合并
		MyMaze[p2].rmset=m.rmpos;                      //修改小数量集合祖宗新指向
	}
	else
	{
		MyMaze[p2].rmset+=MyMaze[p1].rmset;
		MyMaze[p1].rmset=n.rmpos;
	}

};

//开始随机构建迷宫路径并循环生成迷宫
void Maze::CreateMaze()
{
	int s=MyMaze.size();
	if(s<=0)  return;                                   //空房间集
	int p,w;                                            //p=>rmpos,w=>rmwall
	int pos;

	srand(time(0));
	while(1)
	{
		
		p=rand()%s;           //随机房间位置=>从此房间为基点开辟路径
		w=rand()%N;                                      //随机墙方向=>此方向开辟路径

		switch(w)
		{
		case 0:
			if((p%row)==(row-1))                         //右边所有房间右边墙不打开
				break;
			if(isConnect(MyMaze[p],MyMaze[p+1]))         //测试量房间是否在同一集合
				break;
			UnionRM(MyMaze[p],MyMaze[p+1]);              //合并房间
			MyMaze[p].rmstate[0]=1;                      //打开相关墙生成路径
			MyMaze[p+1].rmstate[2]=1;
			break;

		case 1:
			if((s-p)<=row)
				break;
			if(isConnect(MyMaze[p],MyMaze[p+row]))
				break;
			UnionRM(MyMaze[p],MyMaze[p+row]);
			MyMaze[p].rmstate[1]=1;
			MyMaze[p+row].rmstate[3]=1;
			break;

		case 2:
			if((p%row)==0)
				break;
			if(isConnect(MyMaze[p],MyMaze[p-1]))
				break;
			UnionRM(MyMaze[p],MyMaze[p-1]);
			MyMaze[p].rmstate[2]=1;
			MyMaze[p-1].rmstate[0]=1;
			break;

		case 3:
			if(p<row)
				break;
			if(isConnect(MyMaze[p],MyMaze[p-row]))
				break;
			UnionRM(MyMaze[p],MyMaze[p-row]);
			MyMaze[p].rmstate[3]=1;
			MyMaze[p-row].rmstate[1]=1;
			break;

		}

		if(FindSet(MyMaze[0],pos)==(-1*s))                 //所有房间都已经打开:s房间总数量
			break;
	}
};




//寻找迷宫路径

void Maze::FindPath(vector<misRM>& v)
{
	int s=MyMaze.size();
	if(v.size()==0)
		v.push_back(MyMaze[0]);
	int i=0;
	while(i<N)
	{
		switch(i)
		{
		case 0:
			if(v.back().rmpos==(s-1))
				break;
			if(v.back().rmstate[i]==1)
			{
				v.push_back(MyMaze[v.back().rmpos+1]);
				v.back().rmstate[2]=0;
				v.back().rmdire = 'E';
				FindPath(v);
			}
			break;

		case 1:
			if(v.back().rmpos==(s-1))
				break;
			if(v.back().rmstate[i]==1)
			{
				v.push_back(MyMaze[v.back().rmpos+row]);
				v.back().rmstate[3]=0;
				v.back().rmdire = 'S';
				FindPath(v);
			}
			break;

		case 2:
			if(v.back().rmpos==(s-1))
				break;
			if(v.back().rmstate[i]==1)
			{
				v.push_back(MyMaze[v.back().rmpos-1]);
				v.back().rmstate[0]=0;
				v.back().rmdire = 'W';
				FindPath(v);
			}
			break;

		case 3:
			if(v.back().rmpos==(s-1))
				break;
			if(v.back().rmstate[i]==1)
			{
				v.push_back(MyMaze[v.back().rmpos-row]);
				v.back().rmstate[1]=0;
				v.back().rmdire = 'N';
				FindPath(v);
			}
			break;
		}
		if(v.back().rmpos==(s-1))
			break;
		i++;
	}
	if(i==N)
		v.pop_back();
};



//查询迷宫中房间是否在迷宫路径中

bool Maze::isInPath(vector<misRM>& v,misRM& m)
{
	int s=MyMaze.size();
	vector<misRM>::iterator iter=v.begin();
	while(iter!=v.end())
	{
		if(iter->rmpos==m.rmpos)
			return true;
		iter++;
	}
	return false;
};



//输出迷宫路径信息
void Maze::PathMsg(const string &str,vector<misRM>& v)
{
	ofstream outfile;
	outfile.open(str.c_str(),ofstream::app);
	int s=MyMaze.size();
	vector<misRM>::iterator iter=v.begin();
	cout<<"Maze'Path:";
	outfile<<"Maze'Path:";
	while(iter!=v.end())
	{
		if(iter->rmpos==(s-1))
		{
			cout<<iter->rmdire;
			outfile<<iter->rmdire;;
		}
		else
		{
			cout<<iter->rmdire<<"->";
			outfile<<iter->rmdire<<"->";
		}
		iter++;
	}
	cout<<endl;
	outfile<<endl;
	outfile.close();
};



//输出迷宫信息
void Maze::DisPlay(const string &str)
{
	vector<misRM>::iterator iter=MyMaze.begin();
	int s=MyMaze.size();
	int count=0;

//从控制台中输出普通迷宫信息
	cout<<"********Maze's Msg********"<<endl;
	while(iter!=(MyMaze.begin()+row))
	{
		if(iter==MyMaze.begin())
		{
			cout<<" _ ";
			++iter;
			continue;
		}

		cout<<"_ ";
		++iter;
	}
	cout<<endl<<' ';

	iter=MyMaze.begin();
	while(iter!=MyMaze.end())
	{
		if(iter->rmstate[1]==0)
			cout<<"_";
		else
		{
			if((MyMaze.end()-iter)<=row)
				cout<<'_';
			else
				cout<<" ";
		}

		if(iter->rmstate[0]==0)
			cout<<'|';
		else
				cout<<' ';

		++count;
		if(count%row==0)
		{
			if(count>=s)
				cout<<endl<<endl;
			else
				cout<<endl<<"|";
		}
		++iter;
	}
	cout<<endl;
	ofstream outfile;
	outfile.open(str.c_str(),ofstream::app);
	if(!outfile)
	{
		cout<<"ERROR!Con not Input the Maze MSG into the File:"<<str<<endl;
		return;
	}
	count=0;
	iter=MyMaze.begin();

	outfile<<"********Maze's Msg********"<<endl;
	while(iter!=(MyMaze.begin()+row))
	{
		if(iter==MyMaze.begin())
		{
			outfile<<" _ ";
			++iter;
			continue;
		}

		outfile<<"_ ";
		++iter;
	}
	outfile<<endl<<' ';

	iter=MyMaze.begin();
	while(iter!=MyMaze.end())
	{
		if(iter->rmstate[1]==0)
			outfile<<'_';
		else
		{
			if((MyMaze.end()-iter)<=row)
				outfile<<'_';
			else
				outfile<<' ';
		}

		if(iter->rmstate[0]==0)
			outfile<<'|';
		else
				outfile<<' ';

		++count;
		if(count%row==0)
		{
			if(count>=s)
				outfile<<endl<<endl;
			else
				outfile<<endl<<'|';
		}
		++iter;
	}
	outfile<<endl;
	outfile.close();

};



//输出带路径的迷宫
void Maze::DisPlay(const string &str,vector<misRM>& v)
{
	vector<misRM>::iterator iter=MyMaze.begin();
	int s=MyMaze.size();
	int count=0;

//从控制台中输出带路径迷宫
	cout<<"********Maze(with path)'s Msg********"<<endl;
	while(iter!=(MyMaze.begin()+row))
	{
		if(iter==MyMaze.begin())
		{
			cout<<" _ ";
			++iter;
			continue;
		}

		cout<<"_ ";
		++iter;
	}
	cout<<endl<<' ';

	iter=MyMaze.begin();
	while(iter!=MyMaze.end())
	{
		if(iter->rmstate[1]==0)
			cout<<'_';
		else
		{
			if((MyMaze.end()-iter)<=row)
				cout<<'_';
			else
			{
				if(isInPath(v,*iter))                              //路径
					cout<<'#';
				else
					cout<<' ';
			}
		}

		if(iter->rmstate[0]==0)
			cout<<'|';
		else
		{
			if(isInPath(v,*iter)&&isInPath(v,*(iter+1)))
				cout<<'#';
			else cout<<' ';
		}

		++count;
		if(count%row==0)
		{
			if(count>=s)
				cout<<endl<<endl;
			else
				cout<<endl<<"|";
		}
		++iter;
	}
	cout<<endl;


//向文件中写入带路径迷宫
	ofstream outfile;
	outfile.open(str.c_str(),ofstream::app);
	if(!outfile)
	{
		cout<<"ERROR!Con not Input the Maze MSG into the File:"<<str<<endl;
		return;
	}

	count=0;
	iter=MyMaze.begin();

	outfile<<"********Maze(with path)'s Msg********"<<endl;
	while(iter!=(MyMaze.begin()+row))
	{
		if(iter==MyMaze.begin())
		{
			outfile<<" _ ";
			++iter;
			continue;
		}

		outfile<<"_ ";
		++iter;
	}
	outfile<<endl<<' ';

	iter=MyMaze.begin();
	while(iter!=MyMaze.end())
	{
		if(iter->rmstate[1]==0)
			outfile<<"_";
		else
		{
			if((MyMaze.end()-iter)<=row)
				outfile<<'_';
			else
			{
				if(isInPath(v,*iter))
					outfile<<'#';
				else
					outfile<<' ';
			}
		}

		if(iter->rmstate[0]==0)
			outfile<<'|';
		else
		{
			if(isInPath(v,*iter)&&isInPath(v,*(iter+1)))
				outfile<<'#';
			else 
				outfile<<' ';
		}

		++count;
		if(count%row==0)
		{
			if(count>=s)
				outfile<<endl<<endl;
			else
				outfile<<endl<<"|";
		}
		++iter;
	}
	outfile<<endl;
	outfile.close();
};

```
####4.调试报告
1.随机生成迷宫时，容易产生分布不均匀的现象，为了改进这一缺陷，对union-find进行优化，采用路径压缩以及控制每棵树的高度。
2.采用DFS寻找路径时，标记路径信息比较困难，后来重新定义了结构体来表示每个各自的墙面信息。搜索迷宫路径时房间墙壁的信息的存储采用了模拟栈的方式存储路径信息
####5.对于设计编码的回顾讨论和分析 
1.设计编码中不能熟练的应用某些数据结构与算法
2.设计编码中没有很好地使用模块化特性，代码文件偏长，不能很好定位
