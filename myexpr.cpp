#include <iostream>

/**
 *Author: Brad Warren
 *Id Number:184030907
 *Email: bawarren@iastate.edu
 *
 * Did not do the extra credit
 */

/**
 * Abstract class expr
 */
class expr {
public:
    expr(){}
    ~expr(){}
    virtual long compute() const{}
};

/**
 * Concrete class literal, derived from expr
 */
class literal : expr {
public:
    long value;
    literal(float x) : expr(){
        value = (long) x;
    }
    ~literal(){}
    virtual long compute() const{
        return value;
    }
};

/**
 * Concrete class variable, derived from expr
 */
class variable : expr{
public:
    long *value;
    variable(float x) : expr(){
        *value = (long) x;
    }
    ~variable(){}
    virtual long compute() const{
        return *value;
    }
};

/**
 * Concrete class binary_op, derived from expr. Used to calculate an expression by
 * left and right side with the operator in the middle.
 */
class binary_op : expr{
public:
    char op;
    expr* left = new expr();
    expr* right = new expr();

    //Sets values for each expression and operator
    binary_op(expr *first, expr *second, char opr){
        op = opr;
        left = first;
        right = second;
    }
    ~binary_op(){}
    virtual long compute() const{

        //Gives values returned by the corresponding compute method
        int l = (int) left->compute();
        int r = (int) right->compute();
        int total = 0;

        //Checks for what operator is used
        if(op == '+'){
            total = l + r;
        }else if(op == '-'){
            total = l - r;
        }else if(op == '*'){
            total = l * r;
        }else if(op == '/'){
            if(l == 0){
                std::cerr << "cannot divide from 0 \n";
                exit(1);
            }else{
                total = l / r;
            }
        }else if(op == '%'){
            if(l == 0){
                std::cerr << "cannot divide from 0 \n";
                exit(1);
            }else{
                total = l % r;
            }
        }
        return total;
    }

};


/**
 * Get values of x first. Copy rest of postfix into array of expr. Split array up into stack
 * if varible or number push. If variable is an op call construct new binary op of
 * last two on stack and call binary_op.compute(). Do this until one value remains.
 * @param argc
 * @param argv
 * @return
 */
//Did not do extra credit
int main(int argc, char **argv){
    using namespace std;

    int x1,x2;
}