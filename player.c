// This is a AI function, when playerValidMove is true than this function is called,
// the playerAction decideAction function is called, which should
    // play the move which should be another function
// Finshed 30/10/2017

// By:
// Patrick Chen <z5019251>
// Kelvin Lo <z5141516>

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

#include "Game.h"
#include "Game.c"
#include "Card.h"
#include "Card.c"
#include "player.h"

void playCard (Game game, int value);
void sayIt (Game game);

int main (int argc, char *argv[]) {

    decideMove (new_game);
    sayIt (new_game);
    
    move.action == END_TURN;

    return EXIT_SUCCESS;
}
// This functions pick a card for the player to "play"
// It prioritises playing cards of the value same value first
// Before playing cards of the same color
// Before playing cards of the same suit
playerMove decideMove (Game game) {
    int hand_size = handCardCount(new_game);
    int i = 0;
    Card previous_card = topDiscard (new_game);\
    // this is to make sure that only one card is played
    int card_played = 0;

    while (i < hand_size && card_played != 1) {
        Card check_Card = handCard (new_game, i) {
            if (((*check_Card).value) == ((*previous_card).value)) {
                if ((*check_Card).value == C) {
                    playCard (new_game, i);
                    i = 0;
                } else {
                    playCard (new_game, i);
                    card_played = 1;
                }
            } else if (((*check_Card).color) == ((*previous_card).color) {
                playCard (new_game, i);
                card_played = 1;
            } else if (((*check_Card).suit) == ((*previous_card).suit) {
                playCard (new_game, i);
                card_played = 1;
            }
        }
        i++;
    }
}


// this function draws a card from the player's hand and adds it as the top card to the discard pile
void playCard (Game game, int value) {

        // Adds the nth card from the player's hand to the top of the discard pile
        (*new_game).discard_pile =
        card_list_add
        ((*new_game).discard_pile), value,
        (* ((*new_game).player_hands[player]);

        // Remove the card from the player's hand
        (*new_game).player_hands[player] =
        card_list_remove
        ((*new_game).player_hands[player], i);
}

// this function is to say Uno/Duo/Trio when required
void sayIt (Game game) {
    
    int hand_size = handCardCount(new_game);
    
    if (hand_size == 3) {
        printf ("Trio\n");
    } else if (hand_size == 2) {
        printf ("Duo\n");
    } else if hand_size == 1) {
        printf ("Uno\n");
    }
}
