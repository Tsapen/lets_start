#include "inter.h"
#include "inter.tab.h"
#include <list>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <typeinfo>
using namespace std;

int arg_flag = 1;
stack<pair<int, int>>ftype;//1 - type  , 2  - is_const
list<pair<string, int>>farg;//list for functions arguments
stack<list<Node*>>fbody;//code in current area of visibility
stack<Node*>cur_op;//stack for operands in current operation
stack<Node*>fcondition;//stack for conditions in WHILE and IF
stack<string>query;//query stack
stack<Node*>findex;//buffer for indexes of current matrix
vector<Node*>fcall={nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};//vector for generating expression from calling function
int counter = 0;
stack<list<Node*>*> bodies;
stack <int>queries;
int execute_flag = 1;

    void Funcc(string p)
    {
        cout<<"\n\nFuncc("<<p;
        fbody.top().push_back(cur_op.top());
        cur_op.pop();
        Func *fnc = new Func(farg,fbody.top());
        fbody.pop();

        farg.clear();
        dynamic.push(fnc);
        pfunc.insert(make_pair(p,fnc));
        cout<<")pop body, clear arg\n\n";
    }
    void FArg(string p)
    {
        farg.push_back(make_pair(p,ftype.top().first));
        cout<<" "<<p<<"("<<ftype.top().first<<") -farg\n";
        ftype.pop();
    }
    void Newbody()
    {
        list<Node*>*newlist = new list<Node*>;
        bodies.push(newlist);
        fbody.push(*newlist);
        cout<<"\nEnd args; Newbody\n";
    }
    void NConst()    {   ftype.push(make_pair(0,0));    }
    void YConst()            {   ftype.push(make_pair(0,1));    }
    void Celll()                    {   ftype.top().first = 11;    }
    void Signedd()    {  ftype.top().first = 9;    }
    void Unsignedd()         {  ftype.top().first = 10;   }
    void Assigg()
    {
        Node *expr = cur_op.top();
        cur_op.pop();
        Node *adr = cur_op.top();
        cur_op.pop();
        Assig *asg = new Assig(adr,expr);
        dynamic.push(asg);
        (fbody.top()).push_back(asg);
        counter = 0;
        cout<<"(Assig)\n";
    }
    void Whilee()
    {
        While *whl = new While(fcondition.top(),fbody.top());
        fcondition.pop();
        fbody.pop();
        dynamic.push(whl);
        fbody.top().push_back(whl);
        cout<<"(While)\n";
    }
    void Iff()
    {
        If *_if = new If(fcondition.top(),fbody.top());
        fcondition.pop();
        fbody.pop();
        dynamic.push(_if);
        fbody.top().push_back(_if);
        cout<<"(If)\n";
    }
    void vArr(string p,int unum1,int unum2,int init_flag)
    {
        int x = ftype.top().second;
        if(init_flag)
            x = 2;
        Variable *var = new Variable(x,p,ftype.top().first,unum1,unum2);
        ftype.pop();
        dynamic.push(var);
        fbody.top().push_back(var);
        cout<<p<<"["<<unum1<<","<<unum2<<"],"<<x<<"(Mvar)\n";
    }
    void Var(string p)
    {
        Variable *var = new Variable(ftype.top().second,ftype.top().first,p,cur_op.top());
        ftype.pop();
        cur_op.pop();
        dynamic.push(var);
        fbody.top().push_back(var);
        counter = 0;
        cout<<p<<"(var)\n";
    }
    void Cond()
    {
        fcondition.push(cur_op.top());
        cur_op.pop();
        list<Node*>*newlist = new list<Node*>;
        fbody.push(*newlist);
        cout<<"(cond)\n";
    }
    void Gdir(int dir)
    {
        Action *act = new Action(dir);
        fbody.top().push_back(act);
        dynamic.push(act);
        cout<<"Go "<<dir<<endl;
    }
    void Queryy()
    {
        int x = queries.top();
        Lexpr *lxp = new Lexpr(x,{cur_op.top()});
        queries.pop();
        cur_op.top()=lxp;
        dynamic.push(lxp);
        cout<<x<<"(query)";
    }
    void Lop(int oper)
    {
        Node *sec_op = cur_op.top();
        cur_op.pop();
        Lexpr *lxp = new Lexpr(oper,{cur_op.top(),sec_op});
        cur_op.top() = lxp;
        dynamic.push(lxp);
        cout<<oper<<"(Lop)";
    }
    void Lbr()
    {
        Lexpr *lxp = new Lexpr(11,{cur_op.top()});
        cur_op.top()=lxp;
        dynamic.push(lxp);
        cout<<" l() ";
    }
    void Lwhere(int dr)       {   queries.push(dr);    }
    void EName(string p)
    {
        Adress *newadr = new Adress(p);
        dynamic.push(newadr);
        cur_op.push(newadr);
        cout<<p<<"(adr) ";
    }
    void EArrr(string p)
    {
        Node* x = findex.top();
        findex.pop();
        Node* y = findex.top();
        findex.pop();
        Adress *newadr = new Adress(p, y, x);
        dynamic.push(newadr);
        cur_op.push(newadr);
        cout<<p<<"(Madr) ";
    }
    void IName(string p)
    {
        Adress *newadr = new Adress(p);
        dynamic.push(newadr);
        findex.push(newadr);
        cout<<p<<"(index) ";
    }
    void INum(int p)
    {
        Unsigned *newadr = new Unsigned(p,1);
        dynamic.push(newadr);
        findex.push(newadr);
        cout<<p<<"(index) ";
    }


    void Iscnv(int p)
    {
        if(p)
        {
            string com;
            switch(ftype.top().first)
            {
                case 9: {com = "Conv_s"; break;}
                case 10: {com = "Conv_us"; break;}
                case 11: {com = "Conv_cell"; break;}
            }
            ftype.pop();
            Expr *expre = new Expr(com,{cur_op.top()});
            cur_op.pop();
            cur_op.push(expre);
            dynamic.push(expre);
        }
    }
    void Xrayy()
    {
        Expr *expre = new Expr("Xray",{});
        cur_op.push(expre);
        dynamic.push(expre);
        cout<<"xray ";
    }
    void Call(string p)
    {
        fcall[0] = new Call_func(p);
        dynamic.push(fcall[0]);
        Expr *cff = new Expr("Call", fcall);
        fcall={nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};

        counter = 0;
        dynamic.push(cff);
        cur_op.push(cff);
        cout<<"call "<<p<<" ";
    }
    void Num(int p)
    {
        Signed *newadr = new Signed(p,1);
        dynamic.push(newadr);
        cur_op.push(newadr);
        cout<<p<<"s(E) ";
    }
    void Unum(int p)
    {
        Unsigned *newadr = new Unsigned(p,1);
        dynamic.push(newadr);
        cur_op.push(newadr);
        cout<<p<<"u(E) ";
    }
    void Pl()
    {
       Node* scn = cur_op.top();
        cur_op.pop();
        Expr *xpr = new Expr("+",{cur_op.top(),scn});
        cur_op.pop();
        dynamic.push(xpr);
        cur_op.push(xpr);
        cout<<"E+ ";
    }
    void Min()
    {
        Node* scn = cur_op.top();
         cur_op.pop();
         Expr *xpr = new Expr("-",{cur_op.top(),scn});
         cur_op.pop();
         dynamic.push(xpr);
         cur_op.push(xpr);
         cout<<"E- ";
    }
    void Dv()
    {
        Node* scn = cur_op.top();
         cur_op.pop();
         Signed * s = dynamic_cast<Signed*>(scn);
         Unsigned * us = dynamic_cast<Unsigned*>(scn);

         if(s&&(!s->ex()) || us&&(!us->ex()))
               execute_flag = 0;

         Expr *xpr = new Expr("/",{cur_op.top(),scn});
         cur_op.pop();
         dynamic.push(xpr);
         cur_op.push(xpr);
         cout<<"E/ ";
    }
    void Ml()
    {
        Node* scn = cur_op.top();
         cur_op.pop();
         Expr *xpr = new Expr("*",{cur_op.top(),scn});
         cur_op.pop();
         dynamic.push(xpr);
         cur_op.push(xpr);
         cout<<"E* ";
    }
    void Imin()
    {
         Expr *xpr = new Expr("i-",{cur_op.top()});
         cur_op.pop();
         dynamic.push(xpr);
         cur_op.push(xpr);
         cout<<"Ei- ";
    }
    void Lss()
    {
        Node* scn = cur_op.top();
         cur_op.pop();
         Expr *xpr = new Expr("<",{cur_op.top(),scn});
         cur_op.pop();
         dynamic.push(xpr);
         cur_op.push(xpr);
         cout<<"E< ";
    }
    void Mr()
    {
        Node* scn = cur_op.top();
         cur_op.pop();
         Expr *xpr = new Expr(">",{cur_op.top(),scn});
         cur_op.pop();
         dynamic.push(xpr);
         cur_op.push(xpr);
         cout<<"E> ";
    }
    void Eq()
    {
        Node* scn = cur_op.top();
         cur_op.pop();
         Expr *xpr = new Expr("=",{cur_op.top(),scn});
         cur_op.pop();
         dynamic.push(xpr);
         cur_op.push(xpr);
         cout<<"E= ";
    }
    void Br()
    {
         Expr *xpr = new Expr(")",{cur_op.top()});
         cur_op.pop();
         dynamic.push(xpr);
         cur_op.push(xpr);
         cout<<"E() ";
    }

    void AName(string p)
    {
        cout<<p<<"(An)(counter "<<counter<<") " ;
        fcall[++counter] = new Adress(p);
        dynamic.push(fcall[counter]);

    }
    void ANum(int p)
    {
        cout<<p<<"(A)(counter "<<counter<<") " ;
        fcall[++counter] = new Signed(p,1);
        dynamic.push(fcall[counter]);

    }
    void AUNum(int p)
    {
        cout<<p<<"u(A)(counter "<<counter<<") " ;
        fcall[++counter] = new Unsigned(p,1);
        dynamic.push(fcall[counter]);
    }
