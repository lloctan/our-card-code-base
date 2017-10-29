//player.c
//Alexander Crouch z5161507 and Elison Zon z3463249
//29.10.2017

//    DRAW_CARD,
    // Play a single card onto the discard pile.
//    PLAY_CARD,
    // Say the word "UNO".
//    SAY_UNO,
    // Say the word "DUO".
 //   SAY_DUO,
 //        Say the word "TRIO".
 //   SAY_TRIO,
    // End the player's turn.
//    END_TURN

// Prototype
#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "player.h"



// Miscellaneous
// This should only run once. Hence, global
int random_seed = 80;
srandom (random_seed);



#define NOT_FOUND -1



// Returns a pointer to a vector which references cards in the hand the
// AI can choose from to play
static int *valid_cards_to_play
(Game game)
{

    // Number of cards in the hand
    int in_hand = handCardCount (game);

    // The first element contains the number of playable cards.
    // If 2, then the two integers following it contain indices
    // to the player's hand which are playable
    int playable_cards =
    malloc ((sizeof (int)) * (in_hand + 1));

    // To check whether or not the card is playable, we try it
    // with "isValidMove"
    int found = 0;
    playerMove move;
    move.action = PLAY_CARD;

    while (0 <= (in_hand -= 1))
    {

        move.card =
        handCard (game, in_hand);

        if (isValidMove (game, move))
        {

            found += 1;
            // Put the index into the array
            playable_cards[found] = in_hand;

        }
    }

    // If we find none, return NULL pointer
    if (found == 0)
    {
        free (playable_cards);
        playable_cards = NULL;
    }

    // If we find some, put the number of possible cards to play
    // in the first element
    else
    {
        playable_cards[0] = found;
        // THe remaining elements are indices to the cards in the hand.
        // So, we can get the cards with
        // "int *valid_cards = valid_cards_to_play (game);"
        // "handCard (game, valid_cards[1]);"
        // "int *free (valid_cards);"
    }

    return found;

}



// Find a card in the player's hand that matches the specified color,
// if such a card exists.
// Returns the card index, or NOT_FOUND if no matching card was found.
static int findMatchingCardColor
(Game game, color color)
{

    int found = 0;
    Card card;

    const int in_hand = handCardCount (game);
    int choose[in_hand] = { 0 };

    while (0 <= (in_hand -= 1))
    {

        card = handCard (game, in_hand);

        if ((*card).color == color)
        {

            choose[found] = in_hand;
            found += 1;

        }
    }

    // If we find none, return none. If we find one, return it.
    // If we find more than one, return one at random
    if (found == 0)
    {
        found = NOT_FOUND;
    }
    else if (found == 1)
    {
        found = choose[0];
    }
    else
    {
        found = choose[random () % (found - 1)];
    }

    return found;

}



// Do two cards match on either value, color, or suit?
// Returns TRUE if they match any of the above features, and
// FALSE if they don't match on any of the above features.
static int doCardsMatch
(Card first, Card second)
{

    return
    // Match on colour
    (((*first).color == (*second).color)
    // Match on suit
    || ((*first).suit == (*second).suit)
    // Match on value
    || ((*first).value == (*second).value));

}

//determine which cards can be played
static int whatCard (Game game, int *found) {
    int cardIndex = -1;
    Card check;
    Card check2;
    Card topDiscard = topDiscard(game);
    int gotcha = -1;
    int a = 1;
    //prioritises move that it wants to make
    //checks if the discard is a draw 2, if so looks for a draw two in hand
    //if it's not a draw 2 just play the highest value card, 101: strategy FTW
    if (topDiscard->value == DRAW_TWO) {
        while (a <= found[0] && gotcha == -1) {
            check = handCard(game, found[a]);
                if (check->value == DRAW_TWO) {
                    gotcha = 0;
                    cardIndex = a;
                }
            a++;
        }
    } else {
        check = handCard(game, found[1]);
        int max = check->value; 
        while (a < found[0]) {
            check = handCard(game, found[a]);
            if (check->value > max) {
                max = check->value;
                cardIndex = a;
            }
        a++;
        }   
    }
    return cardIndex;
}

// Determine whether the player can currently draw a card.
// If they can't draw a card, they should probably end their turn.
static int canDrawCard
(Game game)
{

    Card in_discard = topDiscard (game);

    int found =
    findMatchingCardColor
    (game, (*in_discard).color);

    // If unable to play a card, draw a card
    if (found == NOT_FOUND)
    {
        found = FALSE;
    }

    // If able to play a card, no need to draw one
    else
    {
        found = TRUE;
    }

    return found;

}



// Determine whether the current player should SAY_UNO.
// There are two different situations where it could be a
// valid move to SAY_UNO.
// For now, just deal with the simple situation: "claim card".
// Note: there are several possible ways to determine this.
static int shouldSayUNO
(Game game)
{

    printf("%s\n","UNO");
    return (handCardCount (game) == 1);

}

// Determine whether the current player should SAY_UNO.
// There are two different situations where it could be a
// valid move to SAY_DUO.
// For now, just deal with the simple situation: "claim card".
// Note: there are several possible ways to determine this.
static int shouldSayDUO
(Game game)
{
    printf("%s\n","DUO");
    return (handCardCount (game) == 2);

}

// Determine whether the current player should SAY_UNO.
// There are two different situations where it could be a
// valid move to SAY_TRIO.
// For now, just deal with the simple situation: "claim card".
// Note: there are several possible ways to determine this.
static int shouldSayTRIO
(Game game)
{
    printf("%s\n","TRIO");
    return (handCardCount (game) == 3);

}



// This function is to be implemented by the A.I.
// It will be called when the player can make an action on their turn.
//
// !!!!! The function is called repeatedly, until !!!!!
// !!!!! they make the action `END_TURN`. !!!!!
//
// If the player's turn is skipped, this funciton
// is *not*called for that player.
//
// Don't forget about the `isValidMove` function -- it's a handy way
// to work out before you play a move whether or not it will be valid
// (and you should only ever be making valid moves).
//
// You can also use it to help you work out where you are at in the
// game, without needing to look through all of the previous state
// yourself --
//
// Looking at the diagram of valid moves at any given point in the game,
// we can see that at the start of the game, it's valid to:
// - call somebody out for forgetting to SAY_UNO / SAY_DUO / SAY_TRIO,
// - draw a card,
// - play a card, *if*you have a valid card that you can play.
//
// It's not valid to end your turn unless you've done some other
// action/s (again, see the diagram).
//
// We can take advantage of that for our very simple A.I. to determine
// where we are at in our turn, and thus what move we should make.

playerMove decideMove 
( Game game ) 
{ 
  // Start out by making a move struct, to say what our move is. 
  playerMove move ; 

  // Set our move to be END_TURN, and check whether that's 
  // a valid move -- if it is, then just end our turn ( for now ). 
  move.action = END_TURN ; 

  // If it's not valid to end turn, we must need to make 
  // some other action... 
  // 
  // What actions are valid at this point? 
  if ( isValidMove ( game , move ) == 0 )
    { 

      // Valid actions include: 
      // - play a card, 
      // - draw a card, 
      // - call the previous play out for forgetting to SAY_UNO etc. 
      int * valid_cards = valid_cards_to_play ( game ) ;
      if ( valid_cards )
    {
      // Determine which is the best move to play 
      move.card =
        handCard ( game , whatCard ( game , valid_cards )) ;
      free ( valid_cards ) ;
    }
      else
    {
      // The easiest thing here is to draw a card, since we don't 
      // need to try to work out if we have a valid card, etc. 
      move.action = DRAW_CARD ; 
    }

      // Now submit move 
      playMove ( game , move ) ;

    } 

  // Now we end our turn 
  else
    {

      // Check necessity to call "TRIO", "DUO" or "UNO" 
      int num_cards_in_hand = handCardCount ( game ) ;
      if ( num_cards_in_hand <= 3 )
    {

      if ( num_cards_in_hand == 3 )
        {
          move.action = SAY_TRIO ;
        }
      else if ( num_cards_in_hand == 2 )
        {
          move.action = SAY_DUO ;
        }
      else if ( num_cards_in_hand == 1 )
        {
          move.action = SAY_UNO ;
        }

      playMove ( game , move ) ;

    }

      // End turn now 
      move.action = END_TURN ;
      playMove ( game , move ) ;

    }

  return move ; 
} 




