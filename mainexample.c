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
        int message[5] = {72, 101, 108, 108, 111}; // "Hello"
        int i = 0;
        while (i < 5) {
                display(message[i]);
                i = i + 1;
        }
        
        display(10); // New line

        return i;
}
