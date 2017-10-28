


/* Initialise */


// '("perror" "printf") 
#include <stdio.h>

// '("malloc" "free")
#include <stdlib.h>

// '("errx")
#include <err.h>

// '("assert") 
#include <assert.h>

// Various complete functions
#include "Card.c"

// Various prototpyes
#include "Game.h"



#ifndef card_c

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

#endif



// Handle "calloc" errors
int assert_calloc
(void *this_pointer)
{

    // If unable to correctly allocate memory, raise an error
    if (this_pointer == NULL) {

        printf
        ("%s",
        "Unable to allocate memory to the heap. \n");

        perror
        ("calloc");

    };

    // Else, we return successfully
    return 0;

}





/* Miscellane */

// This is temporary, so that "testGame.c" runs
#define cards_dupe 52

// At the beginning of a game, this is the initial number
// of cards to distribute to players
#define initial_draw 7




/* Mechanics */

// This is the "struct" which forms the list of cards, like for the deck,
// discard pile and players' hands
struct card_list
{

    // The card itself
    Card card;

    // The next card
    struct card_list *next;

};


// THis is the "struct" which forms the move history
struct move_list
{

    // The move itself
    playerMove move;

    // The next move
    struct move_list *next;

};



// THis is the "struct" which contains pointers to the first move of
// that turn. So, each element here refers to a link-list of moves
struct turn_list
{

    // POinter to the first move of that turn
    struct move_list *moves;

    // The next turn
    struct turn_list *next;

};



// This "struct" tracks one distinct card and the number of the same card
// which currently exists in the game.
struct card_tracker
{

    // Pointer to the card
    Card card;

    // Number of duplicates
    int dupes;

};



// This is the "struct" which maintains all variables during a game
struct _game
{

    /* An array of cards in the game. If the game has at most 200
     * different cards, the array will have 200 elements. Each element
     * contains a "struct" with a unique card and the number of duplicates
     * for that card.
     *
     * cards_in_game
     * v
     * [id-000      id-001       id-002       id-003      id_004   ...]
     *   v           v            v            v           v
     *   card-000    (null)     card-002     card-003    (null)
     *   2           0            1            2           0
     *
     */
    struct card_tracker *cards_in_game;

    // Current direction
    direction direction;

    /* In initial deck, the number of cards in the deck initially.
     * This keep track of the number of all cards, the number for a
     * particular colour, suit or value.
     *
     * cards_initially
     * v
     * [ini-deck-len      color-len      suit-len    value-len]
     *   v                 v              v             v
     *  [400]
     *                    [81 69 94 88 68]
     *
     *                                   [82 90 74 82 72]
     *
     *                                                 [32 21 ...]
     */
    int **cards_initially;

    /* In case of "DRAW_TWO", this accumulates the number of cards a
     * player has to pick up. So, if two people play consecutive
     * "DRAW_TWO" cards, this value becomes 4. Once a person picks up
     * cards, this reverts to 0.
     */
    int cards_to_draw;

    // Number of distinct cards which exist and need to free later
    int cards_distinct;

    // Deck size plus cards in players' hands
    int cards_in_total;

    // Cards left in the deck
    int cards_in_deck;

    // Maximum number of identical cards possible
    int cards_max_dupe;

    /* History of every turn which occurs. Each turn contains a variable
     * which points to a list of moves. This way, we can find out on the
     * m-th turn what the n-th move was. We'll implement it as follows:
     *
     * Note, the most recent turn is at the front of the list. This is
     * strategic; so that our AI scans the latest move as early as
     * possible.
     *
     * turn_history
     * v
     * { turn-3 } >  { turn-2 } >  { turn-1 } >  { turn-0 }
     *   v             v             v             v
     *   { move-0 }    { move-0 }    { move-0 }    { move-0 }
     *     v             v             v             v
     *     { move-1 }    { move-1 }    { move-1 }    { move-1 }
     *       v             v             v
     *       { move-2 }    { move-2 }    { move-2 }
     *         v                           v
     *         { move-3 }                  { move-3 }
     *           v
     *           { move-4 }
     */
    struct turn_list *turn_history;


    /* Originally, we would have TURN_HISTORY to figure out
     * what cards are in the discard pile. But this would take
     * toooo much time and effort. So here we are, a variable
     * we dedicate, DISCARD_PILE. And a deck.
     */
    struct card_list *discard_pile;
    struct card_list *deck;

    /* Keep track of the cards in each players' hands. Each element
     * represents one player, and contains a link-list of pointers to cards.
     *
     * player_hands
     * v
     * [player-0      player-1      player-2      player-3]
     *   v             v             v             v
     *   { card-0 }    { card-0 }    { card-0 }    { card-0 }
     *     v             v             v
     *     { card-1 }    { card-1 }    { card-1 }
     *                                   v
     *                                   { card-2 }
     */
    struct card_list **player_hands;

    // Current player
    int current_player;

    // Current turn number
    int turn;

    /* We can add more variables here. I'm not quite sure what some
     * of the functions in "Game.h" mean, like "numOfSuit" or
     * "numOfColor".
     */

    // Convenience variable?
    int game_continue;

};




/* Free all link-lists */

int card_list_free
(struct card_list *list)
{

    struct card_list *previous;
    while ((previous = list))
    {
        list = (*previous).next;
        free (previous);
    }

    return 0 ;

}

int move_list_free
(struct move_list *list)
{

    struct move_list *previous;
    while ((previous = list))
    {
        list = (*previous).next;
        free (previous);
    }

    return 0 ;

}

int turn_list_free
(struct turn_list *list)
{

    struct turn_list *previous;
    while ((previous = list))
    {
        list = (*previous).next;
        move_list_free ((*previous).moves);
        free (previous);
    }

    return 0 ;

}




/* In-game list functions  */

/* Remove a card in a link-list. This function returns a pointer
 * to the beginning of the "card_list". This pointer is important
 * so keep it rather than discard it, like so:
 *
 * my_list = card_list_remove (my_list, nth);
 */

struct card_list *card_list_remove
(struct card_list *list, int nth)
{

    // If this list is empty, skip this
    if (list == NULL);

    // If the first element, free it
    else if (nth == 0)
    {
        struct card_list *next = (*list).next;
        free (list);
        list = next;
    }

    // For an element in the rest of the list, remove it and then bridge
    // the gap
    else
    {
        struct card_list *previous;
        struct card_list *current = list;
        while
        ((previous = current)
        && (0 <= (nth -= 1))
        && (current = (*current).next));
        (*previous).next = (*current).next;
        free (current);
    }

    return list;

}

// Find a card and return its index
int card_list_find
(struct card_list *list, Card card)
{

    int index = -1;

    // If this list is empty, skip this
    if (list == NULL);

    // Else, find the card
    else
    {
        struct card_list *current = list;
        while
        ((0 <= (index += 1))
        && ((*current).card != card)
        && (current = (*current).next));
        if (current == NULL)
        {
            index = -1;
        }
    }

    return index;

}

// Find and remove a card
struct card_list *card_list_find_and_remove
(struct card_list *list, Card card)
{

    // If this list is empty, skip this
    if (list == NULL);

    // Special case for the card being the first element
    else if ((*list).card == card)
    {
        struct card_list *previous = list;
        list = (*list).next;
        free (previous);
    }

    // Else, in the rest of the list, find the card
    else
    {
        struct card_list *previous;
        struct card_list *current = list;
        while
        ((previous = current)
        && ((*current).card != card)
        && (current = (*current).next));
        (*previous).next = (*current).next;
        free (current);
    }

    return list;

}

/* Add a card to the list  */
struct card_list *card_list_add
(struct card_list *list, int nth,
Card card)
{

    if ((list == NULL) || (nth == 0))
    {
        struct card_list *front =
        malloc (sizeof (struct card_list));
        assert_malloc (front);

        (*front).next = list;
        (*front).card = card;
        list = front;
    }
    else if (nth == -1)
    {
        struct card_list *current = list;
        while
        ((*current).next
        && (current = (*current).next));

        (*current).next =
        malloc (sizeof (struct card_list));
        assert_malloc ((*current).next);
        (* ((*current).next)).next = NULL;
        (* ((*current).next)).card = card;
    }
    else if (nth < -1)
    {
        errx
        (EXIT_FAILURE,
        "NTH needs to either be negative one or more. \n");
        perror
        ("card_list_add");
    }
    else
    {
        struct card_list *previous = list;
        struct card_list *current = list;
        while
        ((0 <= (nth -= 1))
        && (previous = current)
        && (current = (*current).next));

        // If we are still within the list, proceed
        if (0 < nth)
        {
            errx
            (EXIT_FAILURE,
            "NTH exceeds the list length. \n");
            perror
            ("card_list_add");
        }
        else
        {
            // Between two list parts, create a new cell and
            // connect them in
            (*previous).next =
            malloc (sizeof (struct card_list));
            assert_malloc ((*previous).next);

	    /*  Unhappy compiler =/ 
	     *  
	     *  (* ((*previous).next)).next = current;
	     *  (* ((*previous).next)).card = card;
	     */

	    previous = (*previous).next;
	    (*previous).next = current;
	    (*previous).card = card;
        }
    }

    return list;

}

/* Retrieve a card from the list  */
Card card_list_retrieve
(struct card_list *list, int nth)
{

    if (list == NULL)
    {
        errx
        (EXIT_FAILURE,
        "Empty list. \n");
        perror
        ("card_list_retrieve");
    }
    else if (nth == -1)
    {
        while
        ((*list).next
        && (list = (*list).next));
    }
    else if (nth < -1)
    {
        errx
        (EXIT_FAILURE,
        "NTH needs to either be negative one or more. \n");
        perror
        ("card_list_retrieve");
    }
    else
    {
        // This loop ends either when we reach the end of the list
        // or find the NTH node.
        while
        ((0 <= (nth -= 1))
        && ((*list).next)
        && (list = (*list).next));

        // If end of list, signal error
        if (list == NULL)
        {
            errx
            (EXIT_FAILURE,
            "Exceeds the length of the list. \n");
            perror
            ("card_list_retrieve");
        }
    }

    return (*list).card;

}



/* Remove a move in a link-list. This function returns a pointer
 * to the beginning of the "move_list". This pointer is important
 * so keep it rather than discard it, like so:
 *
 * my_list = move_list_remove (my_list, nth);
 */

struct move_list *move_list_remove
(struct move_list *list, int nth)
{

    // If this list is empty, skip this
    if (list == NULL);

    // If the first element, free it
    else if (nth == 0)
    {
        struct move_list *next = (*list).next;
        free (list);
        list = next;
    }

    // For an element in the rest of the list, remove it and then bridge
    // the gap
    else
    {
        struct move_list *previous;
        struct move_list *current = list;
        while
        ((previous = current)
        && (0 <= (nth -= 1))
        && (current = (*current).next));
        (*previous).next = (*current).next;
        free (current);
    }

    return list;

}

/* Add a move to the list  */
struct move_list *move_list_add
(struct move_list *list, int nth,
playerMove move)
{

    if ((list == NULL) || (nth == 0))
    {
        struct move_list *front =
        malloc (sizeof (struct move_list));
        assert_malloc (front);

        (*front).next = list;
        (*front).move = move;
        list = front;
    }
    else if (nth == -1)
    {
        struct move_list *current = list;
        while
        ((*current).next
        && (current = (*current).next));

        (*current).next =
        malloc (sizeof (struct move_list));
        assert_malloc ((*current).next);
        (* ((*current).next)).next = NULL;
        (* ((*current).next)).move = move;
    }
    else if (nth < -1)
    {
        errx
        (EXIT_FAILURE,
        "NTH needs to either be negative one or more. \n");
        perror
        ("move_list_add");
    }
    else
    {
        struct move_list *previous = list;
        struct move_list *current = list;
        while
        ((0 <= (nth -= 1))
        && (previous = current)
        && (current = (*current).next));

        // If we are still within the list, proceed
        if (0 < nth)
        {
            errx
            (EXIT_FAILURE,
            "NTH exceeds the list length. \n");
            perror
            ("move_list_add");
        }
        else
        {
            // Between two list parts, create a new cell and
            // connect them in
            (*previous).next =
            malloc (sizeof (struct move_list));
            assert_malloc ((*previous).next);

	    /*  Unhappy compiler =/ 
	     *  
	     *  (* ((*previous).next)).next = current;
	     *  (* ((*previous).next)).move = move;
	     */

	    previous = (*previous).next;
	    (*previous).next = current;
	    (*previous).move = move;
        }
    }

    return list;

}

/* Retrieve a move from the list  */
playerMove move_list_retrieve
(struct move_list *list, int nth)
{

    if (list == NULL)
    {
        errx
        (EXIT_FAILURE,
        "Empty list. \n");
        perror
        ("move_list_retrieve");
    }
    else if (nth == -1)
    {
        while
        ((*list).next
        && (list = (*list).next));
    }
    else if (nth < -1)
    {
        errx
        (EXIT_FAILURE,
        "NTH needs to either be negative one or more. \n");
        perror
        ("move_list_retrieve");
    }
    else
    {
        // This loop ends either when we reach the end of the list
        // or find the NTH node.
        while
        ((0 <= (nth -= 1))
        && ((*list).next)
        && (list = (*list).next));

        // If end of list, signal error
        if (list == NULL)
        {
            errx
            (EXIT_FAILURE,
            "Exceeds the length of the list. \n");
            perror
            ("move_list_retrieve");
        }
    }

    return (*list).move;

}

int move_list_length
(struct move_list *list)
{

    int len = 0;

    // If empty list, move on
    if (list == NULL);

    // Else, run through the list and count
    else
    {
        while
        ((len += 1)
        && (list = (*list).next));
    }

    return len;

}



/* Remove a turn in a link-list. This function returns a pointer
 * to the beginning of the "turn_list". This pointer is important
 * so keep it rather than discard it, like so:
 *
 * my_list = turn_list_remove (my_list, nth);
 */

struct turn_list *turn_list_remove
(struct turn_list *list, int nth)
{

    // If this list is empty, skip this
    if (list == NULL);

    // If the first element, free it
    else if (nth == 0)
    {
        struct turn_list *next = (*list).next;
        free (list);
        list = next;
    }

    // For an element in the rest of the list, remove it and then bridge
    // the gap
    else
    {
        struct turn_list *previous = list;
        struct turn_list *current = list;
        while
        ((previous = current)
        && (0 <= (nth -= 1))
        && (current = (*current).next));
        (*previous).next = (*current).next;
        free (current);
    }

    return list;

}

/* Add a turn to the list  */
struct turn_list *turn_list_add
(struct turn_list *list, int nth,
struct move_list *moves)
{

    if ((list == NULL) || (nth == 0))
    {
        struct turn_list *front =
        malloc (sizeof (struct turn_list));
        assert_malloc (front);

        (*front).next = list;
        (*front).moves = moves;
        list = front;
    }
    else if (nth == -1)
    {
        struct turn_list *current = list;
        while
        ((*current).next
        && (current = (*current).next));

        (*current).next =
        malloc (sizeof (struct turn_list));
        assert_malloc ((*current).next);
        (* ((*current).next)).next = NULL;
        (* ((*current).next)).moves = moves;
    }
    else if (nth < -1)
    {
        errx
        (EXIT_FAILURE,
        "NTH needs to either be negative one or more. \n");
        perror
        ("turn_list_add");
    }
    else
    {
        struct turn_list *previous = list;
        struct turn_list *current = list;
        while
        ((0 <= (nth -= 1))
        && (previous = current)
        && (current = (*current).next));

        // If we are still within the list, proceed
        if (0 < nth)
        {
            errx
            (EXIT_FAILURE,
            "NTH exceeds the list length. \n");
            perror
            ("turn_list_add");
        }
        else
        {
            // Between two list parts, create a new cell and
            // connect them in
            (*previous).next =
            malloc (sizeof (struct turn_list));
            assert_malloc ((*previous).next);

	    /*  Unhappy compiler =/ 
	     *  
	     *  (* ((*previous).next)).next = current;
	     *  (* ((*previous).next)).moves = moves;
	     */

	    previous = (*previous).next;
	    (*previous).next = current;
	    (*previous).moves = moves;
        }
    }

    return list;

}

/* Retrieve a turn from the list  */
struct move_list *turn_list_retrieve
(struct turn_list *list, int nth)
{

    if (list == NULL)
    {
        errx
        (EXIT_FAILURE,
        "Empty list. \n");
        perror
        ("turn_list_retrieve");
    }
    else if (nth == -1)
    {
        while
        ((*list).next
        && (list = (*list).next));
    }
    else if (nth < -1)
    {
        errx
        (EXIT_FAILURE,
        "NTH needs to either be negative one or more. \n");
        perror
        ("turn_list_retrieve");
    }
    else
    {
        // This loop ends either when we reach the end of the list
        // or find the NTH node.
        while
        ((0 <= (nth -= 1))
        && ((*list).next)
        && (list = (*list).next));

        // If end of list, signal error
        if (list == NULL)
        {
            errx
            (EXIT_FAILURE,
            "Exceeds the length of the list. \n");
            perror
            ("turn_list_retrieve");
        }
    }

    return (*list).moves;

}





// Fill in the cards which now exist in the game, and keep track of
// the number of each value, colour and suit which exists in it.
// Also, make a deck
int fill_in_remaining_empty_variables
(Game new_game,
color colors[], suit suits[], value values[])
{

    int **cards_initially =
    (*new_game).cards_initially;

    struct card_tracker *cards_in_game =
    (*new_game).cards_in_game;

    // First element contains the length of the initial deck
    cards_initially[0][0] =
    (*new_game).cards_in_total;

    /* Note, "enums" work like this:
     * (RED = 0)
     * (BLUE = 1)
     * (GREEN = 2)
     * (YELLOW = 3)
     * (PURPLE = 4)
     *
     * So, elements in COLORS, SUITS and VALUES are integers
     * (colors[0] = BLUE = 1)
     * (colors[1] = RED = 2)
     * (colors[2] = BLUE = 1)
     * (colors[3] = PURPLE = 4)
     *
     * And we can use these integers as indices in our three arrays
     * in CARDS_INITIALLY.
     *
     *   array             colour-array   blue
     * (cards_initially  [1]         [1])
     *
     *   array             colour-array   red
     * (cards_initially  [1]         [2])
     *
     *   array             colour-array   purple
     * (cards_initially  [1]         [4])
     */

    int count = (*new_game).cards_in_deck;
    int card_id;
    while (0 <= (count -= 1))
    {

        // Second element contains the number of cards for each colour
        cards_initially[1][colors[count]] += 1;

        // Third element cotains the number of cards for each suit
        cards_initially[2][suits[count]] += 1;

        // Final element contains the number of cards for each value
        cards_initially[3][values[count]] += 1;

        card_id =
        total_values * total_suits * colors[count] +
        total_values * suits[count] +
        values[count];

        // Also make the card or increment the number of an existing card
        if ((cards_in_game[card_id]).card == NULL)
        {

            (cards_in_game[card_id]).card =
            newCard
            (values[count],
            colors[count],
            suits[count]);

            (cards_in_game[card_id]).dupes = 1;

        }

        // Also need to make sure we are within duplicate-limits.
        else if
        ((cards_in_game[card_id]).dupes <
        (*new_game).cards_max_dupe)
        {

            (cards_in_game[card_id]).dupes += 1;

        }
        else
        {

            printf
            ("%s %03d ",
            "We have the maximum number of this card already: ",
            card_id);

            perror
            ("fill_in_remaining_empty_variables");

        }

        // Anyway, whatever the card is, put it into the deck.
        // Fortunately, we implement the "while" loop so that we
        // begin from the end of the deck and traverse to the front.
        // In doing so, we always append to the front of the link-list
        (*new_game).deck =
        card_list_add
        ((*new_game).deck, 0,
        (cards_in_game[card_id]).card);

    }

    return 0;

}



// A function to distribute cards to the players from the deck
// INITIAL_DRAW is the variable which regulates the number of
// cards to draw
int game_distribute_cards
(Game new_game)
{

    int cards_in_all_hands =
    initial_draw * NUM_PLAYERS;

    printf
    ("%s \n%s \n%s %03d %s %03d \n",
    "game_distribute_cards",
    "Enough cards to distribute?",
    "  ... ", cards_in_all_hands,
    "<", (*new_game).cards_in_total);

    // Make sure we have enough cards to distribute
    assert
    (cards_in_all_hands <
    (*new_game).cards_in_total);

    // We pass give each player a card from the top of the deck
    int to_deal = initial_draw;
    int player;
    while (0 <= (to_deal -= 1))
    {

        player = -1;
        while ((player += 1) < NUM_PLAYERS)
        {

            // Add to this player's hand the top card in the
            // deck
            (*new_game).player_hands[player] =
            card_list_add
            ((*new_game).player_hands[player], 0,
            (* ((*new_game).deck)).card);

            // And then remove this card from the deck itself
            (*new_game).deck =
            card_list_remove
            ((*new_game).deck, 0);

        }
    }

    // The top card of the deck now becomes the first card in
    // the discard pile
    (*new_game).discard_pile =
    card_list_add
    ((*new_game).discard_pile, 0,
    (* ((*new_game).deck)).card);

    // And then remove this card from the deck itself
    (*new_game).deck =
    card_list_remove
    ((*new_game).deck, 0);

    // Now, we're done!
    return 0;

}





// Create a new game engine.
//
// This creates a game with a deck of the given size
// and the value, color, and suit of each card to be taken from
// each of the arrays in order.
//
// Your game will always have 4 players. Each player starts with a hand
// of 7 cards. The first card from the deck is given to player 0, the
// second to player 1, the third to player 2, the fourth to player 3,
// the fifth to player 0, and so on until each player has 7 cards.
Game newGame
(int deckSize, value values[],
color colors[], suit suits[])
{

    // Allocate memory in the heap for a new game
    Game new_game =
    calloc (sizeof (struct _game), 1);
    assert_calloc (new_game);

    // Allocate memory for an array which will keep track of the
    // cards in the game. This array will only contain pointers.
    // The "struct card_tracker" contains a pointer and an integer.
    struct card_tracker *cards_in_game =
    calloc
    (total_colors * total_suits * total_values,
    sizeof (struct card_tracker));

    assert_calloc (cards_in_game);
    // The first element contains the initial deck size, the next
    // values will be of all the card suits, then the card colours
    // and finally the card values
    int **cards_initially =
    malloc ((sizeof (int *)) * 4);
    assert_malloc (cards_initially);

    cards_initially[0] =
    calloc (sizeof (int), 1);
    assert_calloc (cards_initially[0]);

    cards_initially[1] =
    calloc (sizeof (int), total_colors);
    assert_calloc (cards_initially[1]);

    cards_initially[2] =
    calloc (sizeof (int), total_suits);
    assert_calloc (cards_initially[2]);

    cards_initially[3] =
    calloc (sizeof (int), total_values);
    assert_calloc (cards_initially[3]);

    // Allocate memory for an array which keeps track of the cards
    // in each players' hands. Each element represents one player,
    // and contains an link-list of cards.
    struct card_list **player_hands =
    calloc (sizeof (struct card_list *), 4);
    assert_calloc (player_hands);


    // Provide values to the new game

    // Keep track of the cards in the game
    (*new_game).cards_in_game = cards_in_game;

    // Presume the game begins clockwise
    (*new_game).direction = CLOCKWISE;

    // Keep track of the initial deck
    (*new_game).cards_initially = cards_initially;

    // Number of cards which the current player needs to draw
    (*new_game).cards_to_draw = 0;

    // Number of cards in total
    (*new_game).cards_in_total = deckSize;

    // Number of cards in the deck
    (*new_game).cards_in_deck = deckSize;

    // Maximum number of cards to have duplicates of
    (*new_game).cards_max_dupe = cards_dupe;

    // Make these NULL
    (*new_game).turn_history = NULL;
    (*new_game).discard_pile = NULL;
    (*new_game).deck = NULL;

    // Keep track of cards in each players' hands
    (*new_game).player_hands = player_hands;

    // Current player
    (*new_game).current_player = 0;

    // Current turn
    (*new_game).turn = 0;

    // Continue
    (*new_game).game_continue = 1;


    // Fill in the cards which now exist in the game, and keep track
    // of the number of each colour, suit and value which exists in
    // it
    fill_in_remaining_empty_variables
    (new_game, colors, suits, values);

    // Distribute cards and then place the top card in the deck
    // onto the discard pile
    game_distribute_cards
    (new_game);

    return new_game;

}





// Destroy an existing game.
//
// This should free all existing memory used in the game including
// allocations for players and cards.
void destroyGame
(Game game)
{

    // Firstly, free all the cards in the game. The number of cards
    // in the game is CARDS_DISTINCT.
    struct card_tracker *card_index =
    ((*game).cards_in_game - 1);

    while
    ((card_index += 1)
    && (0 < (*game).cards_distinct))
    {

        // If we find a distinct card, decrement CARDS_DISTINCT and
        // free the memory which contains the card
        if ((*card_index).card == NULL);
        {

            (*game).cards_distinct -= 1;
            free ((*card_index).card);

        }
    }

    // Free the variable which tracks these cards
    free ((*game).cards_in_game);


    // Now free the all items which keep track of the number of
    // colours, suits and values of each card, and the initial
    // deck size.
    int items = 4;
    while (0 <= (items -= 1))
    {
        free (((*game).cards_initially)[items]);
    }

    // And free their container
    free ((*game).cards_initially);


    // Free the entire link-list for all moves and every turn
    turn_list_free ((*game).turn_history);


    // Free the entire link-lists for players' hands
    items = NUM_PLAYERS;
    while (0 <= (items -= 1))
    {
        card_list_free (((*game).player_hands)[items]);
    }
    free ((*game).player_hands);


    // And finally, clear the game
    free (game);

}



// The following functions can be used by players to discover
// information about the state of the game.

// Get the number of cards that were in the initial deck.
int numCards
(Game game)
{

    return (*game).cards_in_deck;

}



// Get the number of cards in the initial deck of a particular
// suit.
int numOfSuit
(Game game, suit suit)
{

    /* 
     * // made a while loop to check each node value
     * // if they correspond to the suit,
     * int i = 0;
     * int count = 0;
     * // assuming cards_in_game is an array
     * while (i < deckSize)) {
     *   if ((*game).cards_in_game[i] == suit) {
     *     count++;
     *   }
     *   i++;
     * }
     * return count;
     */

    return (((*game).cards_initially)[2][suit]);

}



// Get the number of cards in the initial deck of a particular color.
int numOfColor
(Game game, color color)
{

    /* 
     * int i = 0;
     * int count = 0;
     * // assuming cards_in_game is an array
     * while (i < deckSize)) {
     *   if ((*game).cards_in_game[i] == color) {
     *     count++;
     *   }
     *   i++;
     * }
     * return count;
     */

    return (((*game).cards_initially)[1][color]);

}



// Get the number of cards in the initial deck of a particular value.
int numOfValue
(Game game, value value)
{

    /* 
     * int i = 0;
     * int count = 0;
     * // assuming cards_in_game is an array
     * while (i < deckSize)) {
     *   if ((*game).cards_in_game[i] == color) {
     *     count++;
     *   }
     *   i++;
     * }
     * return count;
     */

    return (((*game).cards_initially)[3][value]);

}



// Get the number of the player whose turn it is.
int currentPlayer (Game game)
{

    return ((*game).current_player);

}





// Get the current turn number.
//
// The turn number increases after a player ends their turn.
// The turn number should start at 0 once the game has started.
int currentTurn
(Game game)
{

    return (*game).turn;

}



// !!!
// Get the number of points for a given player.
// Player should be between 0 and 3.
//
// This should _not_ be called by a player.
int playerPoints
(Game game, int player)
{

    return 0;

}



// Get the current direction of play.
direction playDirection
(Game game)
{

    return (*game).direction;

}





// This function returns the number of turns that have occurred in the
// game so far including the current turn.
// When using either the turnMoves or pastMove function,
// the turn number should be less than the number of moves that this
// function returns.
// (i.e. on turn 0 of the game, this should return 1, as there has been
// 1 turn so far including the current turn; if you called pastMove you
// would need to call it on turn 0, as this is the only possible value
// less than 1.)
int numTurns
(Game game)
{

    // Since we begin at turn zero, we need to add one to the result.
    return ((*game).turn + 1);

}



// Get the number of moves that happened on a turn.
//
// A turn may consist of multiple moves such as drawing cards, playing
// cards, and ending the turn.
int turnMoves
(Game game, int turn)
{

    int moves = 0;

    if ((0 <= turn) && (turn <= (*game).turn))
    {

        struct turn_list *current_turn =
        (*game).turn_history;

        // Remember, the latest turn is the first one in the list. Find
        // the current turn.
        turn = (*game).turn - turn;
        while
        ((0 <= (turn -= 1))
        && (current_turn = (*current_turn).next));

        struct move_list *current_move =
        (*current_turn).moves;

        // And then count the number of moves
        if (current_move)
        {
            while
            ((0 <= (moves += 1))
            && (current_move = (*current_move).next));
        }
    }

    return moves;

}



// Look at a previous move from a specified turn.
playerMove pastMove
(Game game, int turn, int move)
{

    playerMove the_move;

    if ((0 <= turn) && (turn <= (*game).turn))
    {

        struct turn_list *current_turn =
        (*game).turn_history;

        // Remember, the latest turn is the first one in the list. Find
        // the current turn.
        turn = (*game).turn - turn;
        while
        ((0 <= (turn -= 1))
        && (current_turn = (*current_turn).next));

        struct move_list *current_move =
        (*current_turn).moves;

        // And then look for move
        if (current_move)
        {

            while
            ((0 <= (move -= 1))
            && (current_move = (*current_move).next));

        }

        // If we find the move, capture it
        if (current_move)
        {

            the_move = (*current_move).move;

        }
    }
    else
    {

        the_move.action = 0;
        the_move.nextColor = 0;
        the_move.card = NULL;

    }

    return the_move;

}



// Get the number of cards in a given player's hand.
int playerCardCount
(Game game, int player)
{

    struct card_list *player_hand =
    (*game).player_hands[player];

    int cards_in_hand = 0;
    while
    (player_hand
    && (cards_in_hand += 1)
    && (player_hand = (*player_hand).next));

    return cards_in_hand;

}



// Get the number of cards in the current player's hand.
int handCardCount
(Game game)
{

    struct card_list *player_hand =
    (*game).player_hands[currentPlayer (game)];

    int cards_in_hand = 0;
    while
    (player_hand
    && (cards_in_hand += 1)
    && (player_hand = (*player_hand).next));

    return cards_in_hand;

}



// View a card from the current player's own hand.
//
// The player should not need to free () this card,
// so you should not allocate or clone an existing card
// but give a reference to an existing card.
Card handCard
(Game game, int card)
{

    struct card_list *player_hand =
    (*game).player_hands[currentPlayer (game)];

    while
    ((0 <= (card -= 1))
    && (player_hand = (*player_hand).next));

    return (*player_hand).card;

}





// Get the card that is on the top of the discard pile.
Card topDiscard
(Game game)
{

    return (* ((*game).discard_pile)).card;

}





// !!!
// Check if a given move is valid.
//
// If the last player played a 2 (DRAW_TWO),
// the next player must either play a 2
// or draw 2 cards.
// Otherwise, the player must play a card that is either a ZERO
// or that has the same color, value, or suit as the card on the top
// of the discard pile.
//
// If the player plays an ADVANCE, the next player's turn is skipped.
// If the player plays a BACKWARDS, the direction of play is reversed.
// If the player plays a CONTINUE, they may play another card.
// If the player plays a DECLARE, they must also state which color
// the next player's discarded card should be.
//
// A player can only play cards from their hand.
// A player may choose to draw a card instead of discarding a card.
// A player must draw a card if they are unable to discard a card.
//
// This check should verify that:
// *The card being played is in the player's hand
// *The player has played at least one card before finishing their turn,
// unless a draw-two was played, in which case the player may not
// play a card, and instead must draw the appropriate number of cards.
int isValidMove
(Game game, playerMove move)
{

    // Get the current turn and then get the previous turn number
    // int previous_turn;
    // int current_turn = (*game).turn;
    // if (1 <= current_turn)
    // {
    //     previous_turn = current_turn - 1;
    // }
    //
    // Using the previous turn number look through the turn_list struct for the move history
    // Find the moves played in the turn/move history
    // Using if statements, if the following "moves" were played check the hand if there is a relevant card
    // Checking a player's hand?
    // while player_hand linked lists
    // e.g. if previous_turn = DRAW_TWO check for a DRAW_TWO card in player card array
    // If found than return validmove
    // If not than return INVALID MOVE
    // Repeat for all plays involving interaction, e.g. ADVANCE,
    /* e.g.
     while (((*game).player_hands[player]) != NULL)
     (while (*game.turn) != NULL) {
     (*new_game).player_hands[player]
    */

    return 0;

}



int advance_turn
(Game new_game)
{

    (*new_game).turn += 1;

    if ((*new_game).direction == ANTICLOCKWISE)
    {

        // If we are at player zero, begin again from
        // the player at the other end
        if ((*new_game).current_player == 0)
        {
            (*new_game).current_player = NUM_PLAYERS;
        }

        (*new_game).current_player -= 1;

    }
    else
    {

        (*new_game).current_player += 1;

        // If we are at player on the other end, begin again
        // from player zero
        if ((*new_game).current_player == NUM_PLAYERS)
        {
            (*new_game).current_player = 0;
        }

    }

    return (*new_game).turn;

}


// Play the given action for the current player
//
// If the player makes the END_TURN move, their turn ends,
// and it becomes the turn of the next player.
//
// This should _not_ be called by the player AI.
void playMove
(Game game, playerMove move)
{

    // If the game is new, make a new history list for both
    // the first turn and move
    if ((*game).turn_history == NULL)
    {

        (*game).turn_history =
        turn_list_add
        ((*game).turn_history, 0, NULL);

        (* ((*game).turn_history)).moves =
        move_list_add
        ((* ((*game).turn_history)).moves, 0,
        move);
    }

    // Else, traverse the existing list and put in the move
    else
    {

        struct move_list *last_turn =
        (* ((*game).turn_history)).moves;

        playerMove last_move =
        move_list_retrieve (last_turn, -1);

        // If the last move of the most recent turn is END_TURN,
        // then make a new turn
        if (last_move.action  == END_TURN)
        {

            (*game).turn_history =
            turn_list_add
            ((*game).turn_history, 0, NULL);

        }

        // On this turn, put new move at the end of the list
        // of moves
        (* ((*game).turn_history)).moves =
        move_list_add
        ((* ((*game).turn_history)).moves, -1,
        move);

    }


    // If end turn, increment the turn number and advance to
    // the next player
    if (move.action == END_TURN)
    {

        struct move_list *moves =
        (* ((*game).turn_history)).moves;

        playerMove move;

        // Walk through the chain of moves and execute
        while (moves)
        {

            move = (*moves).move;
            moves = (*moves).next;

            // End when END_TURN
            if (move.action == END_TURN);

            // If PLAY_CARD, put the card into DISCARD_PILE and
            // remove it from hand
            else if (move.action == PLAY_CARD)
            {

                // If the previous players played DRAW_TWO,
                // but the current player plays something
                // else, they must pick up two cards from
                // the deck
                (*game).player_hands
                [(*game).current_player] =
                card_list_find_and_remove
                ((*game).player_hands
                [(*game).current_player],
                move.card);

                (*game).discard_pile =
                card_list_add
                ((*game).discard_pile, 0,
                move.card);

                value value = (* (move.card)).value;

                // For special effects ...
                if (value == DRAW_TWO)
                {
                    (*game).cards_to_draw += 2;
                }
                else if (value == ADVANCE)
                {
                    advance_turn (game);
                }
                else if (value == BACKWARDS)
                {
                    // We know CLOCKWISE is 0
                    // and ANTICLOCKWISE is 1.
                    // So we make direction their opposite
                    (*game).direction =
                    ! ((*game).direction);
                }
                else if (value == CONTINUE)
                {
                    // @@@
                }
                else if (value == DECLARE)
                {
                    // @@@
                }
            }
        }

        advance_turn
        (game);

    }

    // return 0;

}



// !!!
// Check the game winner.
//
// Returns NOT_FINISHED if the game has not yet finished,
// 0-3, representing which player has won the game, or
// NO_WINNER if the game has ended but there was no winner.
int gameWinner
(Game game)
{

    // I guess this is checked at the end of every turn?
    // You would check for cards in the player hand, if there are no cards at the end of turn than the current player would
    // be declared winner

    return 0;

}





/* 
 * int experiment_002
 * (void)
 * {
 *
 *   Game game_one =
 *     newGame (20, ???, ???, ???);
 *
 *   printf
 *     ("%s \n\n",
 *       "Game_one.");
 *
 *   destroyGame
 *     (game_one);
 *
 *   return 0;
 *
 * }
 *
 *
 *
 * int main
 * (int argc, char *argv[])
 * {
 *
 *   experiment_002 ();
 *
 *   return 0;
 *
 * }
 */


