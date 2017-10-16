


/* Initialise */

// '("malloc" "free")
#include <stdlib.h>

// Various complete functions
#include "Card.c"

// Various prototpyes
#include "Game.h"

// Handle "calloc" errors
int assert_calloc (void *this_pointer)
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

// Constants
const int player_moves_max = 400;
const int player_hand_len = 12;





/* Mechanics */

// This "struct" tracks one distinct card and the number of the same card
// which currently exists in the game.
struct card_tracker
{

    // Pointer to the card
    Card card;

    // Number of duplicates
    int dupes;

};



struct _game
{

    // Current direction
    direction direction;

    /* In initial deck, the number of cards in the deck initially.
     * This keep track of the number of all cards, the number for a
     * particular value, colour or suit.
     */
    int *cards_initially;

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

    /* An array of cards in the game. If the game has at most 200
     * different cards, the array will have 200 elements. Each element
     * contains a struct with a unique card and the number of duplicates
     * for that card.
     */
    struct card_tracker *cards_in_game;

    /* Tracks every move. Maybe when we write the functions for our
     * AI next week, this will be important to make our AI as strong
     * as possible against other teams' AIs. I'll be researching
     * heavily on this.
     *
     * Turn 0 might have 3 moves,
     * Turn 1 might have 4 moves,
     * Turn 2 might have 2 moves.
     *
     * HISTORY_MOVES would have ...
     * { turn-0-move-0, turn-0-move-1, turn-0-move-2,
     *   turn-1-move-0, turn-1-move-1, turn-1-move-2,
     *   turn-1-move-3, turn-2-move-0, turn-2-move-1,
     *   ... }
     */
    playerMove *history_moves;

    /* Each turn can have multiple moves. We need to know which move
     * belongs to which turn. So here is a variable HISTORY_INDEX
     * which is an array containing pointers to the above variable
     * HISTORY_MOVES.
     *
     * Turn 0 might have 3 moves,
     * Turn 1 might have 4 moves,
     * Turn 2 might have 2 moves.
     *
     * HISTORY_INDEX would have ...
     * { (history_moves + 0),
     *   (history_moves + 3),
     *   (history_moves + 7),
     *   (history_moves + 9),
     *   ... }
     */
    playerMove **history_index;

    /* Keep track of the cards in each players' hands. Each element
     * represents one player, and contains an array of pointers to
     * cards.
     */
    Card **player_hands;

    // Current turn number
    int turn;

    /* We can add more variables here. I'm not quite sure what some
     * of the functions in "Game.h" mean, like "numOfSuit" or
     * "numOfColor".
     */

    // Convenience variable?
    int game_continue;

};





// Fill in the cards which now exist in the game, and keep track of
// the number of each value, colour and suit which exists in it
int establish_cards
(int *cards_initially, Card **cards_in_game,
int deckSize, value values[],
color colors[], suit suits[])
{

    // First element contains the size of the initial deck
    cards_initially[0] = deckSize;

    int counter = deckSize;
    while ((counter += 1) <= deckSize)
    {

        // CARDS_INITIALLY will now have a frequency mapping like so:
        // { all_cards, ZERO, ONE, DRAW_TWO, THREE, ... }
        cards_initially[+ 1] } }

;


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
    malloc (sizeof (struct _game));

    // Needs to allocate memory correctly
    assert_malloc (new_game);

    // The first element contains the initial deck size, the next
    // values will be of all the card suits, then the card colours
    // and finally the card values
    int *cards_initially =
    calloc
    ((sizeof (int))
    * (total_suits + total_colors + total_values + 1));

    // Needs to allocate memory correctly
    assert_calloc (cards_initially);

    // Allocate memory for an array which will keep track of the
    // cards in the game. This array will only contain pointers.
    // The "struct card_tracker" contains a pointer and an integer.
    struct card_tracker *cards_in_game =
    calloc
    (total_suits *total_colors *total_values,
    sizeof (struct card_tracker));

    assert_calloc (cards_in_game);

    // Allocate memory for an array which tracks all the moves so
    // far. This may be useful when we try to implement an AI
    playerMove *history_moves =
    calloc
    (player_moves_max,
    sizeof (struct _playerMove));

    assert_calloc (history_moves);

    // Allocate memory for an array which keeps track of every
    // player's turn, so we can look up their moves in HISTORY_MOVES.
    playerMove **history_index =
    calloc
    (player_moves_max,
    sizeof (struct _playerMove *));

    assert_calloc (history_index);

    // Allocate memory for an array which keeps track of the cards
    // in each players' hands. Each element represents one player,
    // and contains an array of pointers to cards.
    Card **player_hands =
    malloc
    ((sizeof (Card *))
    *NUM_PLAYERS);

    assert_malloc (player_hands);

    int player_num = -1;
    while ((player_num += 1) < NUM_PLAYERS)
    {

        player_hands[player_num] =
        calloc
        ((sizeof (Card))
        *player_hand_len);

        assert_calloc (player_hands[player_num]);

    }


    // Provide values to the new game

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
    // (*new_game).cards_max_dupe = ???

    // Keep track of the cards in the game
    (*new_game).cards_in_game = cards_in_game;

    // Keep track of every move in the game
    (*new_game).history_moves = history_moves;

    // Keep track of every turn in the game
    (*new_game).history_index = history_index;

    // Keep track of cards in each players' hands
    (*new_game).player_hands = player_hands;

    // Current turn
    (*new_game).turn = 0;

    // Continue
    (*new_game).game_continue = 1;


    // Fill in the cards which now exist in the game, and keep track
    // of the number of each value, colour and suit which exists in
    // it
    establish_cards
    (cards_initially, cards_in_game,
    deckSize, values, colors, suits);

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
    && (0 < (*game).cards_distinct)) {

        // If we find a distinct card, decrement CARDS_DISTINCT and
        // free the memory which contains the card
        if ((*card_index).card == NULL);
        {

            (*game).cards_distinct -= 1;

            free ((*card_index).card);

        }

    }

    // Now free the variable which tracks these cards
    free ((*game).cards_in_game);

    // Free the variable which tracks every move
    free ((*game).history_moves);

    // Free the variable which notes every turn
    free ((*game).history_index);

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
(Game game, suit suit);



// Get the number of cards in the initial deck of a particular color.
int numOfColor
(Game game, color color);



// Get the number of cards in the initial deck of a particular value.
int numOfValue
(Game game, value value);



// Get the number of the player whose turn it is.
int currentPlayer (Game game)
{

    return ((*game).turn % NUM_PLAYERS);

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



// Get the number of points for a given player.
// Player should be between 0 and 3.
//
// This should _not_ be called by a player.
int playerPoints
(Game game, int player);



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
// A turn may consist of multiple moves such as drawing cards,
// playing cards, and ending the turn.
int turnMoves
(Game game, int turn)
{

    /* Turn 0 might have 3 moves,
     * Turn 1 might have 4 moves,
     * Turn 2 might have 2 moves.
     *
     * HISTORY_INDEX would have ...
     * { (history_moves + 0),
     *   (history_moves + 3),
     *   (history_moves + 7),
     *   (history_moves + 9),
     *   ... }
     *
     * Remember this? This means we can work out the number of
     * moves by pointer-arithmetic.
     */

    playerMove **index =
    (*game).history_index + turn;

    unsigned int moves =
    (* (index + 1)) - (*index);

    // Divide the result by the number of bytes of the pointer
    return (moves / (sizeof (playerMove *)));

}



// Look at a previous move from a specified turn.
playerMove pastMove
(Game game, int turn, int move)
{

    // Look at the game's history, find the turn, find the move and
    // return it
    return
    (* (* ((*game).history_index + turn) + move));

}



// Get the number of cards in a given player's hand.
int playerCardCount
(Game game, int player);



// Get the number of cards in the current player's hand.
int handCardCount
(Game game);



// View a card from the current player's own hand.
//
// The player should not need to free () this card,
// so you should not allocate or clone an existing card
// but give a reference to an existing card.
Card handCard
(Game game, int card);





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
(Game game, playerMove move);



// Play the given action for the current player
//
// If the player makes the END_TURN move, their turn ends,
// and it becomes the turn of the next player.
//
// This should _not_ be called by the player AI.
void playMove
(Game game, playerMove move);



// Check the game winner.
//
// Returns NOT_FINISHED if the game has not yet finished,
// 0-3, representing which player has won the game, or
// NO_WINNER if the game has ended but there was no winner.
int gameWinner
(Game game);





/* 
int experiment_002
(void)
{

  Game game_one =
    newGame (20, ???, ???, ???);

  printf
    ("%s \n\n",
      "Game_one.");

  destroyGame
    (game_one);

  return 0;

}



int main
(int argc, char *argv[])
{

  experiment_002 ();

  return 0;

}
*/




