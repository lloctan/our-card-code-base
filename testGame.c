


/* lloctan  */



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



// For verbose or silence, toggle here
#define verbose 1

#if verbose
# define conditional_print printf
#else
# define conditional_print ignore
int ignore
(char *format, ...)
{
    return 0;
}
#endif



// Print current values and apply assert to inputs
int assert_print
(int one, int two)
{

    conditional_print
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

    conditional_print
    ("%s \n\n",
    "card_enum_color");

    int one = color_max;
    while (0 <= (one -= 1))
    {
        assert_print
        (one, (int) color_all[one]);
    }

    conditional_print
    ("\n\n\n");

    return 0;

}

int card_enum_suit
(void)
{
    conditional_print
    ("%s \n\n",
    "card_enum_suit");

    int one = suit_max;
    while (0 <= (one -= 1))
    {
        assert_print
        (one, (int) suit_all[one]);
    }

    conditional_print
    ("\n\n\n");

    return 0;

}

int card_enum_value
(void)
{

    conditional_print
    ("%s \n\n",
    "card_enum_value");

    int one = value_max;
    while (0 <= (one -= 1))
    {
        assert_print
        (one, (int) value_all[one]);
    }

    conditional_print
    ("\n\n\n");

    return 0;

}



// Check that we can create new cards, obtain properties and
// free them afterwards. Repeat them RANDOM_ITERATIONS_CARDS
// times
int card_all
(void)
{

    // Variables for making random cards
    int color;
    int suit;
    int value;
    Card new_card;

    conditional_print
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

        conditional_print
        ("%s %d \n",
        "Before \"free\", NEW_CARD is in location ...",
        (int) new_card);

        conditional_print
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

        conditional_print
        ("%s %d \n",
        "After \"free\", this location is now free ...",
        (int) new_card);

        conditional_print
        ("\n\n");

    }

    conditional_print
    ("\n");

    return 0;

}



/* This is to test everything in "Card.c". This will check
 * that the enums are consistent and correct. This will
 * also create new cards, retrieve information about their
 * properties, like colour and suit, and free the cards.
 */
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

    conditional_print
    ("%s \n\n",
    "game_enum_direction");

    while (0 <= (one -= 1))
    {
        assert_print
        (one, (int) direction_all[one]);
    }

    conditional_print
    ("\n\n\n");

    return 0;

}

int game_enum_action
(int one)
{

    conditional_print
    ("%s \n\n",
    "game_enum_action");

    while (0 <= (one -= 1))
    {
        assert_print
        (one, (int) action_all[one]);
    }

    conditional_print
    ("\n\n\n");

    return 0;

}



// Before we make a new game, try out the functions which relate
// to making new cards for the deck. This is for the case of
// one distinct card ("RED" "HEARTS" "ONE")
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

        conditional_print
        ("%03d %03d %03d     %s %s %s \n",
        color, suit, value,
        color_names[color],
        suit_names[suit],
        value_names[value]);

    }

    conditional_print
    ("\n");

    return 0;

}

// Before we make a new game, try out the functions which relate
// to making new cards for the deck. This is for the case of
// typical cards, and without effects
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
    // details to keep track of, like the number of cards of a
    // particular colour
    while (0 <= (count_cards -= 1))
    {

        // Assigning random values for these three properties
        // of each card in the deck
        color = (int) (random () % color_max);
        suit = (int) (random () % suit_max);

        // VALUE needs to be a number. So we exclude special
        // effects, like DRAW_TWO and ADVANCE
        while
        ((value = (int) (random () % value_max))
        && ((value == 2) || (9 < value)));

        // Increment the counter which tracks these properties
        deck_color_tracker[color] += 1;
        deck_suit_tracker[suit] += 1;
        deck_value_tracker[value] += 1;

        // Apply to the card in the deck
        deck_colors[count_cards] = color_all[color];
        deck_suits[count_cards] = suit_all[suit];
        deck_values[count_cards] = value_all[value];

        conditional_print
        ("%03d %03d %03d     %s %s %s \n",
        color, suit, value,
        color_names[color],
        suit_names[suit],
        value_names[value]);

    }

    conditional_print
    ("\n");

    return 0;

}

// Before we make a new game, try out the functions which relate
// to making new cards for the deck. This is for the case of
// all kinds of cards, with or without effects
int game_make_deck_with_effects
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
    // details to keep track of, like the number of cards of a
    // particular colour
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

        conditional_print
        ("%03d %03d %03d     %s %s %s \n",
        color, suit, value,
        color_names[color],
        suit_names[suit],
        value_names[value]);

    }

    conditional_print
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
    conditional_print
    ("%s \n%s %03d %03d \n",
    "game_check_deck",
    "Number of cards in the deck.",
    deck_size, numCards (new_game));

    assert
    (deck_size == numCards (new_game));

    conditional_print
    ("%s \n",
    "Number of cards in the deck which are ...");

    int count = color_max;
    while (0 <= (count -= 1))
    {

        conditional_print
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

        conditional_print
        ("%s %s. %03d %03d \n",
        "  ... ", (suit_names[count]),
        deck_suit_tracker[count],
        numOfSuit (new_game, suit_all[count]));

        assert
        (deck_suit_tracker[count] ==
        numOfSuit (new_game, suit_all[count]));

    }

    count = value_max;
    while (0 <= (count -= 1))
    {

        conditional_print
        ("%s %s. %03d %03d \n",
        "  ... ", (value_names[count]),
        deck_value_tracker[count],
        numOfValue (new_game, value_all[count]));

        assert
        (deck_value_tracker[count] ==
        numOfValue (new_game, value_all[count]));

    }

    conditional_print
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

    conditional_print
    ("%s \n%s \n%s %03d %s \n\n",
    "game_check_hands_and_discards",
    "Check the cards in hands match those in initial deck.",
    "We expect", in_hand_initially, "initial cards.");

    int player = NUM_PLAYERS;
    while (0 <= (player -= 1))
    {

        conditional_print
        ("%s %03d %s %03d %s \n%s \n",
        "Player", player, "has ",
        handCardCount (new_game), "cards.",
        "Below are pairs of cards from "
        "initial deck and from the hand.");

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
            conditional_print
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

        conditional_print
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

    conditional_print
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

    conditional_print
    ("\n");

    return 0;

}

// This function advances the current player
int next_player
(direction direction, int current_player)
{

    if (direction == ANTICLOCKWISE)
    {
        if (current_player == 0)
        {
            current_player = NUM_PLAYERS - 1;
        }
        else
        {
            current_player -= 1;
        }
    }
    else
    {
        if (current_player == NUM_PLAYERS - 1)
        {
            current_player = 0;
        }
        else
        {
            current_player += 1;
        }
    }

    return current_player;

}

// This makes sure that during the game, we have the correct turn and
// player. Requires feeding the correct TURN and PLAYER into inputs
int assert_turn_and_player
(Game new_game, int turn, int player)
{

    // Print the current turn and make sure "currentTurn" returns
    // correctly
    conditional_print
    ("%s %03d %03d \n",
    "Current turn.",
    turn, currentTurn (new_game));

    assert
    (turn == currentTurn (new_game));

    // Similarly for current player
    conditional_print
    ("%s %03d %03d \n",
    "Current player.",
    player, currentPlayer (new_game));

    assert
    (player == currentPlayer (new_game));

    return 0;

}

// This function makes sure that during the game, the n-th card in a
// player's hand is indeed the one from "handCard". Requires feeding
// inputs. Also retrieve the card from "handCard"
Card assert_and_retrieve_card
(Game new_game, int player, int card_num)
{

    Card actual_card =
    (* ((*new_game).player_hands[player])).card;

    Card function_card =
    handCard (new_game, card_num);

    conditional_print
    ("%s %03d%s %03d %s \n%s %s %s %s %s \n",
    "In the hand of player", player, ", card number", card_num,
    "is ...", "  ... (",
    color_names[(*actual_card).card_color],
    suit_names[(*actual_card).card_suit],
    value_names[(*actual_card).card_value],
    ") (");

    assert
    (actual_card == function_card);

    return function_card;

}

// This function prints all the valid cards to play in
// PLAYER's hand. It evaluates the best possible move and
// returns the index to that card in the hand
int best_value_move
(Game new_game, int player)
{

    return 0;

}




// This function makes combines three tasks. It announces the move
// to play, it checks that the move is valid, and then plays it for us
int game_announce_check_and_make_move
(Game new_game, playerMove move)
{

    if (move.action == END_TURN)
    {

        conditional_print
        ("%s \n\n", "END_TURN");

    }
    else if (move.action == PLAY_CARD)
    {

        Card card = move.card;

        conditional_print
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

        conditional_print
        ("%s %s %s \n",
        action_names[move.action],
        "DECLARE",
        color_names[(*card).card_color]);

    }
    else
    {

        conditional_print
        ("%s \n",
        action_names[move.action]);

    }

    assert
    (isValidMove (new_game, move) == 1);

    playMove (new_game, move);

    return 0;

}

int assert_card_count_hands
(Game new_game, int *count_array)
{

    conditional_print
    ("%s \n%s \n", "assert_card_count_hands",
    "Below are player number, actual number of cards left \n"
    "and the number from \"playerCardCount\".");

    // First, get the array which contains players' hands
    struct card_list **player_hands_array =
    (*new_game).player_hands;

    struct card_list *player_hand;
    int from_function;

    // For each player, go through their hand and count the
    // number of cards they have
    int player = NUM_PLAYERS;
    while (0 <= (player -= 1))
    {

        player_hand = player_hands_array[player];

        // So, stop when PLAYER_HAND is empty
        while
        (player_hand
        && (count_array[player] += 1)
        && (player_hand = (*player_hand).next));

        from_function =
        playerCardCount (new_game, player);

        conditional_print
        ("  ... %03d %s %03d %03d \n",
        player, "has",
        count_array[player],
        from_function);

        assert
        (count_array[player] == from_function);

    }

    conditional_print
    ("\n");

    return 0;

}

// This function ensures that we know the winner correctly
int assert_winner
(Game new_game, int winner)
{

    conditional_print
    ("%s %03d %s \n",
    "Player", gameWinner (new_game), "wins!");

    assert
    (gameWinner (new_game) == winner);

    conditional_print
    ("\n");

    return 0;

}





// This simulates a game where all the cards are the same
// ("RED" "HEARTS" ONE"). Returns the winner of the game
int game_play_basic
(Game new_game)
{

    playerMove move;
    int cards_in_hand;
    int player = -1;
    int turn = -1;

    // Only when the most recent player has zero cards left in
    // their hand does the game end.
    while
    ((0 < (cards_in_hand = handCardCount (new_game)))
    && (0 <= (turn += 1)))
    {

        // Because all cards are ("RED" "HEARTS" "ONE"), we know
        // that the direction is always "CLOCKWISE"
        player = next_player (CLOCKWISE, player);

        // On turn 0, player 0, the game begins
        conditional_print
        ("%s %03d %s %03d %s \n",
        "Turn", turn, "and player", player, ".");

        assert_turn_and_player
        (new_game, turn, player);

        // Our move will always be ("RED" "HEARTS" "ONE").
        // So, choose the first card. But if no cards left, end turn
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
            move.card =
            assert_and_retrieve_card
            (new_game, player, 0);
            game_announce_check_and_make_move
            (new_game, move);

            // History should now contain this move. We should
            // verify it. Look at the first move. Does it match
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

    return player;

}

// This does everything to create a game and simulate it
// All the cards are ("RED" "HEARTS" "ONE")
int game_all_basic
(void)
{

    // Variables for making random games
    // Note, initialising to zero for ("RED" "HEARTS" "ONE")
    color deck_colors[deck_size] = { 0 };
    suit deck_suits[deck_size] = { 0 };
    value deck_values[deck_size] = { 0 };
    Game new_game;

    // Variables to track the frequency of colours, suits
    // and values in the deck
    int deck_color_tracker[color_max] = { 0 };
    int deck_suit_tracker[suit_max] = { 0 };
    int deck_value_tracker[value_max] = { 0 };

    // Miscellane
    int turn;
    int player;
    int winner;
    int cards_in_hands[NUM_PLAYERS] = { 0 };

    conditional_print
    ("%s \n\n",
    "game_new_and_destroy_game");

    // Make one game, all cards are ("RED" "HEARTS" "ONE")
    conditional_print
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

    conditional_print
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
    winner = game_play_basic (new_game);

    // How many cards are in other players' hand
    assert_card_count_hands
    (new_game, cards_in_hands);

    // Who wins? The one who started, of course, player 0
    assert_winner
    (new_game, winner);

    // Finish up
    destroyGame
    (new_game);

    conditional_print
    ("%s %d \n",
    "After \"free\", NEW_GAME is in location ...",
    (int) new_game);

    conditional_print
    ("\n\n\n");

    return 0;

}





int game_play_all_numbers
(Game new_game)
{

    playerMove move;
    int cards_in_hand;
    int player = -1;
    int turn = -1;

    // Only when the most recent player has zero cards left in
    // their hand does the game end.
    while
    ((0 < (cards_in_hand = handCardCount (new_game)))
    && (0 <= (turn += 1)))
    {

        // Because all cards are without effects, we know
        // that the direction is always "CLOCKWISE"
        player = next_player (CLOCKWISE, player);

        // On turn 0, player 0, the game begins
        conditional_print
        ("%s %03d %s %03d %s \n",
        "Turn", turn, "and player", player, ".");

        assert_turn_and_player
        (new_game, turn, player);

        // Our move will always be ("RED" "HEARTS" "ONE").
        // So, choose the first card. But if no cards left, end turn
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
            move.card =
            assert_and_retrieve_card
            (new_game, player, 0);
            game_announce_check_and_make_move
            (new_game, move);

            // History should now contain this move. We should
            // verify it. Look at the first move. Does it match
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

    return player;

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
    int winner;
    int cards_in_hands[NUM_PLAYERS] = { 0 };

    conditional_print
    ("%s \n\n",
    "game_new_and_destroy_game");

    int count_games = random_iterations_games;
    while (0 <= (count_games -= 1))
    {

        conditional_print
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

        conditional_print
        ("%s %d \n",
        "Before \"free\", NEW_GAME is in location ...",
        (int) new_game);

        // For checking deck-values in a new game
        // With "newGame", did we initialise properly?
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
        winner = game_play_all_numbers (new_game);

        // How many cards are in other players' hand
        assert_card_count_hands
        (new_game, cards_in_hands);

        // Who wins? The one who started, of course, player 0
        assert_winner
        (new_game, winner);

        destroyGame
        (new_game);

        conditional_print
        ("%s %d \n",
        "After \"free\", NEW_GAME is in location ...",
        (int) new_game);

    }

    conditional_print
    ("\n\n\n");

    return 0;

}





/* Patrick */


/* 
 * #define MAX_DECK 100
 * #define MIN_DECK 30
 *
 * void num
 * (void);
 *
 * void rngDeck
 * (Game game);
 *
 * int patrick_main
 * (int argc, char *argv[])
 * {
 *
 *   conditional_print ("This is a test for the Game ADT\n");
 *
 *   num ();
 *
 *   conditional_print ("All tests passed\n");
 *
 *   return EXIT_SUCCESS;
 *
 * }
 *
 * void num
 * (void)
 * {
 *
 *   value values[50];
 *   suit suits[50];
 *   color colors[50];
 *
 *   int i = 0;
 *
 *   while (i < 50) {
 *     values[i] = rand () % 16;
 *     suits[i] = HEARTS;
 *     colors[i] = RED;
 *     i++;
 *   }
 *
 *   Game game = newgame (50, values, colors, suits);
 *
 *   assert ((game -> cards_in_deck) == 50);
 *
 * }
 *
 *
 * // this function generates a random deck to be used
 * void rngDeck
 * (void)
 * {
 *
 *   // generating a random amount of cards
 *   int i = 0;
 *   int count = (rand () % (MAX_DECK + 1 - MIN_DECK) + MIN_DECK);
 *
 *   while (i < count) {
 *
 *     values[i] = (rand () % (15));
 *     // Hearts for now but will change once I implement a function to convert RNG numbers into string
 *     suits[i] = HEARTS;
 *     colors[i] = BLUE;
 *     i++;
 *
 *   }
 * }
 */




/* Main */




int main
(int argc, char *argv[])
{

    card_main ();

    game_all_basic ();

    game_all_numbers_only ();

    printf
    ("%s \n\n\n",
    "Success!! =) ");

    return 0;

}




