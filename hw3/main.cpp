#include "routing.h"
#include <iostream>
#include <vector>
#define MAX_NODE 1000

using namespace std;

void init(struct coordinate (&input)[MAX_NODE],vector<class Node> &node);
void run(int source, int distination, vector<class Node> &node);

int main()  {
    freopen("temp_in.txt", "r", stdin);
    freopen("teno_out.txt", "w", stdout);
    struct coordinate input[MAX_NODE];
    vector<class Node> node;
    init(input,node);

    //flattening the grath
    for (auto &i : node)
        i.flattening();

    int source, distination;
    int test; 
    cin >> test;
    for (int i = 0; i < test; i++) {
        cin >> source >> distination ;
        run(source, distination, node);
    }
    return 0;
}

//setting the neighbor node
void init(struct coordinate (&input)[MAX_NODE],vector<class Node> &node) {
    int total_node;
    cin >> total_node ;

    for (int i = 0; i < total_node;i++)
        cin >> input[i].id >> input[i].x >> input[i].y ;

    for (int i = 0; i < total_node; i++) {
        Node trans(input[i]);
        node.push_back(trans);
        for (int j = 0; j < total_node; j++)
            if (i!=j && (input[i]^input[j]) < 1 )   {
                struct coordinate tr;
                tr = input[j];
                node[i].push_nearby(tr);
            }
    }
}




