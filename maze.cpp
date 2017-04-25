//MAZE.CPP--the file that keep the class'sfunctions

#include"maze.h"
/**************************************************************************************
								��ʼ��������Ϣ=>δ�γ��Թ�
**************************************************************************************/
Maze::Maze(int m=2,int n=3):row(m),MyMaze(m*n)
{
	int s=MyMaze.size();
	if(s==0)
		return;

	for(int i=0;i<s;i++)
	{
		MyMaze[i].rmset=-1;                         //-1����ÿ���Թ���ʼ������Ϣ
		MyMaze[i].rmpos=i;                          //����λ�� һֱ���ֲ��ɸı�
		for(int j=0;j<N;j++)                        //��ʼ��ÿ������ǽ��λ��:0(�ر�),1(��)
			MyMaze[i].rmstate[j]=0;
	}
	
	MyMaze[0].rmstate[2]=1;                         //�����ܹ���ڼ�����(���Ͻ������³�)
	MyMaze[s-1].rmstate[0]=1;
};



/**************************************************************************************
								���Ҽ��ϲ��������ڵ�λ��
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
								�鿴���������Ƿ���ͨ
**************************************************************************************/
bool Maze::isConnect(misRM& m,misRM& n)
{
	int pm,pn;
	if(FindSet(m,pm)==FindSet(n,pn))
	{
		if(FindSet(m,pm)==-1)                        //-1���������䲻���κμ�����
			return false;
		else
			if(pm==pn)                               //������һ��
				return true;
		     else return false;
	}
	return false;

};



/**************************************************************************************
									�ϲ���������·��
**************************************************************************************/
void Maze::UnionRM(misRM& m,misRM& n)
{
	int s1,s2,p1,p2;

	s1=FindSet(m,p1);
	s2=FindSet(n,p2);

	if(s1<=s2)                                         //m���ڼ��Ϸ����
	{
		MyMaze[p1].rmset+=MyMaze[p2].rmset;            //���������ϲ�
		MyMaze[p2].rmset=m.rmpos;                      //�޸�С��������������ָ��
	}
	else
	{
		MyMaze[p2].rmset+=MyMaze[p1].rmset;
		MyMaze[p1].rmset=n.rmpos;
	}

};



/**************************************************************************************
							 ��ʼ��������Թ�·����ѭ�������Թ�
**************************************************************************************/
void Maze::CreateMaze()
{
	int s=MyMaze.size();
	if(s<=0)  return;                                   //�շ��伯
	int p,w;                                            //p=>rmpos,w=>rmwall
	int pos;

	srand(time(0));
	while(1)
	{
		
		p=rand()%s;                                      //�������λ��=>�Ӵ˷���Ϊ���㿪��·��
		w=rand()%N;                                      //���ǽ����=>�˷��򿪱�·��

		switch(w)
		{
		case 0:
			if((p%row)==(row-1))                         //�ұ����з����ұ�ǽ����
				break;
			if(isConnect(MyMaze[p],MyMaze[p+1]))         //�����������Ƿ���ͬһ����
				break;
			UnionRM(MyMaze[p],MyMaze[p+1]);              //�ϲ�����
			MyMaze[p].rmstate[0]=1;                      //�����ǽ����·��
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

		if(FindSet(MyMaze[0],pos)==(-1*s))                 //���з��䶼�Ѿ���:s����������
			break;
	}
};



/**************************************************************************************
									  Ѱ���Թ�·��
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
						��ѯ�Թ��з����Ƿ����Թ�·����
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
								����Թ�·����Ϣ
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
									  ����Թ���Ϣ
            ˵��:Ϊ��ʾ����,�����Ѱ��ļ�����������̨�����ֿ�ʵ���û�����ѡ��ϲ�
**************************************************************************************/
void Maze::DisPlay(const string &str)
{
	vector<misRM>::iterator iter=MyMaze.begin();
	int s=MyMaze.size();
	int count=0;

/*****************************************************
					�ӿ���̨�������ͨ�Թ���Ϣ
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
					���ļ���д����ͨ�Թ���Ϣ
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
									�����·�����Թ�
            ˵��:Ϊ��ʾ����,�����Ѱ��ļ�����������̨�����ֿ�ʵ���û�����ѡ��ϲ�
**************************************************************************************/
void Maze::DisPlay(const string &str,vector<misRM>& v)
{
	vector<misRM>::iterator iter=MyMaze.begin();
	int s=MyMaze.size();
	int count=0;

/*****************************************************
					�ӿ���̨�������·���Թ�
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
				if(isInPath(v,*iter))                              //·��
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
					���ļ���д���·���Թ�
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