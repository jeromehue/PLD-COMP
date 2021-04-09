#include <string.h>
#include <stdio.h>

#define SOMETHING 5

/**
 * Function that displays a character
 * onto the screen.
 */
void display(char c) {
        putchar(c);
}

// Our main function
int main() {
        
        int messageSize = 13;
        char message[13] = 
        {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd' ,'!'};
        char message2[5] = {'W', 'o' , 'r', 'l', 'd'};
                
        int i = 0;
        while (i < messageSize) {
                display(message[i]);
                i = i + 1;
        }
        
        display(10); // New line

        return i;
}
