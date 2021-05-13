#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 *Author: Brad Warren
 *Id Number:184030907
 *Email: bawarren@iastate.edu
 *
 *If used with stdnin and not an input file need to
 *indicate end of file with ctrl+d
 */


// Used to determine if the head is null and if head->next is null
int node = 0;
// Used to Track the head of the linked list
struct node* head;

/*
 * Node of the linked list
 * 32 characters long or shorter may exit if words are too long
 */
struct node {
    char string[32];
    unsigned int count;
    struct node* next;

};

/*
 * Adds a word string to a list. If head is NULL, word is added to the head. If head is not NULL and head->next is NULL
 * word is added to head->next. Else word is added to end of linked list unless the word matches one found in the linked list.
 * If that is the case than the words count is incremented by 1.
 */
struct node* addToList(struct node* Listptr, char* word) {
    char *targetWord;
    targetWord = malloc(sizeof(char)*32);
    struct node *temp, *next;
    size_t stringLength = strlen(word);
    temp = malloc(sizeof(struct node));

    char c = 1;
    //Adds to word to head if head is NULL and returns Listptr
    if(node == 0){
        for(int i = 0; i< (int) stringLength; i++){
            c = *(word + i);
            head->string[i] = c;
        }
        node++;
        head->count++;
        return Listptr;
    }else{ //Adds to targetword if head is not NULL
        for(int i = 0; i< (int) stringLength; i++){
            c = *(word + i);
            *(targetWord + i) = c;        }
    }
    //If head->next is NULL then we add targetword to head->next
    if (node == 1) {
        //If head->string == targetword we increment count
        if (strcmp(head->string,targetWord) == 0) {
            head->count++;
            node++;
            free(targetWord);
            return Listptr;
        }else{
            head->next = temp;
            temp->count++;
            strcpy(temp->string, targetWord);
            node++;
        }
    }else{ //Adding targetword to the end of the linked list
        next = head;
        while(next->next != NULL){
            //If next->string == targetword then we increment count
            if(strcmp(next->string, targetWord) == 0){
                next->count++;
                free(targetWord);
                return Listptr;
            }else{
                next = next->next;
            }
        }
        //If next->string == targetword then we increment count
        if(strcmp(next->string, targetWord) == 0) {
            next->count++;
            free(targetWord);
            return Listptr;
        }else{ //Add targetword to the end of linked list
            next->next = temp;
            temp->count++;
            strcpy(temp->string, targetWord);
        }
    }
    free(targetWord);
    return Listptr;
}

/*
 * Reads from standard input, splits the input stream into words,
 * and counts the number of occurrences of each word.
 */
int main() {
    char fileLine[150];
    char *buff = fileLine;
    head = malloc(sizeof(struct node));

    int letter;
    char use;
    char last;
    //Gets file information one line at a time
    letter = getc(stdin);
    use = (char) letter;
    int wordIndex = 0;
    char *word;
    word = malloc(sizeof(char) * 32);
    while (letter != EOF) {
        if(wordIndex == 0){
            word = malloc(sizeof(char) * 32);
            *(word + 0) = '\0';
        }
        //Checks for the beginning of a word
        if ((use <= 122 && use >= 97) || (use <= 90 && use >= 65)) {
            *(word + wordIndex) = tolower(use);
            wordIndex++;
        }
        //Checks if word ended
        if (word[0] != '\0' && !((use <= 122 && use >= 97) || (use <= 90 && use >= 65))) {
            //Adds end value so no garbage appears
            *(word + wordIndex) = '\0';
            wordIndex = 0;
            addToList(head, word);
        }
        last = use;
        letter = getc(stdin);
        use = (char) letter;
    }

    //For case when EOF is right after a word
    if(word[0] != '\0' && letter == EOF && ((last <= 122 && last >= 97) || (last <= 90 && last >= 65))){
        *(word + wordIndex) = tolower(last);
        addToList(head, word); 
    }

    //Prints out head of the linked list
    printf("\n%d ", head->count);
    printf("%s\n", head->string);
    struct node *t = head;
    t = t->next;
    //Prints out rest of the values in the linked list
    while(t != NULL){
        printf("%d ", t->count);
        printf("%s\n", t->string);
        t = t->next;
    }
    return 0;
}

