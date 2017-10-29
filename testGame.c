


#include <assert.h>

#include "Card.h"
#include "Game.h"

#define deck_len 52
#define cards_hand_initially 7




struct _card
{
    color color;
    suit suit;
    value value;
};





int main_basic
(void)
{

    color color[deck_len] = { RED };
    suit suit[deck_len] = { HEARTS };
    value value[deck_len] = { ONE };

    Game game =
    newGame (deck_len, value, color, suit);

    // Initial deck check
    assert (numCards (game) == deck_len);
    assert (numOfColor (game, RED) == deck_len);
    assert (numOfSuit (game, HEARTS) == deck_len);
    assert (numOfValue (game, ONE) == deck_len);

    assert (numOfColor (game, GREEN) == 0);
    assert (numOfSuit (game, DIAMONDS) == 0);
    assert (numOfValue (game, DRAW_TWO) == 0);


    // Check the first three cards of each players' hands
    int turns = -1;
    int cards_in_hand;
    playerMove past_move;
    playerMove move_one;
    playerMove move_two;
    Card discard_top;
    while ((turns += 1) < 12)
    {

        assert (playDirection (game) == CLOCKWISE);
        assert (currentTurn (game) == turns);
        assert (currentPlayer (game) == (turns % NUM_PLAYERS));

        // Number of cards is how many they get subtract
        // the number of turns they've had
        assert
        (handCardCount (game) ==
        cards_hand_initially - (turns / NUM_PLAYERS));

        move_one.card = handCard (game, 0);
        assert ((* (move_one.card)).color == RED);
        assert ((* (move_one.card)).suit == HEARTS);
        assert ((* (move_one.card)).value == ONE);

        move_one.action = PLAY_CARD;
        assert (isValidMove (game, move_one) == 1);

        playMove (game, move_one);

        discard_top = topDiscard (game);
        assert ((*discard_top).color == (* (move_one.card)).color);
        assert ((*discard_top).suit == (* (move_one.card)).suit);
        assert ((*discard_top).value == (* (move_one.card)).value);

        move_two.action = END_TURN;
        playMove (game, move_two);

        // Check these two moves
        past_move = pastMove (game, (turns - 1), 0);
        assert (past_move.action == move_one.action);
        assert (past_move.card == move_one.card);

        past_move = pastMove (game, (turns - 1), 1);
        assert (past_move.action == move_two.action);
        assert (past_move.card == move_two.card);

    }

    // We know player 0 will finish on turn 28
    turns -= 1;
    while ((turns += 1) <= 28)
    {

        cards_in_hand = handCardCount (game);

        if (cards_in_hand <= 3)
        {

            if (cards_in_hand == 3)
            {
                move_one.action = SAY_TRIO;
                assert (isValidMove (game, move_one));
            }
            else if (cards_in_hand == 2)
            {
                move_one.action = SAY_DUO;
                assert (isValidMove (game, move_one));
            }
            else if (cards_in_hand == 1)
            {
                move_one.action = SAY_UNO;
                assert (isValidMove (game, move_one));
            }
        }

        if (0 < cards_in_hand)
        {

            move_one.card = handCard (game, 0);
            move_one.action = PLAY_CARD;
            assert (isValidMove (game, move_one) == 1);
            playMove (game, move_one);

            move_two.action = END_TURN;
            playMove (game, move_two);

        }
        else
        {

            move_two.action = END_TURN;
            playMove (game, move_two);

        }

        assert (gameWinner (game) == NOT_FINISHED);

    }

    // One card left for everyone else
    int player = NUM_PLAYERS;
    while (0 < (player -= 1))
    {
        assert (playerCardCount (game, player) == 1);
    }

    assert (playerCardCount (game, 0) == 0);
    assert (gameWinner (game) == 0);

    return 0;

}



static int advance_player
(Game game, int player)
{

    if (playDirection (game) == ANTICLOCKWISE)
    {

        if (player == 0)
        {
            player = NUM_PLAYERS;
        }

        player -= 1;

    }
    else
    {

        player += 1;

        if (player == NUM_PLAYERS)
        {
            player = 0;
        }

    }

    return player;

}



int main_numerical_only
(void)
{

    color color[deck_len] = { RED };
    suit suit[deck_len] = { HEARTS };
    value value[deck_len] = { ONE };

    // Switch it up a bit!
    // The first card of each player are ...
    // Player 0   (BLUE DIAMONDS EIGHT)
    // Player 1   (GREEN DIAMONDS ONE)
    // Player 2   (RED HEARTS EIGHT)
    // Player 3   (RED HEARTS ONE)
    // This means player zero and one each miss a turn
    color[0] = BLUE;
    suit[0] = DIAMONDS;
    value[0] = EIGHT;
    color[1] = GREEN;
    suit[1] = DIAMONDS;
    value[2] = EIGHT;

    Game game =
    newGame (deck_len, value, color, suit);

    // Initial deck check
    assert (numCards (game) == deck_len);
    assert (numOfColor (game, RED) == (deck_len - 2));
    assert (numOfSuit (game, HEARTS) == (deck_len - 2));
    assert (numOfValue (game, ONE) == (deck_len - 2));

    assert (numOfColor (game, GREEN) == 1);
    assert (numOfSuit (game, DIAMONDS) == 2);
    assert (numOfValue (game, DRAW_TWO) == 0);


    // Check the first three cards of each players' hands
    int turns = -1;
    int player = 0;
    int cards_in_hand;
    int choose_card;
    playerMove past_move;
    playerMove move_one;
    playerMove move_two;
    Card discard_top;
    while ((turns += 1) < 12)
    {

        assert (playDirection (game) == CLOCKWISE);
        assert (currentTurn (game) == turns);
        assert (currentPlayer (game) == player);

        // Number of cards is how many they get subtract the number of
        // turns they've had.
        assert
        (handCardCount (game) ==
        cards_hand_initially - (turns / NUM_PLAYERS));

        move_one.card = handCard (game, 0);
        move_one.action = PLAY_CARD;
        // For players zero and one, their first card is invalid, so
        // they need to pick another one
        if (player <= 1)
        {
            assert (isValidMove (game, move_one) == 0);
            move_one.card = handCard (game, 1);
            move_one.action = PLAY_CARD;

            assert
            ((* (move_one.card)).color ==
            color[NUM_PLAYERS + turns]);
            assert
            ((* (move_one.card)).suit ==
            suit[NUM_PLAYERS + turns]);
            assert
            ((* (move_one.card)).value ==
            value[NUM_PLAYERS + turns]);
        }
        else
        {
            assert (isValidMove (game, move_one) == 1);
            assert
            ((* (move_one.card)).color ==
            color[turns]);
            assert
            ((* (move_one.card)).suit ==
            suit[turns]);
            assert
            ((* (move_one.card)).value ==
            value[turns]);
        }

        playMove (game, move_one);

        discard_top = topDiscard (game);
        assert
        ((*discard_top).color ==
        (* (move_one.card)).color);
        assert
        ((*discard_top).suit ==
        (* (move_one.card)).suit);
        assert
        ((*discard_top).value ==
        (* (move_one.card)).value);

        move_two.action = END_TURN;
        playMove (game, move_two);

        assert (turnMoves (game, turns) == 2);

        // Check these two moves
        past_move = pastMove (game, (turns - 1), 0);
        assert (past_move.action == move_one.action);
        assert (past_move.card == move_one.card);

        past_move = pastMove (game, (turns - 1), 1);
        assert (past_move.action == move_two.action);
        assert (past_move.card == move_two.card);

        player = advance_player (game, player);

    }

    // We know player 2 will finish on turn 30
    turns -= 1;
    while ((turns += 1) <= 30)
    {

        cards_in_hand = handCardCount (game);

        if (cards_in_hand <= 3)
        {

            if (cards_in_hand == 3)
            {
                move_one.action = SAY_TRIO;
                assert (isValidMove (game, move_one));
            }
            else if (cards_in_hand == 2)
            {
                move_one.action = SAY_DUO;
                assert (isValidMove (game, move_one));
            }
            else if (cards_in_hand == 1)
            {
                move_one.action = SAY_UNO;
                assert (isValidMove (game, move_one));
            }
        }

        if (0 < cards_in_hand)
        {

            // If the first card is invalid, move onto the next and so on
            choose_card = 0;
            move_one.card = handCard (game, choose_card);
            move_one.action = PLAY_CARD;
            while
            ((choose_card < cards_in_hand)
            && (isValidMove (game, move_one) == 0))
            {
                choose_card += 1;
                move_one.card = handCard (game, choose_card);
            }

            // Only play the card when that card is actually in the hand.
            // Otherwise, the hand has invalid cards
            if (choose_card < cards_in_hand)
            {
                assert (isValidMove (game, move_one) == 1);
                playMove (game, move_one);
            }

            move_two.action = END_TURN;
            playMove (game, move_two);

        }
        else
        {

            move_two.action = END_TURN;
            playMove (game, move_two);

        }

        assert (gameWinner (game) == NOT_FINISHED);
        player = advance_player (game, player);

    }

    // One card left for everyone else
    player = NUM_PLAYERS;
    while (0 <= (player -= 1))
    {
        if (player == 2)
        {
            assert (playerCardCount (game, player) == 0);
        }
        else
        {
            assert (playerCardCount (game, player) == 1);
        }
    }

    assert (playerCardCount (game, 2) == 0);
    assert (gameWinner (game) == 2);

    return 0;

}



int main
(int argc, char *argv[])
{

    main_basic ();
    main_numerical_only ();

    return 0;

}


