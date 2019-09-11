#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <stack>
#include "inter.h"

int err_flag = 0;// errors counter
int ex_index[] = {-1,-1};//buffer for work with matrixes
int cnst = 0;
stack<Node*> argum;
stack<Node*> dynamic;
stack<map<string,Node*>>  cur_data;
map<string, Node*> pfunc;
list<string>errs;
int spec_flag = 0;
Node *cur_arg = nullptr;

int futype = 0;
void OutErr(string _err){ ++err_flag; throw (_err);}

void starts(){ get_map(); Program go;    }

Program::Program(){
    if (pfunc.find("start")!=pfunc.end())
    {
        Node *_start =pfunc.find("start")->second;
        try{
            int res =  _start->ex();
            cout<<"Result: "<< res<<"\nNum of errrors: "<<err_flag<<endl;
        }
        catch(string _err)
        {
            cout<<"\nError: "<<_err<<endl;
        }
        return;
    }
    OutErr("\nStart is not found\n");
}

Program::~Program()
{
    while(!dynamic.empty())
    {
        delete dynamic.top();
        dynamic.pop();
    }
}

Signed::Signed(int _num, int _is_const = 0):value (_num), is_const(_is_const)  {}

int Signed::type() {return 9;}

int Signed::ex()  {return value;}

void Signed::set_value(int a)   { if(is_const) OutErr("\nVar is const\n"); else value=a; }


Unsigned::Unsigned(int _num, int _is_const = 0): is_const(_is_const)
{
    if (_num < 0)  { OutErr("\nTrying of initialisation is error\n");  value = 0;   }
    else  value = _num;
}

int Unsigned::type(){return 10;}

int Unsigned::ex()  {return value;}

void Unsigned::set_value(int a)
{
    if(is_const)  { OutErr("\nVar is const\n");  return;    }

    if(a < 0)   {  OutErr("\nTrying of asssignment is error\n");  value = 0;     }

    value=a;

}

Adress::Adress(string _name):name(_name), ind({nullptr,nullptr}){}

Adress::Adress(string _name, Node* expr1, Node* expr2): name(_name), ind({expr1,expr2})   { }

int Adress::type()
{
    auto ptr = cur_data.top().find(name);

    if(ptr==(cur_data.top().end()))  { OutErr("\nUndeclared variabl\n");  return -1;    }

    return ptr->second->type();
}

int Adress::ex()
{
    int x = 0;
    if(ind[0])   {x=1;  ex_index[0]=ind[0]->ex();     ex_index[1]=ind[1]->ex();}

    if((cur_data.top().find(name))==(cur_data.top().end())) 
    {
        OutErr("\nUndecl var\n"); 
        return -1;      
    }

    if(spec_flag)
    {
        cur_arg = cur_data.top().find(name)->second;
        return 1;
    }
    int res = cur_data.top().find(name)->second->ex();



    return res;
}

void Adress::set_value(int a)
{
    auto ptr = cur_data.top().find(name);
    if(ptr==(cur_data.top().end()))     
    {
        OutErr("\nUndecl var\n");  
        return;  
    }

    if(ind[0])
    {
        ex_index[0]=ind[0]->ex();   
        ex_index[1]=ind[1]->ex();
    }

    cur_data.top().find(name)->second->set_value(a);

}

Matrix::Matrix(int _height, int _width, int _mtype, int _is_const = 0):
    height(_height),width(_width), mtype(_mtype), is_const(_is_const)
{
    if(_is_const == 2 && _mtype==11 && _height==5 && _width==5)
    {
        is_const = 0;
        do_xray();
        int posit = location->ex();
        int u = posit/10, r = posit%10;
        for(int i = u-2; i < u+3; ++i)
            for(int j= r-2; j < r+3; ++j) { matrix[i][j] = new Cell(xray[i][j],1); dynamic.push(matrix[i][j]);}
        return;
    }

    matrix.resize(height);
        for(int i = 0; i < height; ++i)           
            matrix[i].resize(width, nullptr);


}

int Matrix::type()    {  return mtype;    }

int Matrix::ex()
{
    Node *cur = matrix[ex_index[0]][ex_index[1]];

    if(cur)
        return cur->ex();
    OutErr("Element has not init");
    return -1;
}

void Matrix::set_value(int a)
{
    if(a==5 && mtype==11 && height==5 && width==5 )
    {
        do_xray();
        for(int i = 0; i < 5; ++i)
            for(int j= 0; j < 5; ++j)   { matrix[i][j] = new Cell(xray[i][j],1); dynamic.push(matrix[i][j]);    }
        return;
    }

    if(is_const==1)    {    OutErr("\nMatrix is constant\n");   return;      }

    Node *x = matrix[ex_index[0]][ex_index[1]];
    if(!x)
        switch (mtype)
        {
        case 9:
        {
            matrix[ex_index[0]][ex_index[1]]= new Signed(a,0);
            dynamic.push(matrix[ex_index[0]][ex_index[1]]);
            return;
        }
        case 10:
        {
            matrix[ex_index[0]][ex_index[1]] = new Unsigned(a,0);
            dynamic.push(matrix[ex_index[0]][ex_index[1]]);
            return;
        }
        case 11:
        {
            matrix[ex_index[0]][ex_index[1]]= new Cell(a,0);
            dynamic.push(matrix[ex_index[0]][ex_index[1]]);
            return;
        }
        }

    x->set_value(a);
}

Func::Func(list<pair<string,int>>_args, list<Node*>_body): args(_args),body(_body), Node(){  }

int Func::type()      {        return futype;    }

int Func::ex()
{
    cur_data.push({});

    for (auto cur_pair: args)
    {
        if(argum.top()->type()==(cur_pair.second))
        {
                cur_data.top().insert(make_pair(cur_pair.first,argum.top()));
                argum.pop();
        }
        else   {  OutErr("\nList of arguments is not correct\n");   return 0;}

    }
    cur_data.top().insert(make_pair("location",location));
    cur_data.top().insert(make_pair("finish",finish));
    cur_data.top().insert(make_pair("crash",crash));
    int res = -1;
    for (auto ptr:body)     {   res = ptr->ex();       }
    Node *x = body.back();
    futype = x->type();
    cur_data.pop();

    return res;
}


While::While(Node *_condition, list<Node*>_body):condition(_condition), body(_body), Node(){}

int While::type(){ return 2;}

int While::ex()
{
    int cond = 0;
    while(cond = condition->ex())
    {
        for(auto ptr:body)
            ptr->ex();
    }
    return 0;

}


If::If(Node *_condition, list<Node*> _body):condition(_condition), body(_body), Node(){}

int If::type(){ return 0; }

int If::ex()
{
    int cond = condition->ex();
    if(cond)
    {
        for(auto ptr:body)
            ptr->ex();
    }
    return 0;
}


Assig::Assig(Node *_adress, Node*_expr):   adress(_adress), expr(_expr){}

int Assig::type(){return adress->type();}

int Assig::ex()
{
    int res =expr->ex();
    int t1 = adress->type(),t2=expr->type();
    if((adress->type()) == (expr->type()))
    {
        adress->set_value(res);
        return 1;
    }
    OutErr("\nTypes are not equel\n");    
    return 0;
}


Variable::Variable (int i,string n, int v, int s1, int s2):size({s1,s2}),name(n),vtype(v), is_const(i), expr(nullptr){}

Variable::Variable (int i,  int v,string n, Node*e) : size({-1, -1}),name(n), vtype(v), is_const(i), expr(e){}

Variable::Variable (string n, int v, Node*e) : size({-1, -1}),name(n), vtype(v), is_const(0), expr(e){}

int Variable::type(){return vtype;}

int Variable::ex()
{
    if((cur_data.top().find(name))!= (cur_data.top().end()))
        return -1;

    if(!expr)
    {
        Matrix* mtr = new Matrix(size[0],size[1], vtype, is_const);
        dynamic.push(mtr);
        cur_data.top().insert(make_pair(name,mtr));
        return 0;
    }
    switch(vtype)
    {
        case 9:
        {
            int res = 0;
            if(expr)
                res =expr->ex();

            if(expr && (expr->type() != vtype))    {  OutErr("\nUnreal initialisation\n");    return -1;     }

            Signed* sgn = new Signed(res,is_const);
            dynamic.push(sgn);
            cur_data.top().insert(make_pair(name,sgn));
            return 0;
        }
        case 10:
        {
            int res = 0;
            if(expr)
                res = expr->ex();
            if(expr && expr->type() != vtype)     {  OutErr("\nUnreal initialisation\n");   return 0;         }

            Unsigned* unsgn = new Unsigned(res,is_const);
            dynamic.push(unsgn);
            cur_data.top().insert(make_pair(name,unsgn));
            return 0;
        }
        case 11:
        {
            int res = 0;
            if(expr)
                res = expr->ex();
            if(expr && expr->type() != vtype)     {  OutErr("\nUnreal initialisation\n");   return 0;         }

            Cell* unsgn = new Cell(res,is_const);
            dynamic.push(unsgn);
            cur_data.top().insert(make_pair(name,unsgn));
            return 0;
        }
    }
    return -1;
}

map<string, int> table= {{"Conv_s",0},{"Conv_us",1},{"Adress",2},
                        {"Xray",3},{"Call",4},{"Num",5},{"Unum",6},
                        {"+",7},{"-",8},{"/",9},{"*",10},{"i-",11},
                        {"<",12},{">",13},{"=",14},{")",15},{"Conv_cell",16}};

int Expr::ex()
{
    switch(expr)
    {
    case 0:
    {
        switch(operands[0]->type() - 9)
        {
        case 0:{ return operands[0]->ex();       }
        case 1:    {
            int a = operands[0]->ex();
            unsigned un = (unsigned)a;
            if(un != a)  {   OutErr("\nOverflow\n");     return -1;                }

            return a;
        }
        case 2:    {  return 0;     }
        }
    }
    case 1:
    {
        switch(operands[0]->type() - 9)
        {
        case 0:     {   return abs(operands[0]->ex());           }
        case 1:       {   return operands[0]->ex();         }
        case 2:            {  return 0;        }
        }
    }
    case 2:        { int res = operands[0]->ex(); return res;    }
    case 3:     { return 5;}
    case 4:
    {
        spec_flag = 1;
        for(int i = 1 ; operands[i] ; ++i)
        {   operands[i]->ex();
            if(cur_arg)
            {
                argum.push(cur_arg);
                cur_arg = nullptr;
            }
            else
                argum.push(operands[i]);
        }
        spec_flag = 0;
        int res = operands[0]->ex();
        return res;
    }
    case 5:        { int res = operands[0]->ex(); return res;       }
    case 6:        { int res = operands[0]->ex(); return res;  }
    case 7:
    {
        if(fail_check_type())      return 0;

        int res1 = operands[0]->ex(), res2 = operands[1]->ex();
        return res1+res2;
    }
    case 8:
    {
        if(fail_check_type())          return 0;

        int res1 = operands[0]->ex(), res2 = operands[1]->ex();
        return res1-res2;
    }
    case 9:
    {
        if(fail_check_type())
            return -1;

        int res1 = operands[0]->ex(), res2 = operands[1]->ex();
        if(!res2)
            OutErr("\nDivide on zero\n");
        return res1/res2;
    }
    case 10:
    {
        if(fail_check_type())
            return -1;
        int res1 = operands[0]->ex(), res2 = operands[1]->ex();
        return res1*res2;
    }
    case 11:
    {
        if(operands[0]->type()>11)    {   OutErr("\nUncorrect operand\n");   return 0;    }
        int res = -operands[0]->ex();
        return res;
    }
    case 12:
    {
        if(fail_check_type())
            return -1;
        int res1 = operands[0]->ex(), res2 = operands[1]->ex();
        return res1<res2;
    }
    case 13:
    {
        if(fail_check_type())
            return -1;
        int res1 = operands[0]->ex(), res2 = operands[1]->ex();
        return res1>res2;
    }
    case 14:
    {
        if(fail_check_type())
            return -1;
        int res1 = operands[0]->ex(), res2 = operands[1]->ex();
        return res1==res2;
    }
    case 15:
    {
        if(operands[0]->type()>10){   OutErr("\nUncorrect operand\n");  return -1;   }
        int res1 = operands[0]->ex();
        return res1;
    }
    case 16:
    {
        switch(operands[0]->type() - 9)
        {
        case 0:   {   int res = operands[0]->ex(); return res;      }
        case 1:         { int res = operands[0]->ex(); return res;       }
        case 2:    {  return 0;   }
        }
    }
    }
    return -1;
}

Expr::Expr(string act, vector<Node*> _operands):Node(),expr(table[act]),operands(_operands) {}

int Expr::fail_check_type()
{
    if(operands[0]->type()<9 || operands[1]->type()>10)  {  OutErr("\nUncorrect operands\n");  return 1;  }
    return 0;
}

int Expr::type()
{
    switch(expr)
    {
        case 0: return 9;
        case 1: return 10 ;
        case 3: return 11;
        case 16: return 11;
    }
    return operands[0]->type();
}


Call_func::Call_func(string _name):name(_name){}

int Call_func::type() {     return futype;   }

int Call_func::ex()
{
    if (pfunc.find(name)!=pfunc.end())        {    return pfunc.find(name)->second->ex();       }

    OutErr("\nFunc not found\n");        return -1;
}


