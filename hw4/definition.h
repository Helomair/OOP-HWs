#include <vector>
#include <queue>
#include <iostream>
#include <cmath>
#define MIDDLE -1
#define DIRACTION -2
#define INTERSECTION -3
#define SELECT 1
#define DELETE 0

using namespace std;

static double pi = 4 * atan(1.0);
struct coordinate {
    int id;
    double x, y;
    //Distance square
    double operator ^ (struct coordinate neighbor) {
        return (x - neighbor.x) * (x - neighbor.x) + (y - neighbor.y) * (y - neighbor.y);
    }
    //equal operator
    void operator = (struct coordinate neighbor) {
        id = neighbor.id;
        x = neighbor.x;
        y = neighbor.y;
    }
    //find the middle point
    struct coordinate operator | (struct coordinate neighbor ) {
        struct coordinate trans;
        trans.id = MIDDLE;
        trans.x = (x + neighbor.x) / 2;
        trans.y = (y + neighbor.y) / 2;
        return trans;
    }
    //find the vector of two points
    struct coordinate operator ->* (struct coordinate via) {
        struct coordinate trans;
        trans.id = DIRACTION;
        trans.x = (via.x - x);
        trans.y = (via.y - y);
        return trans;
    }
    //define is the outer product in or out
    bool outer_product (struct coordinate via) {
        if ( x*via.y-y*via.x > 0 )
            return true;
        else
            return false;
    }
    //find the inner product
    double inner_product (struct coordinate via)    {
        return x * via.x + y * via.y;
    }
    //find the right hand angle
    double right_hand_angle (struct coordinate via)  {
        double angle = acos(inner_product(via) / (product()*via.product()));
        if (outer_product(via))
            return (pi-angle)/pi*180;
        else
            return (pi+angle)/pi*180;
    }
    double product () {
        return sqrt(x * x + y * y);
    }
};

//line ab and line cd ,return intersection point. 
struct coordinate get_inte (struct coordinate a,struct coordinate b,struct coordinate c,struct coordinate d)   {
    struct coordinate intersection;
    double m = (a.y - b.y) / (a.x - b.x);
    double y6 = m * (c.x - a.x) + a.y;
    double y5 = m * (d.x - a.x) + a.y;

    intersection.x = c.x + (d.x - c.x) * (y6 - c.y) / (d.y - y5 + y6 - c.y);
    intersection.y = m * (intersection.x - a.x) + a.y;
    intersection.id = INTERSECTION;
    return intersection;
}

//the sending package
class Package 
{
    public:
        struct coordinate source,intersection,distination;
        struct coordinate diraction;
        int next_hop, last_hop;
        bool visit[1000];
        queue<int> path;
};

class GFGPackage : public Package
{
  public:
    GFGPackage (int src, int des)
    {
        last_hop = des;
        next_hop = src;
        for(int i=0;i<1000;i++) visit[i] = 0;
    }
    struct coordinate deadend;
    int mode;
};

