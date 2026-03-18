#ifndef BLACK_JACK_GAME_H
#define BLACK_JACK_GAME_H
/*project: Black Jack Game
    standard:      
        C99
    warning flags:
        "-Wall",
        "-Wextra",
        "-Wconversion",
        "-Wshadow",*/
/*written by: Redan */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <ctype.h>

typedef struct Card{    //card represnt a songle playing card with rank and suit
    uint8_t data;       //first 4 bits for rank and last 4 bits for suit
    struct Card *next;  //pointer to next card in list (for deck and hand)
} Card;

typedef struct {
    Card *head;         //represent the top of the deck or the first card in hand
    size_t size;        //number of cards in the deck or hand
} CardList;

typedef struct {        //represent the state of the single game session
    CardList deck;      //52 cards shared between player and dealer
    CardList dealer_hand; //represent the dealer hand
    CardList player_hand; //represent the player hand
    int      cash;      //represent the player cash
    int      pot;       //represent the current bet amount for the round
} GameState;

//typedef enum {COMMA=0, NEWLINE=1, SPACE=2,NONE=3} PrintFormat; //unused cooma and newline
typedef enum {NONE=0, SPACE=1} PrintFormat; //for print hand to be more readable

/*Initializes cardlist to NULL and zero (assumes it's doesn't contain any card nodes)*/
void init_cardList(CardList *cardlist);

/*Frees all memory allocated for the cards in the list and resets it to an empty list*/
Card *card_new(const int rank, const int suit);               

/*draw a card from the deck and add it to the cardlist*/      
Card *card_draw(CardList *deck, CardList *cardlist);    

/*remove and return a card at a specific index from the list*/
Card *card_remove_at(CardList *cardlist, size_t index); 

/*push a card to the front of the list and update the size*/
void card_push(CardList *cardlist, Card *card);

/*fill the deck with all 52 cards*/
void fill_deck(CardList *cardlist);

/*move all cards from a hand back to the deck(reset the hand)*/
void return_hand_to_deck(CardList *deck,CardList*handToReturn);

/*free all dynamically allocated memory for the cards in the list and reset it to an empty list*/
void clear_cardList(CardList *cardlist);


/*function phase*/
//phase 2:handle pot and player cash for current round
int betting_handler(GameState *game);

//phase 3:take card from deck and add to player and dealer hand and hide one of dealer cards
void initial_Deal_Handler(GameState *game);

//phase 4:check if player have black jack
int Black_Jack_Check(GameState *game);

//phase 5:handle player hit or stand and check if player busted or have 21 after hit
int Hit_or_Stand(GameState *game);

//phase 6:handle dealer draw and check if dealer busted or win or tie with player
void dealer_draw_phase(GameState *game);

//phase 7:return all cards to deck(reset hands)
int Reset_Cards(GameState *game);


/*input handlers*/
int readPotHandler(const int p_cash,const int pot);
int want_to_play(void);
int Hit_or_Stand_Handler(void);

/*getter functions*/
int get_suit(const Card *card);             //return the suit of the card (1(hearts),2(diamonds),4(clubs),8(spades))                               
int get_rank(const Card *card);             //return the rank of the card (1-13) from ace to king
int get_card_value(const Card *card);       //return the value of the card (1-10) with ace as 1 and picture cards as 10
int get_hand_value(const CardList * hand);  //return the total value of the hand (with ace handling)                            

/*display function*/
void print_card(const Card *card);
void print_hand(const CardList *hand,const char* user,const int space);

#endif /* BLACK_JACK_GAME_H */