


/*  lloctan  */



/* Here are a series of tests to check all the possible functions in
 * "Card.c" and "Game.c" are working.
 */




#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Card.h"
#include "Game.h"

// #include "Card.c"
#include "Game.c"



/* Miscellane  */

// Print current values and apply assert to inputs
int assert_print
(int one, int two)
{

    printf
    ("%03d %03d \n",
    one, two);

    assert (one == two);

    return 0;

}

// If we want to robustly check that the card-making function
// is working, we can be pretty sure of its efficacy by generating
// a handful cards rather than hundreds of cards. Here, we
// establish such limits for making cards and making games.
int random_iterations_cards = 30;
int random_iterations_games = 4;

// Makes it predictable
int random_seed = 80;



// Convenient constants
color color_all[] =
{ RED, BLUE, GREEN, YELLOW, PURPLE };

char *color_names[] =
{[RED] = "RED",
    [BLUE] = "BLUE",
    [GREEN] = "GREEN",
    [YELLOW] = "YELLOW",
    [PURPLE] = "PURPLE" };

suit suit_all[] =
{ HEARTS, DIAMONDS, CLUBS, SPADES, QUESTIONS };

char *suit_names[] =
{[HEARTS] = "HEARTS",
    [DIAMONDS] = "DIAMONDS",
    [CLUBS] = "CLUBS",
    [SPADES] = "SPADES",
    [QUESTIONS] = "QUESTIONS" };

value value_all[] =
{ ZERO, ONE, DRAW_TWO, THREE, FOUR, FIVE, SIX,
    SEVEN, EIGHT, NINE, ADVANCE, BACKWARDS, CONTINUE,
    DECLARE, E, F };

char *value_names[] =
{[ZERO] = "ZERO",
    [ONE] = "ONE",
    [DRAW_TWO] = "DRAW_TWO",
    [THREE] = "THREE",
    [FOUR] = "FOUR",
    [FIVE] = "FIVE",
    [SIX] = "SIX",
    [SEVEN] = "SEVEN",
    [EIGHT] = "EIGHT",
    [NINE] = "NINE",
    [ADVANCE] = "ADVANCE",
    [BACKWARDS] = "BACKWARDS",
    [CONTINUE] = "CONTINUE",
    [DECLARE] = "DECLARE",
    [E] = "E",
    [F] = "F" };

direction direction_all[] =
{ CLOCKWISE, ANTICLOCKWISE };

char *direction_names[] =
{[CLOCKWISE] = "CLOCKWISE",
    [ANTICLOCKWISE] = "ANTICLOCKWISE" };

action action_all[] =
{ DRAW_CARD, PLAY_CARD, SAY_UNO, SAY_DUO, SAY_TRIO,
    END_TURN };

char *action_names[] =
{[DRAW_CARD] = "DRAW_CARD",
    [PLAY_CARD] = "PLAY_CARD",
    [SAY_UNO] = "SAY_UNO",
    [SAY_DUO] = "SAY_DUO",
    [SAY_TRIO] = "SAY_TRIO",
    [END_TURN] = "END_TURN" };





// For "Card.c"
#define color_max 5
#define suit_max 5
#define value_max 16

// For "Game.c"
#define direction_max 2
#define action_max 6
#define deck_size 52
#define in_hand_initially 7




/* "Card.c"  */

// Ensure that our "enum"s are working properly
int card_enum_color
(void)
{

    printf
    ("%s \n\n",
    "card_enum_color");

    int one = color_max;
    while (0 <= (one -= 1))
    {
        assert_print
        (one, (int) color_all[one]);
    }

    printf
    ("\n\n\n");

    return 0;

}

int card_enum_suit
(void)
{
    printf
    ("%s \n\n",
    "card_enum_suit");

    int one = suit_max;
    while (0 <= (one -= 1))
    {
        assert_print
        (one, (int) suit_all[one]);
    }

    printf
    ("\n\n\n");

    return 0;

}

int card_enum_value
(void)
{

    printf
    ("%s \n\n",
    "card_enum_value");

    int one = value_max;
    while (0 <= (one -= 1))
    {
        assert_print
        (one, (int) value_all[one]);
    }

    printf
    ("\n\n\n");

    return 0;

}



int card_all
(void)
{

    // Variables for making random cards
    int color;
    int suit;
    int value;
    Card new_card;

    printf
    ("%s \n\n",
    "card_new_and_destroy_card");

    srandom (random_seed);
    int count_cards = random_iterations_cards;
    while (0 <= (count_cards -= 1))
    {

        color = (int) (random () % color_max);
        suit = (int) (random () % suit_max);
        value = (int) (random () % value_max);

        new_card =
        newCard
        (value_all[value],
        color_all[color],
        suit_all[suit]);

        printf
        ("%s %d \n",
        "Before \"free\", NEW_CARD is in location ...",
        (int) new_card);

        printf
        ("%s \n%s %s %s \n"
        "%03d %03d %03d \n%03d %03d %03d \n",
        "Input and output.",
        color_names[color],
        suit_names[suit],
        value_names[value],
        color, suit, value,
        (int) (*new_card).card_color,
        (int) (*new_card).card_suit,
        (int) (*new_card).card_value);

        assert (color == (int) cardColor (new_card));
        assert (suit == (int) cardSuit (new_card));
        assert (value == (int) cardValue (new_card));

        free
        (new_card);

        printf
        ("%s %d \n",
        "After \"free\", this location is now free ...",
        (int) new_card);

        printf
        ("\n\n");

    }

    printf
    ("\n");

    return 0;

}



int card_main
(void)
{

    card_enum_color ();
    card_enum_suit ();
    card_enum_value ();

    card_all ();

    return 0;

}





/* Game  */

// Ensure that our "enum"s are working properly
int game_enum_direction
(int one)
{

    printf
    ("%s \n\n",
    "game_enum_direction");

    while (0 <= (one -= 1))
    {
        assert_print
        (one, (int) direction_all[one]);
    }

    printf
    ("\n\n\n");

    return 0;

}

int game_enum_action
(int one)
{

    printf
    ("%s \n\n",
    "game_enum_action");

    while (0 <= (one -= 1))
    {
        assert_print
        (one, (int) action_all[one]);
    }

    printf
    ("\n\n\n");

    return 0;

}



// Before we make a new game, try out the functions which relate
// to making new cards for the deck
int game_make_deck_basic
(color deck_colors[deck_size],
suit deck_suits[deck_size],
value deck_values[deck_size],
int deck_color_tracker[color_max],
int deck_suit_tracker[suit_max],
int deck_value_tracker[value_max])
{

    // All of them are ("RED" "HEARTS" "ONE")
    int color = 0;
    int suit = 0;
    int value = 0;

    deck_color_tracker[color] = deck_size;
    deck_suit_tracker[suit] = deck_size;
    deck_value_tracker[value] = deck_size;

    // Make a proper deck
    int count_cards = deck_size;

    // For the sake of verifying the cards, print the deck
    while (0 <= (count_cards -= 1))
    {

        printf
        ("%03d %03d %03d     %s %s %s \n",
        color, suit, value,
        color_names[color],
        suit_names[suit],
        value_names[value]);

    }

    printf
    ("\n");

    return 0;

}

int game_make_deck_numbers_only
(int game_number,
color deck_colors[deck_size],
suit deck_suits[deck_size],
value deck_values[deck_size],
int deck_color_tracker[color_max],
int deck_suit_tracker[suit_max],
int deck_value_tracker[value_max])
{

    // Variables for making random cards
    int color;
    int suit;
    int value;

    // For error-checking, make it predictable
    srandom (random_seed + game_number);

    // Make a proper deck
    int count_cards = deck_size;

    // But! For our purposes of testing, we need a few important

    while (0 <= (count_cards -= 1))
    {

        // Assigning random values for these three properties
        // of each card in the deck
        color = (int) (random () % color_max);
        suit = (int) (random () % suit_max);
        value = (int) (random () % value_max);

        // Increment the counter which tracks these properties
        deck_color_tracker[color] += 1;
        deck_suit_tracker[suit] += 1;
        deck_value_tracker[value] += 1;

        // Apply to the card in the deck
        deck_colors[count_cards] = color_all[color];
        deck_suits[count_cards] = suit_all[suit];
        deck_values[count_cards] = value_all[value];

        printf
        ("%03d %03d %03d     %s %s %s \n",
        color, suit, value,
        color_names[color],
        suit_names[suit],
        value_names[value]);

    }

    printf
    ("\n");

    return 0;

}


// Once we have a new game, check that the functions relating
// to the cards in the deck are working
int game_check_deck
(Game new_game,
color deck_colors[deck_size],
suit deck_suits[deck_size],
value deck_values[deck_size],
int deck_color_tracker[color_max],
int deck_suit_tracker[suit_max],
int deck_value_tracker[value_max])
{

    // Acutally not sure about this!! Does "initial deck" mean
    // before or after dealing cards to the players?
    printf
    ("%s %03d %03d \n",
    "Number of cards in the deck.",
    deck_size, numCards (new_game));

    assert
    (deck_size == numCards (new_game));

    printf
    ("%s \n",
    "Number of cards in the deck which are ...");

    int count = color_max;
    while (0 <= (count -= 1))
    {

        printf
        ("%s %s. %03d %03d \n",
        "  ... ", (color_names[count]),
        deck_color_tracker[count],
        numOfColor (new_game, color_all[count]));

        assert
        (deck_color_tracker[count] ==
        numOfColor (new_game, color_all[count]));

    }

    count = suit_max;
    while (0 <= (count -= 1))
    {

        printf
        ("%s %s. %03d %03d \n",
        "  ... ", (suit_names[count]),
        deck_suit_tracker[count],
        numOfSuit (new_game, suit_all[count] ));

        assert
        (deck_suit_tracker[count] ==
        numOfSuit (new_game, suit_all[count]));

    }

    count = value_max;
    while (0 <= (count -= 1))
    {

        printf
        ("%s %s. %03d %03d \n",
        "  ... ", (value_names[count]),
        deck_value_tracker[count],
        numOfValue (new_game, value_all[count]));

        assert
        (deck_value_tracker[count] ==
        numOfValue (new_game, value_all[count]));

    }

    printf
    ("\n");

    return 0;

}

// When we call "newGame", from the deck, each player receives
// seven cards and the next card on the deck becomes the first in
// the discard pile. Confirm this
int game_check_hands_and_discards
(Game new_game,
color deck_colors[deck_size],
suit deck_suits[deck_size],
value deck_values[deck_size])
{

    // Variables for cards
    int color_from_ini_deck;
    int suit_from_ini_deck;
    int value_from_ini_deck;

    int color_from_hand;
    int suit_from_hand;
    int value_from_hand;

    // Check all the cards in each players' hands and make sure that
    // they match those in the deck
    int in_hand_index;
    Card in_hand_card;

    printf
    ("%s \n%s \n%s %03d %s \n\n",
    "game_check_hands_and_discards",
    "Check the cards in hands match those in initial deck.",
    "We expect", in_hand_initially, "initial cards.");

    int player = NUM_PLAYERS;
    while (0 <= (player -= 1))
    {

        printf
        ("%s %03d %s %03d %s \n%s \n",
        "Player", player, "has ", handCardCount (new_game), "cards.",
        "Below are pairs of cards from initial deck and from the hand.");

        // In the beginning, make sure each player has seven cards
        assert
        (in_hand_initially ==
        handCardCount (new_game));

        // Now check that cards in hands match those corresponding
        // in the initial deck
        in_hand_index = in_hand_initially;
        while (0 <= (in_hand_index -= 1))
        {

            // Retrieve the properties of the card from the initial deck
            color_from_ini_deck =
            deck_colors
            [in_hand_initially * player + in_hand_index];

            suit_from_ini_deck =
            deck_suits
            [in_hand_initially * player + in_hand_index];

            value_from_ini_deck =
            deck_values
            [in_hand_initially * player + in_hand_index];

            // With "handCard" function, retrieve the card and then its
            // properties
            in_hand_card =
            handCard (new_game, in_hand_index);

            color_from_hand =
            (*in_hand_card).card_color;

            suit_from_hand =
            (*in_hand_card).card_suit;

            value_from_hand =
            (*in_hand_card).card_value;

            // Compare the properties
            printf
            ("%s %03d %s %s %s %s %s %s %s %s %s \n",
            "  ... ", in_hand_index, ". (",
            (color_names[color_from_ini_deck]),
            (suit_names[suit_from_ini_deck]),
            (value_names[value_from_ini_deck]),
            ") (",
            (color_names[color_from_hand]),
            (suit_names[suit_from_hand]),
            (value_names[value_from_hand]),
            ")");

            assert (color_from_ini_deck == color_from_hand);
            assert (suit_from_ini_deck == suit_from_hand);
            assert (value_from_ini_deck == value_from_hand);

        }

        printf
        ("\n");

    }

    // Now check the first card in the discard pile. It should be the
    // first card in the deck after distributing cards to players
    color_from_ini_deck =
    deck_colors[in_hand_initially * NUM_PLAYERS];

    suit_from_ini_deck =
    deck_suits[in_hand_initially * NUM_PLAYERS];

    value_from_ini_deck =
    deck_values[in_hand_initially * NUM_PLAYERS];

    Card card_in_discard = topDiscard (new_game);

    printf
    ("%s \n%s %s %s %s %s %s %s %s %s \n",
    "The first card in the discard pile.",
    "  ... (",
    color_names[color_from_ini_deck],
    suit_names[suit_from_ini_deck],
    value_names[value_from_ini_deck],
    ") (",
    color_names[(*card_in_discard).card_color],
    suit_names[(*card_in_discard).card_suit],
    value_names[(*card_in_discard).card_value],
    ")");

    assert
    (deck_colors[in_hand_initially * NUM_PLAYERS] ==
    (*card_in_discard).card_color);

    assert
    (deck_suits[in_hand_initially * NUM_PLAYERS] ==
    (*card_in_discard).card_suit);

    assert
    (deck_values[in_hand_initially * NUM_PLAYERS] ==
    (*card_in_discard).card_value);

    printf
    ("\n");

    return 0;

}

int assert_turn_and_player
(Game new_game, int turn, int player)
{

    // Print the current turn and make sure "currentTurn" returns correctly
    printf
    ("%s %03d %03d \n",
    "Current turn.",
    turn, currentTurn (new_game));

    assert
    (turn == currentTurn (new_game));

    // Similarly for current player
    printf
    ("%s %03d %03d \n",
    "Current player.",
    player, currentPlayer (new_game));

    assert
    (player == currentPlayer (new_game));

    return 0;

}

int game_announce_check_and_make_move
(Game new_game, playerMove move)
{

    if (move.action == END_TURN)
    {

        printf
        ("%s \n\n", "END_TURN");

    }
    else if (move.action == PLAY_CARD)
    {

        Card card = move.card;

        printf
        ("%s %s %s %s %s %s \n",
        action_names[move.action],
        "(",
        (color_names[(*card).card_color]),
        (suit_names[(*card).card_suit]),
        (value_names[(*card).card_value]),
        ")");

    }
    else if (move.action == DECLARE)
    {

        Card card = move.card;

        printf
        ("%s %s %s \n",
        action_names[move.action],
        "DECLARE",
        color_names[(*card).card_color]);

    }
    else
    {

        printf
        ("%s \n",
        action_names[move.action]);

    }

    assert
    (isValidMove (new_game, move) == 1);

    playMove (new_game, move);

    return 0;

}





int game_play_basic
(Game new_game)
{

    playerMove move;
    int cards_in_hand;
    int player = 0;
    int turn = -1;

    // Only when the most recent player has zero cards left in
    // their hand does the game end.

    while
    ((0 < (cards_in_hand = handCardCount (new_game)))
    && (0 <= (turn += 1)))
    {

        // Because all cards are ("RED" "HEARTS" "ONE"), we
        // know that the direction is always "CLOCKWISE"
        player = (turn % NUM_PLAYERS);

        // On turn 0, player 0, the game begins
        printf
        ("%s %03d %s %03d %s \n",
        "Turn", turn, "and player", player, ".");

        assert_turn_and_player
        (new_game, turn, player);

        // Our move will always be the first card
        // ("RED" "HEARTS" "ONE").
        // But if no cards left, end turn
        if (0 < cards_in_hand)
        {

            if (cards_in_hand == 3)
            {

                move.action = SAY_TRIO;
                game_announce_check_and_make_move
                (new_game, move);

            }
            else if (cards_in_hand == 2)
            {

                move.action = SAY_DUO;
                game_announce_check_and_make_move
                (new_game, move);

            }
            else if (cards_in_hand == 1)
            {

                move.action = SAY_UNO;
                game_announce_check_and_make_move
                (new_game, move);

            }

            // Always the first card!
            move.action = PLAY_CARD;
            move.card = handCard (new_game, 0);
            game_announce_check_and_make_move
            (new_game, move);

            // History should now contain this move. We should
            // verify it.Look at the first move. Does it match
            // MOVE?
            playerMove in_pastMove =
            pastMove (new_game, turn, 0);

            assert
            ((& move) == (& in_pastMove));

        }

        move.action = END_TURN;
        game_announce_check_and_make_move
        (new_game, move);

    }

    return 0;

}

int game_all_basic
(void)
{

    // Variables for making random games
    color deck_colors[deck_size];
    suit deck_suits[deck_size];
    value deck_values[deck_size];
    Game new_game;

    // Variables to keep track the frequency of colours, suits
    // and values in the deck
    int deck_color_tracker[color_max] = { 0 };
    int deck_suit_tracker[suit_max] = { 0 };
    int deck_value_tracker[value_max] = { 0 };

    // Miscellane
    int turn;
    int player;

    printf
    ("%s \n\n",
    "game_new_and_destroy_game");

    // Make one game, all cards are ("RED" "HEARTS" "ONE")
    printf
    ("%s %03d \n%s \n", "New game.",
    0, "Deck of cards.");

    // For checking deck-making functions
    game_make_deck_basic
    (deck_colors, deck_suits, deck_values,
    deck_color_tracker,
    deck_suit_tracker,
    deck_value_tracker);

    new_game =
    newGame
    (deck_size, deck_values, deck_colors, deck_suits);

    printf
    ("%s %d \n",
    "Before \"free\", NEW_GAME is in location ...",
    (int) new_game);

    // For checking deck-values in a new game. With "newGame", did we
    // initialise properly?
    game_check_deck
    (new_game,
    deck_colors, deck_suits, deck_values,
    deck_color_tracker,
    deck_suit_tracker,
    deck_value_tracker);

    // When we call "newGame", from the deck, each player receives
    // seven cards and the next card on the deck becomes the first in
    // the discard pile. Confirm this
    game_check_hands_and_discards
    (new_game,
    deck_colors, deck_suits, deck_values);

    // Now, we play the game
    game_play_basic
    (new_game);

    // How many cards are in other players' hand
    printf
    ("%s \n",
    "Cards in players hands.");

    player = NUM_PLAYERS;
    while (0 <= (player -= 1))
    {

        printf
        ("%s %03d %s %03d %s \n",
        "  ...", player, "has",
        playerCardCount (new_game, player), ".");

    }

    printf
    ("\n");

    // Who wins? The one who started, of course, player 0
    printf
    ("%s %03d %s \n",
    "Player", gameWinner (new_game), "wins!");

    assert
    (gameWinner (new_game) == 0);

    // Finish up
    destroyGame
    (new_game);

    printf
    ("%s %d \n",
    "After \"free\", NEW_GAME is in location ...",
    (int) new_game);

    printf
    ("\n\n\n");

    return 0;

}






int game_all_numbers_only
(void)
{

    // Variables for making random games
    color deck_colors[deck_size];
    suit deck_suits[deck_size];
    value deck_values[deck_size];
    Game new_game;

    // Variables to keep track the frequency of colours, suits
    // and values in the deck
    int deck_color_tracker[color_max] = { 0 };
    int deck_suit_tracker[suit_max] = { 0 };
    int deck_value_tracker[value_max] = { 0 };

    // Miscellane
    int turn;
    int player;

    printf
    ("%s \n\n",
    "game_new_and_destroy_game");

    int count_games = random_iterations_games;
    while (0 <= (count_games -= 1))
    {

        printf
        ("%s %03d \n%s \n", "New game.",
        count_games, "Deck of cards.");

        // For checking deck-making functions
        game_make_deck_numbers_only
        (count_games,
        deck_colors, deck_suits, deck_values,
        deck_color_tracker,
        deck_suit_tracker,
        deck_value_tracker);

        new_game =
        newGame
        (deck_size, deck_values, deck_colors, deck_suits);

        printf
        ("%s %d \n",
        "Before \"free\", NEW_GAME is in location ...",
        (int) new_game);

        // For checking deck-values in a new game
        // With "newGame", did we initialise properly?




        // Number of the player whose turn it is. Typically, this
        // would be (turn % NUM_PLAYERS), but because we also
        // need to consider "CLOCKWISE" and "ANTI-CLOCKWISE", we
        // need the game history to know
        turn = 0;



        destroyGame
        (new_game);

        printf
        ("%s %d \n",
        "After \"free\", NEW_GAME is in location ...",
        (int) new_game);

    }

    printf
    ("\n\n\n");

    return 0;

}





/* Patrick */



#define MAX_DECK 100
#define MIN_DECK 30

void num
(void);

void rngDeck
(Game game);

int patrick_main
(int argc, char *argv[])
{

    printf ("This is a test for the Game ADT\n");

    num ();

    printf ("All tests passed\n");

    return EXIT_SUCCESS;

}

void num
(void)
{

    value values[50];
    suit suits[50];
    color colors[50];

    int i = 0;

    while (i < 50) {
        values[i] = rand () % 16;
        suits[i] = HEARTS;
        colors[i] = RED;
        i++;
    }

    Game game = newgame (50, values, colors, suits);

    assert ((game -> cards_in_deck) == 50);

}


// this function generates a random deck to be used
void rngDeck
()
{

    // generating a random amount of cards
    int i = 0;
    int count = (rand () % (MAX_DECK + 1 - MIN_DECK) + MIN_DECK);

    while (i < count) {

        values[i] = (rand () % (15));
        // Hearts for now but will change once I implement a function to convert RNG numbers into string
        suits[i] = HEARTS;
        colors[i] = BLUE;
        i++;

    }
}





/* Main */




int main
(int argc, char *argv[])
{

    card_main ();

    printf
    ("%s \n\n\n",
    "Success!! =) ");

    return 0;

}


