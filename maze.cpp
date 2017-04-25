//MAZE.CPP--the file that keep the class'sfunctions

#include"maze.h"
/**************************************************************************************
								初始化房间信息=>未形成迷宫
**************************************************************************************/
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



/**************************************************************************************
								查找集合并保留祖宗的位置
**************************************************************************************/
int Maze::FindSet(misRM& m,int& n)
{
	int s=m.rmset;
	n=m.rmpos;
	if(s>=0)
		s=FindSet(MyMaze[s],n);
	return s;
};



/**************************************************************************************
								查看两个房间是否连通
**************************************************************************************/
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



/**************************************************************************************
									合并房间生成路径
**************************************************************************************/
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



/**************************************************************************************
							 开始随机构建迷宫路径并循环生成迷宫
**************************************************************************************/
void Maze::CreateMaze()
{
	int s=MyMaze.size();
	if(s<=0)  return;                                   //空房间集
	int p,w;                                            //p=>rmpos,w=>rmwall
	int pos;

	srand(time(0));
	while(1)
	{
		
		p=rand()%s;                                      //随机房间位置=>从此房间为基点开辟路径
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



/**************************************************************************************
									  寻找迷宫路径
**************************************************************************************/
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


/**************************************************************************************
						查询迷宫中房间是否在迷宫路径中
**************************************************************************************/
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



/**************************************************************************************
								输出迷宫路径信息
**************************************************************************************/
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
			cout<<iter->rmpos;
			outfile<<iter->rmpos;;
		}
		else
		{
			cout<<iter->rmpos<<"->";
			outfile<<iter->rmpos<<"->";
		}
		iter++;
	}
	cout<<endl;
	outfile<<endl;
	outfile.close();
};



/**************************************************************************************
									  输出迷宫信息
            说明:为显示清晰,下面已把文件操作及控制台操作分开实现用户可以选择合并
**************************************************************************************/
void Maze::DisPlay(const string &str)
{
	vector<misRM>::iterator iter=MyMaze.begin();
	int s=MyMaze.size();
	int count=0;

/*****************************************************
					从控制台中输出普通迷宫信息
*****************************************************/
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



/*****************************************************
					向文件中写入普通迷宫信息
*****************************************************/
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



/**************************************************************************************
									输出带路径的迷宫
            说明:为显示清晰,下面已把文件操作及控制台操作分开实现用户可以选择合并
**************************************************************************************/
void Maze::DisPlay(const string &str,vector<misRM>& v)
{
	vector<misRM>::iterator iter=MyMaze.begin();
	int s=MyMaze.size();
	int count=0;

/*****************************************************
					从控制台中输出带路径迷宫
*****************************************************/
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
					cout<<'@';
				else
					cout<<' ';
			}
		}

		if(iter->rmstate[0]==0)
			cout<<'|';
		else
		{
			if(isInPath(v,*iter)&&isInPath(v,*(iter+1)))
				cout<<'@';
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


/*****************************************************
					向文件中写入带路径迷宫
*****************************************************/
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
					outfile<<'@';
				else
					outfile<<' ';
			}
		}

		if(iter->rmstate[0]==0)
			outfile<<'|';
		else
		{
			if(isInPath(v,*iter)&&isInPath(v,*(iter+1)))
				outfile<<'@';
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
/**************************************************************************************
									
**************************************************************************************/