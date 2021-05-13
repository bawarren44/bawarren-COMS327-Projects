#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*
 * Author: Brad Warren
 * Id Number: 184030907
 * Email: bawarren@iastate.edu
 *
 * Need to compile with command
 * gcc fib.c -o fib -lm
 * in order to compile math.h
 */

//Global variable for keeping track of fibinacci number
int fibNumber = 0;

/*
 * Write a function to initialize an arbitrary-precision integer from an unsigned integer.
 */
void initialize(char digits[], unsigned numdigits, unsigned value){
    int count = 0;
    for(int i = 0; i < numdigits; i++){
        digits[i] = '0';
    }
    digits[50] = '\0';

    //Traverse through digits array while adding each digit of value to digits
    for(int i = numdigits - 1; i >= 0; i--) {
        if (i == 0) {
            //Gives digit at first character of value
            digits[count] = (value % 10) + 48;
        } else {
            //Gives digit at each character of value
            digits[count] = (fmod((value / pow(10.0, (double) i)), 10.0)) + 48;
        }
        count++;
    }
}

/*
 * Write a function to add two arbitrary-precision integers, and store the result into a third arbitrary-precision integer.
 * All three integers have the same number of digits. The function should return 1 if an overflow occurred, and 0 otherwise.
 */
int add(char result[], char num1[], char num2[], unsigned numdigits){
    //Value that is added if addition goes over 10
    int carryOver = 0;

        for(int i = numdigits - 1; i >= 0; i--){
            //Basic addition and conversion between char and int of numbers
            int one = (num1[i]) - 48;
            int two = (num2[i]) - 48;
            int temp = one + two + carryOver;
            carryOver = 0;

            //Calculates if number is over 10
            if(temp >= 10){
                result[i] = (temp - 10) + 48;
                carryOver = 1;
            }else{
                result[i] = (temp) + 48;
            }
        }

        char temp[numdigits + 1];
        temp[0] = '1';

        //Overflow if 1, else 0
        if(carryOver == 1){
            //Move carryover in manually if number of digits is to be increased
            for(int i = 1; i < numdigits + 1; i++){
                temp[i] = result[i - 1];
            }
            strncpy(result, temp, numdigits + 1);
            return 1;
        }else{
            return 0;
        }
}

/*
 * Write a function to write an arbitrary-precision integer to standard output, with commas as the thousands separator.
 */
void print_int(char digits[], unsigned numdigits){
    int count = 0;
    int index = numdigits -1;
    //Temp char array with number of digits + commas to be added
    char* temp = malloc(numdigits + (numdigits/3) + 1);

    if(numdigits % 3 != 0){
        for(int i = (numdigits + (numdigits/3)) -1; i >= 0; i--){
            //Add commas between every 3 digits of char array
            if(count != 0 && count % 3 == 0 && digits[index] != '\0'){
                *(temp + i) = ',';
                count = 0;
            }else {
                //Add digit to temp
                *(temp + i) = digits[index];
                index--;
                count++;
            }
        }
    }else{ //For case when numdigits % 3 == 0
        for(int i = (numdigits + (numdigits/3)) - 2; i >= 0; i--){
            //Add commas between every 3 digits of char array
            if(count != 0 && count % 3 == 0 && digits[index] != '\0'){
                *(temp + i) = ',';
                count = 0;
            }else {
                //Add digit to temp
                *(temp + i) = digits[index];
                index--;
                count++;
            }
        }
        *(temp + (numdigits + (numdigits/3)) - 1) = 0;
    }
    temp[(numdigits + (numdigits/3))] = 0;

    //Prints out number in correct format
    printf("F(");
    printf("%d", fibNumber);
    printf("): ");
    printf(temp);
    printf("\n");
    free(temp);
    fibNumber++;
}

/*
 * Using the above functions and any other helper functions you need,
 * write a main() function that uses three 50-digit integers to compute and display the Fibonacci sequence
 */
int main(){
    //Initializing and printing beginning values
    int numdigits = 1;
    char dig[50] = "";
    char digits[50] = "";
    char result[50] = "";
    initialize(dig, numdigits, 0);
    print_int(dig, numdigits);
    initialize(digits, numdigits, 1);
    print_int(digits, numdigits);

    //Endless loop to do the fibanacci sequence. Stopped when numdigits == 51
    while(1){
        int flag = 0;
        if(add(result, dig, digits, numdigits) == 1){
            numdigits++;
            flag = 1;
        }

        print_int(result, numdigits);
        //Copy array digits array into dig and result array into digits array
        strncpy(dig, digits, numdigits);
        strncpy(digits, result, numdigits);

        //Add 0 before value in dig to have same number of digits digits array
        if(flag == 1) {
            char temp[numdigits];
            temp[0] = '0';
            for (int i = 1; i < numdigits; i++) {
                temp[i] = dig[i - 1];
            }
            strncpy(dig, temp, numdigits);
        }
        //Ends loop when numdigits equals 50
        if(numdigits == 50){
            break;
        }
    }
}