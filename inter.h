#ifndef INTER_H
#define INTER_H
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <stack>

using namespace std;

void OutErr(string _err);

class Node
{
public:
    Node(){}

    virtual int type() = 0;
    virtual int ex()=0;
    virtual void set_value(int a){}
    ~Node(){}
};

extern stack<Node*> dynamic;
extern stack<map<string,Node*>>  cur_data;
extern map<string, Node*> pfunc;
extern vector<vector<int> > xray;
extern const int SZ;
extern int execute_flag;

class Program
{
public:
    Program();
    ~Program();
};

//9+
class Signed: public Node
{
    int value;
    int is_const;
public:

    Signed(int _num, int _is_const);
    virtual int type()override ;
    virtual int ex() override ;
    virtual void set_value(int a) override;
    ~Signed(){}
};

//10+
class Unsigned: public Node
{
    int value;
    int is_const;
public:
    Unsigned(int _num, int _is_const );
    virtual int type()override;
    virtual int ex() override ;
    virtual void set_value(int a) override;
   ~Unsigned(){}
};

//11+
class Cell: public Node
{
    int loc;
    int is_const;
public:
    Cell(int _loc, int _is_const);
    Cell(const Cell& a);
    virtual int type() override;
    virtual int ex() override;
    virtual void set_value(int _loc) override;
    ~Cell(){}
};

//  9 / 10 / 11 / 12 13 / 14 / 15 +
class Adress:public Node
{
    string name;
    vector<Node*>ind;

public:
    Adress(string _name);
    Adress(string _name, Node* expr1, Node* expr2);
    virtual int type() override;
    virtual int ex() override;
    virtual void set_value(int a) override;
    ~Adress(){}
};

// 12 / 13 / 14 +
class Matrix:public Node
{
    int height;
    int width;
    int mtype;
    int is_const;
    vector<vector<Node*>> matrix;
public:
    Matrix(int _height, int _width, int _mtype, int _is_const);
    virtual int type()override;
    virtual int ex() override;
    virtual void set_value(int a) override;
    ~Matrix(){}
};


// 9 / 10 +
class Func:public Node
{
    list<Node *>body;
    list<pair<string, int>>args;
public:
    Func(list<pair<string,int>>_args, list<Node*>_body);
    virtual int type() override   ;
    virtual int ex() override;
    ~Func(){}
};

//2+
class While: public Node
{
    Node *condition;
    list<Node*>body;
public:
    While(Node *_condition, list<Node*>_body);
    virtual int type()override;
    virtual int ex() override;
    ~While(){}
};

//3+
class If: public Node
{
    Node * condition;
    list<Node*>body;
public:
    If(Node *_condition, list<Node*> _body);
    virtual int type()override;
    virtual int ex() override;
    ~If(){}
};

//4+
class Action: public Node
{
    int dir;
public:
    Action(int _dir = 1):dir(_dir){}
    virtual int type()override ;
    virtual int ex() override;
    int check_go();
    ~Action(){}
};

//5 +
class Assig: public Node{
    Node *adress;
    Node *expr;
public:
    Assig(Node *_adress, Node*_expr);
    virtual int type()override;
    virtual int ex() override;
    ~Assig(){}
};

//6 +
class Variable: public Node
{
    string name;
    int vtype;
    int is_const;
    Node *expr;
    vector<int>size;

public:
    Variable (int _is_const, string _name, int _vtype, int size1, int size) ;

    Variable (int _is_const, int _vtype, string _name, Node*_expr);

    Variable (string _name, int _vtype, Node*_expr) ;

    virtual int type()override;
    virtual int ex() override;
    ~Variable(){}
};

// 9 / 10 /11 / 12 / 13 / 14 +
class Expr: public Node
{
    vector<Node*> operands;
    int expr;
public:
     Expr(string act, vector<Node*> _operands);
    int fail_check_type();
    virtual int type() override;
    virtual int ex()override;
    ~Expr(){}
};

//8 +
class Lexpr:public Node
{
    vector<Node*>op;
    int act;
public:

    Lexpr(int _act, vector<Node*>_op);
    virtual int type()override;
    virtual int ex() override;

    ~Lexpr(){}

};

class Call_func: public Node
{
    string name;
public:
    Call_func(string _name);
    virtual int type()override;
    virtual int ex()override;
    ~Call_func(){}
};


    extern Node* location;
    extern Node* finish;
    extern Node* crash;
    void Funcc(string p);
    void FArg(string p);
    void NConst();
    void YConst();
    void Celll();
    void Signedd();
    void Unsignedd();
    void Assigg();
    void Whilee();
    void Iff();
    void Gdir(int dir);
    void vArr(string p,int unum1,int unum2,int init_flag);
    void Var(string p);
    void Cond();
    void Queryy();
    void Lop(int oper);
    void Lbr();
    void Lwhere(int dr);
    void EName(string p);
    void EArrr(string p);
    void IName(string p);
    void INum(int p);
    void Iscnv(int p);
    void Xrayy();
    void Call(string p);
    void Num(int p);
    void Unum(int p);
    void Pl();
    void Min();
    void Dv();
    void Ml();
    void Imin();
    void Lss();
    void Mr();
    void Eq();
    void Br();
    void AName(string p);
    void ANum(int p);
    void AUNum(int p);
    void Newbody();
    void starts();
    void do_xray();
    void get_map();
#endif
