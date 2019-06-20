#include "definition.h"

class RP
{
  public:
    void getNextHop() {}
};

class FR : public RP 
{
    friend void run(int source, int distination, vector<class Node> &node);
  public:
    FR(struct coordinate tr2){    location = tr2;    }
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
    void receive(struct GFGPackage package){   package_queue.push(package);  }

    void print()
    {
        cout << "  " << location.id << " " << location.x << " " << location.y << "\n";
        for(auto i : nearby_node)
            cout << "     " << i.id << " " << i.x << " " << i.y << "\n";
    }

    //check if the backage are on the right way
    void check() 
    {
        while (package_queue.size()!=0) 
        {
            struct GFGPackage package = package_queue.front();
            package_queue.pop();
            if (package.next_hop == location.id) 
                getNextHop(package);
        }
    }
    
    //detemine the correct next hop
    void getNextHop(GFGPackage &package) 
    {
        if (location.id == package.distination.id) 
        {
            package.path.push(location.id);
            package.mode = -1;
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
                getNextHop(package);
                return;
            }
        }
        package.next_hop = cutoff_node[min_node].id;
        package.last_hop = location.id;
        package.diraction = location ->* cutoff_node[min_node];
        package.path.push(location.id);
        package.visit[cutoff_node[min_node].id] = 1;
        send_queue.push(package);
        //cout << "[F] : " << cutoff_node[min_node].id << " " << package.deadend.id << " " << (package.deadend ^ package.distination) << " " << package.mode << "\n";
    }

    //return the information of this node
    struct coordinate info () {     return location;    }

    void print_flow (GFGPackage package) 
    {
        while (package.path.size()){
            cout << package.path.front() << " ";
            //printf("%d ", package.path.front());
            package.path.pop();
        }
        cout << "\n";
    }

   private:
    queue<class GFGPackage> send_queue;
    vector<struct coordinate> nearby_node;
    vector<struct coordinate> cutoff_node;
    queue<class GFGPackage> package_queue;
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

class GFG : public FR
{
  public:
    GFG(struct coordinate tr2) : FR(tr2) {}

    void push_nearby (struct coordinate a) {  nearby_node.push_back(a);  }

    void getNextHop(GFGPackage &pkg) 
    {
        struct coordinate location = info();
        if (location.id == pkg.distination.id) 
        {
            pkg.path.push(location.id);
            pkg.mode = -1;
            return;
        }
        struct coordinate closest = nearby_node[0];
        for(auto &i : nearby_node)
            closest = ( (i ^ pkg.distination) > (closest ^ pkg.distination)) ? closest : i;
        // dead end
        if( !pkg.mode &&( (pkg.distination ^ closest) > (pkg.distination ^ pkg.deadend) ))
        {
            pkg.deadend = location;
            pkg.visit[location.id] = 1;
            pkg.mode = 1;
            return;
        }
        else if ( (pkg.distination ^ closest) < (pkg.distination ^ pkg.deadend) )
        {
            //cout << "[G] : " << closest.id << " " << pkg.deadend.id << " " << (pkg.distination ^ closest) << " " << (pkg.distination ^ pkg.deadend) << "\n";
            pkg.last_hop = location.id;
            pkg.next_hop = closest.id;
            pkg.path.push(location.id);
            pkg.diraction = pkg.distination ->* closest;
            pkg.deadend = closest;
            pkg.visit[closest.id] = 1;
            pkg.mode = 0;
        }
        else
        {
            pkg.mode = 1;
            //cout << "[G????] : " << closest.id << " " << pkg.deadend.id << " " << (pkg.distination ^ closest) << " " << (pkg.distination ^ pkg.deadend) << "\n";
        }
    }
  private:
    vector<struct coordinate> nearby_node;
    queue<struct GFGPackage> package_queue;
};
