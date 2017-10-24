// Testing each function of test game.c

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "Card.h"
#include "Game.h"

#define MAX_DECK 100
#define MIN_DECK 30

void num (void);
void rngDeck (Game game);

int main (int argc, char *argv[]) {

    printf ("This is a test for the Game ADT\n");

    num ();

    printf ("All tests passed\n");

    return EXIT_SUCCESS;
}

void num (void) {

    value values[50];
    suit suits[50];
    color colors[50];

    int i = 0;

    while (i < 50) {
        values [i] = rand() % 16;
        suits[i] = HEARTS;
        colors[i] = RED;
        i++;
    }

    Game game = newgame(50, values, colors, suits);

    assert ((game->cards_in_deck) == 50);
}


// this function generates a random deck to be used
void rngDeck () {
    // generating a random amount of cards
    int i = 0;
    int count = (rand () % (MAX_DECK + 1 - MIN_DECK) + MIN_DECK);
    
    while (i < count) {
        values [i] = (rand () % (15));
        // Hearts for now but will change once I implement a function to convert RNG numbers into string
        suits [i] = HEARTS;
        colors [i] = BLUE;
        i++;
    }
}
