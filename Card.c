//Card.c File for Ass2
/* Initialise */
// '("perror" "printf")
// '("malloc" "free")
// Various prototypes

#include <stdio.h>
#include <stdlib.h>
#include "Card.h"


// Handles "malloc" errors
// If unable to correctly allocate memory, raise an error
// Else, we return successfully

int check_malloc (void *this_pointer) {
    if (this_pointer == NULL) {
        printf ("%s", "Unable to allocate memory to the heap. \n");
        perror ("malloc");
    };
    return 0;
}


// I couldn't find a way to add up the number of elements in the "enums",
// so I'll hard-code the constants here. You can find the "enums" in
// "card.h"

const int total_colors = 5;
const int total_suits = 5;
const int total_values = 16;


/* Mechanics */
 /* Makes it easier to tell how many of the same card exists in the
     * game. So we can have at most maybe two identical cards.
     *
     * card_id =
     *   total_values *total_suits *card_color +
     *   total_values *card_suit +
     *   card_value
     */
    // { RED BLUE GREEN ... }
    // { HEARTS DIAMONDS CLUBS ... }
    // { ZERO ONE TWO DRAW_TWO ... }

struct _card {
    color card_color;
    suit card_suit;
    value card_value;
};


// Create a new card.
// These values can only be set at creation time.
// The number should be between 0x0 and 0xF.
    // Before anything, check that the "number" is within[0 15]
    // Create a pointer to a "struct _card" and make room for the card
    // in the heap.
    // Make sure we allocate memory properly.
    // If successful, fill in the card properties, which include...
    // unique ID, suit, colour and value.
    // (*new_card).card_id =
    // total_values *total_suits *color +
    // total_values *suit +
    // value;
    // Now return it
Card newCard (value value, color color, suit suit) {
    Card new_card = malloc (sizeof (struct _card));
    check_malloc (new_card);
    (*new_card).card_color = color;
    (*new_card).card_suit = suit;
    (*new_card).card_value = value;
    return new_card;
}


// Destroy an existing card.
// CARD is a pointer to the heap, so we need to free the memory.
void destroyCard (Card card) {
    free (card);
}


// Get the card's suit (HEARTS, DIAMONDS, etc).
suit cardSuit (Card card) {
    return (*card).card_suit;
}


// Get the card's number (0x0 through 0xF).
value cardValue (Card card) {
    return (*card).card_value;
}


// Get the card's color (RED, BLUE, etc).
color cardColor (Card card) {
    return (*card).card_color;
}


/* 
int experiment_001
(void)
{

  Card card_one =
    newCard (THREE, BLUE, SPADES);

  printf
    ("%s \n%d %d %d %d \n\n",
      "Card_one. Identification, three, blue, spades.",
      (*card_one).card_id,
      (*card_one).card_color,
      (*card_one).card_suit,
      (*card_one).card_value);

  printf
    ("%s \n%d %d %d \n\n",
      "Card_one. Three, blue, spades.",
      (int) cardColor (card_one),
      (int) cardSuit (card_one),
      (int) cardValue (card_one));

  destroyCard
    (card_one);

  return 0;

}



int main
(int argc, char *argv[])
{

  experiment_001 ();

  return 0;

}
*/

