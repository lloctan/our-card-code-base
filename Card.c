


/* Initialise */

// '("perror" "printf")
#include <stdio.h>

// '("malloc" "free")
#include <stdlib.h>

// Various prototypes
#include "Card.h"

// Handle "malloc" errors
int assert_malloc (void *this_pointer)
{

    // If unable to correctly allocate memory, raise an error
    if (this_pointer == NULL) {

        printf
        ("%s",
        "Unable to allocate memory to the heap. \n");

        perror
        ("malloc");

    };

    // Else, we return successfully
    return 0;

}

// I couldn't find a way to add up the number of elements in the "enums",
// so I'll hard-code the constants here. You can find the "enums" in
// "card.h"
const int total_colors = 5;
const int total_suits = 5;
const int total_values = 16;





/* Mechanics */

struct _card
{

    /* Makes it easier to tell how many of the same card exists in the
     * game. So we can have at most maybe two identical cards.
     *
     * card_id =
     *   total_values *total_suits *card_color +
     *   total_values *card_suit +
     *   card_value
     */
    // int card_id;

    // { RED BLUE GREEN ... }
    color card_color;

    // { HEARTS DIAMONDS CLUBS ... }
    suit card_suit;

    // { ZERO ONE TWO DRAW_TWO ... }
    value card_value;

};



// Create a new card.
// These values can only be set at creation time.
// The number should be between 0x0 and 0xF.
Card newCard
(value value, color color, suit suit)
{

    // Before anything, check that the "number" is within[0 15]
    assert
    ((0 <= value) && (value < 16));

    // Create a pointer to a "struct _card" and make room for the card
    // in the heap.
    Card new_card =
    malloc (sizeof (struct _card));

    // Make sure we allocate memory properly.
    assert_malloc (new_card);

    // If successful, fill in the card properties, which include...
    // unique ID, suit, colour and value.
    // (*new_card).id =
    // total_values *total_suits *color +
    // total_values *suit +
    // value;

    (*new_card).color = color;
    (*new_card).suit = suit;
    (*new_card).value = value;

    // Now return it
    return new_card;

}



// Destroy an existing card.
void destroyCard
(Card card)
{

    // CARD is a pointer to the heap, so we need to free the memory.
    free (card);

}



// Get the card's suit (HEARTS, DIAMONDS, etc).
suit cardSuit
(Card card)
{

    return (*card).suit;

}



// Get the card's number (0x0 through 0xF).
value cardValue
(Card card)
{

    return (*card).value;

}



// Get the card's color (RED, BLUE, etc).
color cardColor
(Card card)
{

    return (*card).color;

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
      (*card_one).id,
      (*card_one).color,
      (*card_one).suit,
      (*card_one).value);

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

