#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *Author: Brad Warren
 *Id Number:184030907
 *Email: bawarren@iastate.edu
 */

struct node**  list;
struct var ** varList;

/*
 * Node constructor
 */
struct node{
    int number;
    int mark;
    struct node* dest;
};

/*
 * Var constructor
 */
struct var{
    struct node* dest;
};

/*
 * Adds node by number to the dest of a variable.
 */
struct var* addToVar(struct var* varr, int number){
    if(number == 0){
        struct node * temp = malloc(sizeof(struct node));
        int prev;
        temp = varr->dest;
        temp->mark = 0;
        temp = temp->dest;
        while(temp != NULL){
            if(temp->dest != NULL){
                if(prev == temp->dest->number){
                    break;
                }
            }
            temp->mark = 0;
            prev = temp->number;
            temp = temp->dest;
        }
        if(temp != NULL){
            temp->mark = 0;
        }
        varr ->dest = NULL;
    }else if(varr->dest != NULL){
        if(number != varr->dest->number){
            struct node * temp = malloc(sizeof(struct node));
            int prev;
            temp = varr->dest;
            temp->mark = 0;
            temp = temp->dest;
            while(temp != NULL){
                if(temp->dest != NULL){
                    if(prev == temp->dest->number){
                        break;
                    }
                }
                temp->mark = 0;
                prev = temp->number;
                temp = temp->dest;
            }
            if(temp != NULL){
                temp->mark = 0;
            }
        }
        varr->dest = list[number - 1];
    }else{
        list[number - 1]->number = number;
        list[number - 1]->mark++;
        struct node * temp = malloc(sizeof(struct node));
        int prev;
        temp = list[number -1];
        if(list[number - 1]->dest != NULL){
            while(temp != NULL){
                if(temp->dest != NULL){
                    if(prev == temp->dest->number){
                        break;
                    }
                }
                temp->mark = 1;
                prev = temp->number;
                temp = temp->dest;
            }
            if(temp != NULL){
                temp->mark = 1;
            }
        }
        varr->dest = list[number - 1];
    }

    return varr;
}

/*
 * Adds node to the dest of another node according to the number.
 */
struct node* addToNode(struct node* nodee, int number){
    if(number == 0){
        struct node * temp = malloc(sizeof(struct node));
        int prev;
        temp = nodee;
        if(temp->dest != NULL){
            temp = temp->dest;
            while(temp != NULL){
                if(temp->dest != NULL){
                    if(prev == temp->dest->number){
                        break;
                    }
                }
                temp->mark = 0;
                prev = temp->number;
                temp = temp->dest;
            }
            if(temp != NULL){
                temp->mark = 0;
            }
        }
        nodee->dest = NULL;
    }else if(nodee->dest != NULL){
        if(number != nodee->dest->number){
            struct node * temp = malloc(sizeof(struct node));
            temp = nodee->dest;
            temp->mark = 0;
            temp = temp->dest;
            while(temp != NULL){
                temp->mark = 0;
                temp = temp->dest;
            }
        }
        nodee->dest = list[number - 1];
        if(nodee->mark == 1){
            struct node* prev;
            struct node * temp = malloc(sizeof(struct node));
            temp = nodee->dest;
            temp->mark = 1;
            temp = temp->dest;
            while(temp != NULL){
                if(temp->dest != NULL) {
                    if (prev->number == temp->dest->number) {
                        break;
                    }
                }
                temp->mark = 1;
                prev = temp;
                temp = temp->dest;
            }
            if(temp != NULL){
                temp->dest->mark = 1;
            }
        }
    }else{
        list[number - 1]->number = number;
        if(nodee->mark == 1){
            struct node* prev;
            struct node * temp = malloc(sizeof(struct node));
            temp = nodee->dest;
            temp->mark = 1;
            temp = temp->dest;
            while(temp != NULL){
                if(temp->dest != NULL) {
                    if (prev->number == temp->dest->number) {
                        break;
                    }
                }
                temp->mark = 1;
                prev = temp;
                temp = temp->dest;
            }
            if(temp != NULL){
                temp->dest->mark = 1;
            }
        }
        nodee->dest = list[number - 1];
    }

    return nodee;
}

/*
 * Prints out list in the given format separated by reachable and unreachable
 */
void print(int n){
    int reach[n];
    int unreach[n];
    for(int i = 0; i < n; i++){
        unreach[i] = 0;
        reach[i] = 0;
    }
    int reachIdx = 0;
    int unreachIdx = 0;
    for(int i = 0; i < n; i++){
        if(list[i]->mark != 1){
            unreach[unreachIdx] = list[i]->number;
            unreachIdx++;
        }else{
            reach[reachIdx] = list[i]->number;
            reachIdx++;
        }
    }
    printf("Reachable: ");
    int i = reach[0];
    int count = 1;
    while(i != 0){
        printf("%d", i);
        i = reach[count];
        count++;
        if(i != 0){
            printf(", ");
        }
    }
    printf("\nUnreachable: ");
    i = unreach[0];
    count = 1;
    while(i != 0){
        printf("%d", i);
        i = unreach[count];
        count++;
        if(i != 0){
            printf(", ");
        }
    }
   printf("\n");
}

/*
 * If a number does not appear in one of the lists then we print it after the connected ones are printed.
 */
int main() {
    int nodeNumber;
    char fileLine[10];
    char* buff = fileLine;
    int size = 10;
    int number1, number2;

    varList = malloc(sizeof(struct var)* 9);
    for(int i = 0; i<9; i++){
        varList[i] = (struct var*) malloc(sizeof(struct var));
        varList[i]->dest = NULL;
    }

//    FILE* file;
//    file = fopen("C:\\Users\\sup3r\\coms327\\HW\\Hw6\\test_input.txt","r");

    //Gets number of nodes in from the first line of the file
    fgets(buff, size, stdin);
    nodeNumber = atoi(fileLine);
    list = malloc(sizeof(struct node)* nodeNumber);
    for(int i=0; i<nodeNumber; i++)
    {
        list[i] = (struct node*) malloc(sizeof(struct node));
        list[i]->number=i+1;
        list[i]->dest=NULL; //point to next struct instead of NULL
    }

    while(fgets(buff, size, stdin) != NULL){
        //Gets the numbers in the line
        number1 = 0;
        number2 = 0;
        char number[10], nextNumber[10];
        number[0] = 0;
        nextNumber[0] = 0;
        number[0] = fileLine[2];
        int i = 2, numberCount = 0, nextNumberCount = 0;
        while(fileLine[i] != '-'){
            number[numberCount] = fileLine[i];
            numberCount++;
            i++;
        }
        number1 =  atoi(number);
        strcpy(number, " ");
        int j = i + 2;

        while(fileLine[j] < 58 && fileLine[j] > 47){
            nextNumber[nextNumberCount] = fileLine[j];
            nextNumberCount++;
            j++;
        }
        number2 = atoi(nextNumber);
        strcpy(nextNumber, " ");

        //Adds node to be the dest of the node ahead
        if(fileLine[0] == 'L'){
            addToNode(list[number1 - 1], number2);
        }

        //Adds node to be the dest of the variable
        if(fileLine[0] == 'V'){
            if(number1 == 1){
                addToVar(varList[0], number2);
            }else if(number1 == 2){
                addToVar(varList[1], number2);
            }else if(number1 == 3){
                addToVar(varList[2], number2);
            }else if(number1 == 4){
                addToVar(varList[3], number2);
            }else if(number1 == 5){
                addToVar(varList[4], number2);
            }else if(number1 == 6){
                addToVar(varList[5], number2);
            }else if(number1 == 7){
                addToVar(varList[6], number2);
            }else if(number1 == 8){
                addToVar(varList[7], number2);
            }else if(number1 == 9){
                addToVar(varList[8], number2);
            }else{
                fprintf(stderr, "%d variable exists", number1);
                exit(1);
            }
        }
    }
    //Prints nodes in order from reachable to unreachable
    print(nodeNumber);
    return 0;
}
