#include <bits/stdc++.h>
//typedef long long ll;
using namespace std;
#define MAX_RESOURCE 500

int se;
default_random_engine gen;
uniform_real_distribution<double> dis(0,1);

class Request
{
  private:
    int 	id, resourceId;
    bool 	matched = 0;
    double 	weight; // a random value, indicates the priority
  public:
    void inputID (int ID){
  		id = ID;
  	}
  	int getID() {
  		return id;
  	}
  	void inputWeight() {
  		weight = dis(gen);
  	}
  	double getWeight() {
  		return weight;
  	}
  	int getResourceId() {
  		return resourceId;
  	}
  	bool getMatched() {
  		return matched;
  	}
    friend class Resource;
    // accessor
};

// You need forward declaration
class Resource
{
  private:
	
    int 		id, requestId;
    bool 		matched = 0;
  public:
  	static int 	ans;
  	void inputID(int ID){
  		id = ID;
  	}
  	bool getMatched() {
  		return matched;
  	}
  	int getID() {
  		return id;
  	}
    bool operator ->* (Request &b) {
    	if(b.matched || matched)	return 0;
    	matched = b.matched = 1;
    	requestId = b.id;
    	b.resourceId = id;
    	return 1;
    }
};
int Resource::ans = 0;
struct Edge
{
  	double w;
  	int node;
  	bool operator < (const Edge& b) const
    {
    	if(w == b.w)  return node < b.node;
    	return w > b.w;
    }
};
vector<Resource> resource;
vector<Request> request;
priority_queue<Edge> graph[MAX_RESOURCE];

int main(int argc, char** argv)
{
	int n;
	se = atoi(argv[0]);
	gen.seed(se);
	freopen("request.txt","r",stdin);
	freopen("result.txt","w",stdout);
	cin >> n;
	while(n--)
	{
		int timeSlot, requests, resources, edges;

		cin >> timeSlot >> requests >> resources;
		for(int i=0; i<requests; i++)
		{
			Request tmp;
			int id;
			cin >> id;
			tmp.inputID(id);
			tmp.inputWeight();
			request.push_back(tmp);
		}

		for(int i=0; i<resources; i++)
		{
			Resource tmp;
			int id;
			cin >> id;
			tmp.inputID(id);
			resource.push_back(tmp);
		}

		cin >> edges;
		for(int i=0; i<edges; i++)
		{
			int no, from, to;
			cin >> no >> from >> to;
			graph[to].push(Edge{request[from].getWeight(),from} );
		}

		for(auto i:resource)
		{
			if(i.getMatched())
				continue;
			int id = i.getID();
			while(graph[id].size())
			{
				auto tmp = graph[id].top();
				graph[id].pop();
				if(resource[id]->*request[tmp.node])
				{
					Resource::ans ++;
					break;
				}
			}
		}
	}
	cout<<Resource::ans<<"\n";
	for(auto i:request)
		if(i.getMatched())
			cout << i.getID() << " " << i.getResourceId() <<"\n";
	return 0;
}