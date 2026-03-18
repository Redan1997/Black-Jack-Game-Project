#ifndef BLACK_JACK_GAME_H
#define BLACK_JACK_GAME_H
/*written by: Redan */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

typedef struct Card{
    uint8_t data;
    struct Card *next;
} Card;

typedef struct {
    Card *head;
    size_t size;
} CardList;

typedef struct {
    CardList deck;
    CardList dealer_hand;
    CardList player_hand;
    int      cash;
    int      pot;
} GameState;

typedef enum {COMMA=0, NEWLINE=1, SPACE=2,NONE=3} PrintFormat;

/*Initializes cardlist to NULL and zero (assumes it's doesn't contain any card nodes)*/
void init_cardList(CardList *cardlist);

/*assumes cardlist is an empty list, initiazlizes it by allocating all 52 playing cards and adding them to the list.*/
void fill_deck(CardList *cardlist);

/*Frees all memory allocated for the cards in the list and resets it to an empty list*/
void clear_cardList(CardList *cardlist);
int get_suit(const Card *card);                               
int get_rank(const Card *card);                               
Card *card_new(int rank, int suit);                     
Card *card_remove_at(CardList *cardlist, size_t index); 
Card *card_draw(CardList *deck, CardList *cardlist);    
void card_push(CardList *cardlist, Card *card);

/*function phase*/
int betting_handler(GameState *game);//phase 1
void initial_Deal_Handler(GameState *game);//phase 2
int Black_Jack_Check(GameState *game);//phase 3
int Hit_or_Stand(GameState *game);//4
int Reset_Cards(GameState *game);
void dealer_draw_phase(GameState *game);


/*input handlers*/
int readPotHandler(int p_cash,int pot);
void print_card(const Card *card);
int hand_value(const CardList * hand);
int want_to_play(void);
void return_hand_to_deck(CardList *deck,CardList*handToReturn);
int Hit_or_Stand_Handler(void);
void print_hand(const CardList *hand,const char* user,const int newLine);
int have_ace(const CardList *hand);
#endif /* BLACK_JACK_GAME_H */