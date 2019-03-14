#include <algorithm>
#include <iostream>
#include <stack>
#include <fstream>
#include <sstream>
#include <string.h>
//typedef long long ll;
#define MAX_NODES 5000
using namespace std;

struct Edge
{
	int nxt;
	float cap,load,cost;
};

vector <Edge> Graph[MAX_NODES];
vector <string> input;
vector <int>    output[MAX_NODES];
int source[MAX_NODES], visit[MAX_NODES];
float value[MAX_NODES];

void find_cost(int load, int Nodes)
{
	for(int i = 0; i < Nodes; i++)
		for(auto& j : Graph[i])
			if(j.cap - j.load == 0)	j.cost = 1e9;
			else if (j.load > 0)	j.cost = (float)j.load / (float)(j.cap - j.load);
}
void init()
{
	for(int i = 0; i < MAX_NODES; i++)
		source[i] = -1, value[i] = 1e9, visit[i] = 0;
}
void Dijkstra(int from, int to, int Nodes)
{
	int min_now;
	float min_val = 1e9;
	value[from] = 0, visit[from] = 1;
	for(auto i : Graph[from])	value[i.nxt] = i.cost, cout<<"TEST : "<<from<<" "<<i.nxt<<" "<<i.load<<" "<<i.cost<<"\n";
	
	for(int i = 0; i < Nodes; i++)
	{
		min_val = 1e9;
		for(int j = 0; j < Nodes; j++)
			if(value[j] < min_val && !visit[j])
				min_val = value[j], min_now = j;
		visit[min_now] = 1;

		for(auto j : Graph[min_now])
		{
			if(value[j.nxt] > j.cost + min_val)
				value[j.nxt] = j.cost + min_val, source[j.nxt] = min_now;
			if(j.nxt == to)	cout<<"TEST : "<<to<<" "<<value[j.nxt]<<"\n";
		}
	}
}
void Update_load(int a, int b, int load)
{
	for(auto& i : Graph[a])	
		if(i.nxt == b)	i.load += load;
	for(auto& i : Graph[b])	
		if(i.nxt == a)	i.load += load;
}
int Testing(int a,int b,int load)
{
	for(auto& i : Graph[b])	
		if(i.nxt == a && i.load + load <= i.cap)	return 1;
	return 0;
}

int main()
{
	int Nodes , Edges , Flows;
	int count = 0 , total = 0;
	string inputstr;
	stringstream ss;
	stack <int> ans;
	ifstream in("request.txt");
    ofstream out("result.txt");

    while(getline(in, inputstr))
    	input.push_back(inputstr);
    ss << input[0];
	ss >> Nodes >> Edges ;
	ss.str("");ss.clear();
	for(int i = 0; i < Edges; i++ )
	{
		ss << input[1+i];
		int no, from, to, cap;
		ss >> no >> from >> to >> cap;
		Graph[from] . push_back ({to , (float)cap ,0 , 0.001});
		Graph[to] . push_back ({from , (float)cap ,0 , 0.001});
		ss.str("");ss.clear();
	}
	ss << input[1+Edges];
	ss >> Flows;
	ss.str("");ss.clear();
	for(int i = 0; i < Flows; i++)
	{
		ss << input[2+Edges+i];
		int no, from, to, load, flag = 1;
		ss >> no >> from >> to >> load;
		ss.str("");ss.clear();
		init();
		Dijkstra(from, to, Nodes);
		cout<<value[to]<<"\n";
		
		if(!visit[to])	{cout<<no<<" -1\n"; continue;}
		else
		{
			while(!ans.empty())	ans.pop();
			ans.push(to);
			int tmp = source[to];
			while(tmp != -1 && flag)
			{
				ans.push(tmp);
				flag = Testing(to, tmp, load);
				to = tmp;
				tmp = source[tmp];
			}
			ans.push(from);
			if(!flag)	continue;
			//cout<<to<<" "<<from<<"\n";
			flag = Testing(to, from, load);
			if(!flag)	continue;

			tmp = ans.top();
			output[count].push_back(no);
			while(!ans.empty())
				output[count].push_back(ans.top()), Update_load(tmp,ans.top(),load), tmp = ans.top(), ans.pop();

			count ++, total += load;
			find_cost(load, Nodes);
		}
	}
	out << count << " " << total << "\n";
	for(int i = 0; i < count; i++)
	{
		for(int j : output[i])
			out<<j<<" ";
		out<<"\n";
	}
	return 0;
}