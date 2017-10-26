/* Initialise */
// '("malloc" "free")
// Various complete functions
// Various prototpyes

#include <stdlib.h>

#include "Card.c"
#include "Game.h"


// Handle "calloc" errors
    // If unable to correctly allocate memory, raise an error
    // Else, we return successfully

int check_calloc (void *this_pointer) {
    if (this_pointer == NULL) {
        printf("%s","Unable to allocate memory to the heap. \n");
        perror("calloc");
    };
    return 0;
}


/* Mechanics */
// This is the "struct" which forms the list of cards, like for the deck,
// discard pile and players' hands
    // The card itself
    // The next card

struct card_list {
    Card card;
    struct card_list *next;
};


// THis is the "struct" which forms the move history
    // The move itself
    // The next move

struct move_list {
    playerMove move;
    struct move_list *next;
};


// THis is the "struct" which contains pointers to the first move of
// that turn. So, each element here refers to a link-list of moves
    // Pointer to the first move of that turn
    // The next turn
struct turn_list {
    struct move_list *moves;
    struct turn_list *next;
};


// This "struct" tracks one distinct card and the number of the same card
// which currently exists in the game.
    // Pointer to the card
    // Number of duplicates

struct card_tracker {
    Card card;
    int dupes;
};


// This is the "struct" which maintains all variables during a game
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

    // Current direction

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

    /* In case of "DRAW_TWO", this accumulates the number of cards a
     * player has to pick up. So, if two people play consecutive
     * "DRAW_TWO" cards, this value becomes 4. Once a person picks up
     * cards, this reverts to 0.
     */

    // Number of distinct cards which exist and need to free later

    // Deck size plus cards in players' hands

    // Cards left in the deck

    // Maximum number of identical cards possible

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

    // Current turn number

    /* We can add more variables here. I'm not quite sure what some
     * of the functions in "Game.h" mean, like "numOfSuit" or
     * "numOfColor".
     */

    // Convenience variable?
//Game tracks 12 things

struct _game {
    struct card_tracker *cards_in_game;
    direction direction;
    int **cards_initially;
    int cards_to_draw;
    int cards_distinct;
    int cards_in_total;
    int cards_in_deck;
    int cards_max_dupe;
    struct turn_list *turn_history;
    struct card_list **player_hands;
    int turn;
    int game_continue;
};


/* Functions to free all link-lists we have created */

int card_list_free (struct card_list *list) {
    struct card_list *previous;
    while (previous = list) {
        list = (*previous).next;
        free (previous);
    }
}

int move_list_free (struct move_list *list) {
    struct move_list *previous;
    while (previous = list) {
        list = (*previous).next;
        free (previous);
    }
}

int turn_list_free (struct turn_list *list) {
    struct turn_list *previous;
    while (previous = list)
    {
        list = (*previous).next;
        move_list_free ((*previous).moves);
        free (previous);
    }
}


// Fill in the cards which now exist in the game, and keep track of
// the number of each value, colour and suit which exists in it

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

    // First element contains the length of the initial deck

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

        // Second element contains the number of cards for each colour

        // Third element cotains the number of cards for each suit

        // Final element contains the number of cards for each value

        // Also make the card or increment the number of an existing card

        // Also need to make sure we are within duplicate-limits.

int fill_in_remaining_empty_variables (struct card_tracker *cards_in_game,
int **cards_initially, int deckSize, color colors[],
suit suits[], value values[], int cards_max_dupe) {

    cards_initially[0] = malloc ((sizeof (int)) * 1);
    check_malloc (cards_initially[0]);

    cards_initially[1] = malloc ((sizeof (int)) * total_colors);
    check_malloc (cards_initially[1]);

    cards_initially[2] = malloc ((sizeof (int)) * total_suits);
    check_malloc (cards_initially[2]);

    cards_initially[3] = malloc ((sizeof (int)) * total_values);
    check_malloc (cards_initially[3]);

    cards_initially[0][0] = deckSize;

    int count = deckSize;
    int card_id;

    while (0 <= (count -= 1)) {
        cards_initially[1][colors[count]] += 1;
        cards_initially[2][suits[count]] += 1;
        cards_initially[3][values[count]] += 1;
        card_id = total_values * total_suits * colors[count] +
        total_values * suits[count] + values[count];
        if ((cards_in_game[card_id]).card == NULL) {
            (cards_in_game[card_id]).card =
            newCard
            (values[count],
            colors[count],
            suits[count]);
            (cards_in_game[card_id]).dupes = 1;
        } else if ((cards_in_game[card_id]).dupes <= cards_max_dupe) {
            (cards_in_game[card_id]).dupes += 1;
        } else {
            printf ("%s %03d ",
                    "We have the maximum number of this card already: ",card_id);
            perror ("fill_in_remaining_empty_variables");
        }
    }
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

    // Allocate memory in the heap for a new game

    // Allocate memory for an array which will keep track of the
    // cards in the game. This array will only contain pointers.
    // The "struct card_tracker" contains a pointer and an integer.
    
    // The first element contains the initial deck size, the next
    // values will be of all the card suits, then the card colours
    // and finally the card values

    // Allocate memory for an array which keeps track of the cards
    // in each players' hands. Each element represents one player,
    // and contains an link-list of cards.

    // Provide values to the new game
    // Keep track of the cards in the game

    // Assume the game begins clockwise

    // Keep track of the initial deck

    // Number of cards which the current player needs to draw

    // Number of cards in total

    // Number of cards in the deck

    // Maximum number of cards to have duplicates of
    // (*new_game).cards_max_dupe = ???
    // Keep track of cards in each players' hands

    // Current turn

    // Continue

    // Fill in the cards which now exist in the game, and keep track
    // of the number of each colour, suit and value which exists in it

    // Create link-lists for the deck, discard pile and players' hands

Game newGame (int deckSize, value values[], color colors[], suit suits[]) {
    Game new_game = malloc (sizeof (struct _game)); assert_malloc (new_game);

    struct card_tracker *cards_in_game =
    calloc
    (total_colors * total_suits * total_values,
    sizeof (struct card_tracker));

    check_calloc (cards_in_game);

    int **cards_initially =
    malloc ((sizeof (int *)) * 4);
    assert_malloc (cards_initially);

    cards_initially[0] =
    malloc ((sizeof (int)) * 1);
    check_calloc (cards_initially[0]);

    cards_initially[1] =
    malloc ((sizeof (int)) * total_colors);
    check_calloc (cards_initially[1]);

    cards_initially[2] =
    malloc ((sizeof (int)) * total_suits);
    check_calloc (cards_initially[2]);

    cards_initially[3] =
    malloc ((sizeof (int)) * total_values);
    check_calloc (cards_initially[3]);

    struct card_list **player_hands =
    malloc ((sizeof (struct card_list *)) * 4);
    assert_malloc (player_hands);

    (*new_game).cards_in_game = cards_in_game;
    (*new_game).direction = CLOCKWISE;
    (*new_game).cards_initially = cards_initially;
    (*new_game).cards_to_draw = 0;
    (*new_game).cards_in_total = deckSize;
    (*new_game).cards_in_deck = deckSize;
    (*new_game).player_hands = player_hands;
    (*new_game).turn = 0;
    (*new_game).game_continue = 1;

    fill_in_remaining_empty_variables
    (cards_in_game, cards_initially,
    deckSize, colors, suits, values,
    (*new_game).cards_max_dupe);

    return new_game;
}


// Destroy an existing game.
//
// This should free all existing memory used in the game including
// allocations for players and cards.
    // Firstly, free all the cards in the game. The number of cards
    // in the game is CARDS_DISTINCT.

        // If we find a distinct card, decrement CARDS_DISTINCT and
        // free the memory which contains the card

    // Free the variable which tracks these cards

    // Now free the all items which keep track of the number of
    // colours, suits and values of each card, and the initial
    // deck size.

    // And free their container

    // Free the entire link-list for all moves and every turn

    // Free the entire link-lists for players' hands

    // And finally, clear the game

void destroyGame (Game game) {
    struct card_tracker *card_index =
    ((*game).cards_in_game - 1);

    while ((card_index += 1)
    && (0 < (*game).cards_distinct)) {

        if ((*card_index).card == NULL) {
            (*game).cards_distinct -= 1;
            free ((*card_index).card);
        }
    }

    free ((*game).cards_in_game);

    int items = 4;
    while (0 <= (items -= 1)) {
        free (((*game).cards_initially)[items]);
    }

    free ((*game).cards_initially);

    turn_list_free ((*game).turn_history);

    items = NUM_PLAYERS;
    while (0 <= (items -= 1)) {
        card_list_free (((*game).player_hands)[items]);
    }

    free ((*game).player_hands);

    free (game);
}


// The following functions can be used by players to discover
// information about the state of the game.

// Get the number of cards that were in the initial deck.

int numCards (Game game) {
    return (*game).cards_in_deck;
}


// Get the number of cards in the initial deck of a particular
// suit.
int numOfSuit (Game game, suit suit) {
    return ((((*game).cards_initially)[2])[suit]);
}



// Get the number of cards in the initial deck of a particular color.
int numOfColor (Game game, color color) {
    return ((((*game).cards_initially)[1])[color]);
}


// Get the number of cards in the initial deck of a particular value.
int numOfValue (Game game, value value) {
    return ((((*game).cards_initially)[3])[value]);
}


// Get the number of the player whose turn it is.
int currentPlayer (Game game) {
    return ((*game).turn % NUM_PLAYERS);
}


// Get the current turn number.
//
// The turn number increases after a player ends their turn.
// The turn number should start at 0 once the game has started.
int currentTurn (Game game) {
    return (*game).turn;
}


// !!!
// Get the number of points for a given player.
// Player should be between 0 and 3.
//
// This should _not_ be called by a player.

int playerPoints (Game game, int player) {
    return 0;
}


// Get the current direction of play.
direction playDirection (Game game) {
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

    // Since we begin at turn zero, we need to add one to the result.

int numTurns (Game game) {
    return ((*game).turn + 1);
}


// Get the number of moves that happened on a turn.
//
// A turn may consist of multiple moves such as drawing cards, playing
// cards, and ending the turn.

        // Remember, the latest turn is the first one in the list. Find
        // the current turn.

        // And then count the number of moves

int turnMoves (Game game, int turn) {
    int moves = 0;
    if ((0 <= turn) && (turn <= (*game).turn)) {
        struct turn_list *current_turn = (*game).turn_history;
        turn = (*game).turn - turn;
        while ((0 <= (turn -= 1)) 
        && (current_turn = (*current_turn).next));

        struct move_list *current_move =
        (*current_turn).moves;

        if (current_move) {
            while ((0 <= (moves += 1))
            && (current_move = (*current_move).next));
        }
    }
    return moves;
}


// Look at a previous move from a specified turn.

        // Remember, the latest turn is the first one in the list. Find
        // the current turn.

        // And then look for move

        // If we find the move, capture it

playerMove pastMove (Game game, int turn, int move) {
    playerMove the_move;
    if ((0 <= turn) && (turn <= (*game).turn)) {
        struct turn_list *current_turn = (*game).turn_history;
        turn = (*game).turn - turn;
        while ((0 <= (turn -= 1))
        && (current_turn = (*current_turn).next));
        struct move_list *current_move =
        (*current_turn).moves;
        if (current_move) {
            while ((0 <= (move -= 1))
            && (current_move = (*current_move).next));
        }
        if (current_move) {
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


// !!!
// Get the number of cards in a given player's hand.

int playerCardCount (Game game, int player) {
    return 0;
}


// !!!
// Get the number of cards in the current player's hand.
int handCardCount (Game game) {
    return 0;
}


// !!!
// View a card from the current player's own hand.
//
// The player should not need to free () this card,
// so you should not allocate or clone an existing card
// but give a reference to an existing card.

Card handCard (Game game, int card) {
    Card this;
    return this;
}


// !!!
// Get the card that is on the top of the discard pile.

Card topDiscard (Game game) {
    Card card;
    return card;
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

int isValidMove (Game game, playerMove move) {
    return 0;
}


// !!!
// Play the given action for the current player
//
// If the player makes the END_TURN move, their turn ends,
// and it becomes the turn of the next player.
//
// This should _not_ be called by the player AI.

void playMove (Game game, playerMove move) {
    move = move;
}


// !!!
// Check the game winner.
//
// Returns NOT_FINISHED if the game has not yet finished,
// 0-3, representing which player has won the game, or
// NO_WINNER if the game has ended but there was no winner.

int gameWinner (Game game) {
    return 0;
}


/* Remove a card in a link-list. This function returns a pointer
 * to the beginning of the "card_list". This pointer is important
 * so keep it rather than discard it, like so:
 *
 * my_list = card_list_free (my_list, nth);
 */

    // If this list is empty, skip this

    // If the first element, free it

    // For an element in the rest of the list, remove it and then bridge
    // the gap

struct card_list *card_list_remove (struct card_list *list, int nth) {

    if ((*list).next);
    else if (nth == 0) {
        struct card_list *next = (*list).next;
        free (list);
        list = next;
    } else {
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
