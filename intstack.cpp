#include <iostream>
#include <cstring>
/**
 *Author: Brad Warren
 *Id Number:184030907
 *Email: bawarren@iastate.edu
 *
 * Compile with "g++ intstack.cpp -o [input name here]"
 * input whatever you want in the square brackets above
 */

using namespace std;
/*
 * Class that implements and integer stack
 */
class intstack{

//Private section containing struct for linked list implementation
private:
    struct node{
        int value;
        int initialized = 0;
        struct node* next;
    };

public:
    struct node *head;
    //Constructor
    intstack(){ head = (struct node*) malloc(sizeof(node));}
    //Destructor
    ~intstack(){delete [] head;}

    //Copy constructor and copy assignment operator to disallow copying
    intstack(struct node* x): head(x){}
    intstack(const intstack& temp_obj) = delete;
    intstack& operator=(const intstack& temp_obj) = delete;

    //Conversion operator
    operator bool() {
        return head->initialized == 1;
    }

    //Push method
    void push(int n){
        if(head == nullptr){
            head->value = n;
            head->initialized = 1;
            head->next = nullptr;
        }else{
            node *tmp = new node;
            tmp->value = n;
            tmp->initialized = 1;
            node *temp = head;
            head = tmp;
            head->next = temp;
        }
    }

    //Pop method
    int pop(){
        int n = 0;
        if(head == nullptr || head->next == nullptr){
            std::cerr << "Error: Invalid input, pop() cannot be performed when stack is empty\n";
            exit(1);
        }else{
            node *tmp = head->next;
            n = head->value;
            head = tmp;
        }
        return n;
    }
};

/*
 * postfix calculator, where the expression to be calculated is passed as
 * command-line arguments with whitespace separating each part of the expression.
 */
int main(int argc, char** argv) {
    char zero = 0;
    char *ptr;
    intstack *n = new intstack();
    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "0") == 0) { //If value is zero
            zero = 1;
        }
        int value = (int) strtol(argv[i], &ptr, 10);
        if (value != 0) { //If value is a non-zero integer
            n->push(value);
        } else if (zero == 1) { //If value == 0
            n->push(value);
            zero = 0;
        }else{ //If value is not a integer (pop twice and do calculation then push)
            if(strcmp(argv[i], "+") == 0){
                int one = n->pop();
                int two = n->pop();
                n->push(two + one);
            }else if(strcmp(argv[i], "-") == 0){
                int one = n->pop();
                int two = n->pop();
                n->push(two - one);
            }else if(strcmp(argv[i], "*") == 0){
                int one = n->pop();
                int two = n->pop();
                n->push(two * one);
            }else if(strcmp(argv[i], "/") == 0){
                int one = n->pop();
                int two = n->pop();
                if(one == 0){
                    std::cerr << "Error: Cannot divide by 0\n";
                    exit(1);
                }
                n->push(two / one);
            }else if(strcmp(argv[i], "%") == 0){
                int one = n->pop();
                int two = n->pop();
                if(one == 0){
                    std::cerr << "Error: Cannot mod by 0\n";
                    exit(1);
                }
                n->push(two % one);
            }
	}
    }

    //Displays correct output according to results
    if(*n){
        if(n->head->next->initialized == 1){
            std::cerr << "Error: Invalid input, not enough operators, too many integers\n";
            exit(1);
        }else{
        //Prints out end result of postfix expression
            cout << n->head->value << "\n";
	}
    }else {
        std::cerr << "Error: Invalid input, no number input\n";
    }

    return 0;
}
