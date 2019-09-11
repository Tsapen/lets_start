#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <stack>
#include "inter.h"
#include <cmath>


vector<vector<int> > world;
vector<vector<int> > xray;


Node* location = nullptr;
Node* finish = nullptr;
Node* crash = nullptr;
const int SZ = 10;

void get_map()
{
    char a;
    int posit = 0;
    world.resize(SZ);
    ifstream fin("map.txt");
    for(int i = 0; i<SZ;++i)
    {
        world[i].resize(SZ);
        for(int j = 0; j <SZ; ++j)
        {
            fin>>world[i][j];
            fin>>a;
        }
    }

    fin>>posit;
    location = new Unsigned(posit, 0);
    int cexit = 500;
    finish = new Unsigned(cexit, 0);
    crash = new Unsigned(0,0);

    xray.resize(5);
    for(int i = 0; i < 5 ; ++i)
        xray[i].resize(5);
}

void do_xray()
{
    int posit = location->ex();
    int x = posit/10, y = posit%10;
    for(int i = x-2, j = 0; i < x+3 ; ++i, ++j)
    {
        for(int k = y-2, l = 0 ; k < y+3 ; ++k, ++l )
            if(k>-1 && k<SZ && i>-1 && i<SZ)

                xray[j][l] = world[i][k];
            else
                xray[j][l] = 0;
    }
    return;
}

Cell::Cell(int _loc, int _is_const = 0): is_const(_is_const), loc(_loc) {}

Cell::Cell(const Cell& a): is_const(a.is_const), loc(a.loc)   {}

int Cell::type() {return 11;}

int Cell::ex()    {  return loc;  }

void Cell::set_value(int _loc)  { if(is_const) {OutErr("\nVar const\n");   return;    } loc = _loc;   }


//0- top, 1 - left, 2 - bot, 3 - right

int Action::type() {return 4;}

int Action::ex()
{
    int posit = location->ex(), x = check_go();
    if(x)
    {
        posit+=x;
        location->set_value(posit);
        return 1;
    }
    crash->set_value(1);
    OutErr("Robot has been crashed");
    return 0;
}

map<int,int>dis= {{1,1},{10,10},{100,-1},{1000,-10},{0,0}};

//check can robot go: 0 if wall         -10, -1 , 1, 10 if can dislocate

int Action:: check_go()
{
    int y = pow(10,(3 - dir)), posit = location->ex();
    if((world[posit/10][posit%10]%(10*y))/y)
        y=0;
    return dis[y];
}

/*{"Top",0},{"NTop",1},{"Bot",2},{"NBot",3},{"Left",4},{"NLeft",5},{"Right",6},
 * {"NRight",7},{"L/",8},{"0/0",9},{"L-",10},{"(",11}};         */

Lexpr::Lexpr(int _act, vector<Node*>_op): act(_act), op(_op), Node() {}

int Lexpr::type(){return 7;}

int Lexpr::ex()
{
    int posit = location->ex();
    switch (act)
    {
    case 8:  { if((op[0]->ex() &&! op[1]->ex())||(!op[0]->ex() && op[1]->ex()))     return 1;     return 0; }
    case 9:  { if(op[0]->ex() || op[1]->ex())     return 1;     return 0;        }
    case 10:  { if(op[0]->ex() && op[1]->ex())     return 1;     return 0;        }
    case 11: { return op[0]->ex();  }
    }

    int y = pow(10,(3 - act/2));
    if((world[posit/10][posit%10]%(10*y))/y)
        y=0;
    else
        y = 1;
    return dis[y];

}
