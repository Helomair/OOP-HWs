#include "routing.h"
#include <iostream>
#include <vector>
#define MAX_NODE 1000

using namespace std;

void init(struct coordinate (&input)[MAX_NODE]);
void run(int src, int des);

vector<class GFG> GFGnode;
vector<class FR> FRnode;

int main()  {
    freopen("test_in.txt", "r", stdin);
    //freopen("test_out.txt", "w", stdout);
    struct coordinate input[MAX_NODE];
    
    init(input);

    //flattening the grath
    for (auto &i : FRnode)
        i.flattening();

    int source, destination;
    int test; 
    cin >> test;
    for (int i = 0; i < test; i++) {
        cin >> source >> destination ;
        run(source, destination);
    }
    return 0;
}

//setting the neighbor node
void init(struct coordinate (&input)[MAX_NODE]) {
    int total_node;
    cin >> total_node ;

    for (int i = 0; i < total_node;i++)
        cin >> input[i].id >> input[i].x >> input[i].y ;
    for (int i = 0; i < total_node; i++) {
        FR trans_fr(input[i]);
        GFG trans_gfg(input[i]);
        GFGnode.push_back(trans_gfg);
        FRnode.push_back(trans_fr);
        for (int j = 0; j < total_node; j++)
            if (i!=j && (input[i]^input[j]) < 1 )   {
                struct coordinate tr;
                tr = input[j];
                GFGnode[i].push_nearby(tr);
                FRnode[i].push_nearby(tr);
            }
    }
}


void run(int src, int des)
{
    GFGPackage pkg(src,des);
    pkg.source = GFGnode[src].info();
    //cout << pkg.source.x << " " << pkg.source.y << "\n";
    pkg.distination = GFGnode[des].info();
    pkg.intersection = GFGnode[src].info();
    pkg.deadend = GFGnode[src].info();
    pkg.diraction = pkg.distination ->* pkg.source;
    pkg.mode = 0;
    int test_time = 1000;
    while(test_time--)
    {
        GFGnode[pkg.next_hop].getNextHop(pkg);
        if(pkg.mode == 1) FRnode[pkg.next_hop].getNextHop(pkg);
        if(pkg.mode == -1)
            break;
    }
    while(!pkg.path.empty())
        cout << pkg.path.front() << " ", pkg.path.pop();
    cout << "\n";
}




