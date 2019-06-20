#include "definition.h"

class Node 
{
    friend void run(int source, int distination, vector<class Node> &node);
  public:
    Node(struct coordinate tr2){    location = tr2;    }
    void push_nearby (struct coordinate a) {  nearby_node.push_back(a);  }
    // if the circle between me and the other node has another node -> delete the other node
    void flattening() 
    {
        struct coordinate middle;
        double radius;
        int table[nearby_node.size()];

        for (int i = 0; i < nearby_node.size(); i++)    
        {
            table[i] = SELECT;
            middle = location | nearby_node[i];
            radius = middle ^ location;
            for (int j = 0; j < nearby_node.size(); j++)    
                if (i != j && radius > ( nearby_node[j] ^ middle ) )  
                {
                    table[i] = DELETE;
                    break;
                }
        }
        for (int i = 0; i < nearby_node.size(); i++)
            if (table[i] == SELECT)
                cutoff_node.push_back(nearby_node[i]);
    }
    //accept the package from other node
    void receive(struct Package package){   package_queue.push(package);  }

    //check if the backage are on the right way
    void check() 
    {
        while (package_queue.size()!=0) 
        {
            struct Package package = package_queue.front();
            package_queue.pop();
            if (package.next_hop == location.id) 
                modify(package);
        }
    }
    
    //detemine the correct next hop
    void modify(struct Package package) 
    {
        if (location.id == package.distination.id) 
        {
            package.path.push(location.id);
            print_flow(package);
            return;
        }
        int min_node;
        double angle,min_angle=361;
        for (int i = 0; i < cutoff_node.size(); i++) 
        {
            angle = package.diraction.right_hand_angle(location->*cutoff_node[i]);
            if (angle <0.00001) angle =360;
            if ( angle < min_angle ) 
            {
                min_angle = angle;
                min_node = i;   
            }
        }
        if ( between(package.distination,package.intersection,location,cutoff_node[min_node]) ) 
        {
            package.intersection = get_inte(package.distination, package.intersection, location, cutoff_node[min_node]);
            package.diraction = package.intersection ->* location;
            if (package.diraction.outer_product(package.intersection ->* package.source)!=1) 
            {
                modify(package);
                return;
            }
        }
        package.next_hop = cutoff_node[min_node].id;
        package.last_hop = location.id;
        package.diraction = location ->* cutoff_node[min_node];
        package.path.push(location.id);
        send_queue.push(package);
    }

    //initialize a new package from source
    void make_package (struct coordinate distination) {
        struct Package package;
        package.source = location;
        package.distination = distination;
        package.intersection = location;
        package.last_hop = distination.id;
        package.next_hop = location.id;
        package.diraction = distination ->* location;

        for (int i = 0; i < cutoff_node.size(); i++) 
            if (distination.id == cutoff_node[i].id)    
            {
                package.path.push(location.id);
                package.last_hop = location.id;
                package.next_hop = distination.id;
                package.diraction = location->* distination;
                send_queue.push(package);
                return;
            }
        receive(package);
    }

    //return the information of this node
    struct coordinate info () {     return location;    }

    void print_flow (struct Package package) 
    {
        while (package.path.size()){
            cout << package.path.front() << " ";
            //printf("%d ", package.path.front());
            package.path.pop();
        }
        cout << "\n";
    }

   private:
    queue<struct Package> send_queue;
    vector<struct coordinate> nearby_node;
    vector<struct coordinate> cutoff_node;
    queue<struct Package> package_queue;
    struct coordinate location;

    //determine if the new intersection is between the old intersection and distination or not
    bool between (struct coordinate a,struct coordinate b,struct coordinate c,struct coordinate d)    
    {
        struct coordinate intersection = get_inte(a,b,c,d);
        double aa, bb, cc, dd;
        if (a.x > b.x)      { aa = a.x, bb = b.x; }
        else                { aa = b.x, bb = a.x; }
        if (c.x > d.x)      { cc = c.x, dd = d.x; }
        else                { cc = d.x, dd = c.x; }

        if (intersection.x < aa - 0.0001 && intersection.x > bb + 0.0001 && intersection.x < cc - 0.0001 && intersection.x >dd + 0.0001 )
            return true;
        else
            return false;
    }
};

//run the test case
void run (int source,int distination,vector<class Node> &node) 
{
    node[source].make_package(node[distination].info());
    int n = 200;
    while (n--) 
    {
        for (auto &i : node)   
            i.check();
       
        for (auto &i : node)
            while (i.send_queue.size()!=0) 
            {
                struct Package package = i.send_queue.front();
                i.send_queue.pop();
                for (auto &j : i.nearby_node)    
                {
                    int now = j.id;
                    node[now].receive(package);
                }
            }
    }
    cout << "\n";
}