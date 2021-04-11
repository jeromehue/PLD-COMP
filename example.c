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
int  main() {
        int length = 13;
        char message[13] = {'H', 'e', 'l', 'l', 'o', ',',
                            ' ', 'W', 'o', 'r', 'l', 'd', '!'};
        int tab[3] = {1,2,3};
        int i = 0;
        while (i < length) {
                display(message[i]);
                i = i + 1;
        }

        // Add a new line
        display(10);
        
        return i;
}
