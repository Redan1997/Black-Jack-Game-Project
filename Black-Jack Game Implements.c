#include "Black-Jack Game.h"

const char *RANK_NAMES[] = {//size of 14 to use rank value as index
    "", "Ace", "2", "3", "4", "5", "6", "7",//add "" for index 0
    "8", "9", "10", "Jack", "Queen", "King"
};
const char *SUIT_NAMES[] = {//size of 8 to use suit value as index
    "", "Hearts", "Clubs", "", "Diamond", "", "", "", "Spades"
};

int get_suit(const Card *card){
    switch (card->data & 0xF) { // Masking to get the card value (1-15)
        case 1: return 1;
        case 2: return 2;
        case 4: return 4;
        case 8: return 8;
        default: {
            printf("Invalid card type: %d\n", card->data & 0xf);
            exit(EXIT_FAILURE);
        }
    }
}

int get_rank(const Card *card){
    switch (card->data >> 4) { // Shifting to get the card rank (1-13)
        case 1: return 1;
        case 2: return 2;
        case 3: return 3;
        case 4: return 4;
        case 5: return 5;
        case 6: return 6;
        case 7: return 7;
        case 8: return 8;
        case 9: return 9;
        case 10: return 10;
        case 11: return 11;
        case 12: return 12;
        case 13: return 13;
        default: {
            printf("Invalid card rank: %d\n", card->data >> 4);
            exit(EXIT_FAILURE);
        }
    }
}
int get_card_value(const Card *card){
    int value=get_rank(card);
    if(value>10)//if picture
        return 10;
    return value;
}

void init_cardList(CardList *cardlist){
    cardlist->head = NULL;
    cardlist->size = 0;
}


Card *card_new(int rank, int suit){
    Card *newCard;
    if(!(newCard=malloc(sizeof(Card)))){
        printf("Memory allocation failed for new card\n");
        exit(EXIT_FAILURE);
    }
    newCard->data = (rank << 4) | suit;
    newCard->next = NULL;
    return newCard;
}

void card_push(CardList *cardlist, Card *card){
    card->next=cardlist->head;                              //push to head
    cardlist->head=card;
    cardlist->size++;
}

Card *card_remove_at(CardList *cardlist, const size_t index){
    if(!cardlist->head)                                     //check if list empty 
        return NULL;
    Card *removed=cardlist->head;
    if(!index){                                             //remove head
        cardlist->head=removed->next;
        removed->next=NULL;
        cardlist->size--;
        return removed;
    }
    Card *prev=removed;                                     //save prev of removed to connect to next of removed
    for (size_t i = 0; i < index-1; i++)
    {
        if(!prev->next)                                     //check to prevent dereferencing **********************************
            return NULL;
        prev=prev->next;
    }
    removed=prev->next;
    if(!removed)
        return NULL;
    prev->next=removed->next;
    removed->next=NULL;
    cardlist->size--;
    return removed;
}
Card *card_draw(CardList *deck, CardList *cardlist){
    int random_index = (rand() % (deck->size));             //get random from 0 - sizeof deck
    Card * cardFromDeck=card_remove_at(deck,random_index);  //remove card from deck
    //print_card(cardFromDeck);//for debug
    card_push(cardlist,cardFromDeck);                       //add to hand
    //print_card(cardlist->head);//for debug
    return cardFromDeck;
}

void fill_deck(CardList *deck){
    int suits[]={0x1,0x2,0x4,0x8};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j < 14; j++)//13 card from ace to king
        {
            card_push(deck,card_new(j,suits[i]));           //allocate card and add to list of deck
        }
    }
}

void clear_cardList(CardList *cardlist){
    Card* removed=cardlist->head,*nxt;
    while(removed){
        nxt=removed->next;
        free(removed);
        removed=nxt;
    }
    cardlist->head=NULL;
    cardlist->size=0;
}


/*              handlers            */
int betting_handler(GameState *game){
    printf("--------------------Betting--------------------\n");//----------
    printf("player have %d$ | currenly pot is %d$\n",game->cash,game->pot);
    if(game->cash<10 && !(game->pot))//no need for () ,pointer is priority 0
        return 0;
    int pot=readPotHandler(game->cash,game->pot);
    game->pot+=pot;
    game->cash-=pot;
    printf("Player bet %d$\n",pot);
    return pot;
}
void initial_Deal_Handler(GameState *game){
    printf("--------------------Dealing--------------------\n");
    card_draw(&game->deck,&game->player_hand);
    card_draw(&game->deck,&game->dealer_hand);
    card_draw(&game->deck,&game->player_hand);
    card_draw(&game->deck,&game->dealer_hand);
    print_hand(&game->dealer_hand,"dealer: ",1);//for debug
    // printf("Dealer: ");
    // print_card(game->dealer_hand.head);
    // printf(", ????????\n\n");
    print_hand(&game->player_hand,"Player: ",1);
}
int Black_Jack_Check(GameState *game){
    int player_hand_value=hand_value(&(game->player_hand));
    if(player_hand_value==21){
        int earning = game->pot *1.5;
        game->cash+=earning;
        game->pot=0;
        printf("Player won %d$ and now have %d$\n",earning,game->cash);
        return 1;
    }
    return 0;
}
int Reset_Cards(GameState *game){
    return_hand_to_deck(&game->deck,&game->player_hand);
    return_hand_to_deck(&game->deck,&game->dealer_hand);
    if(game->cash<10&&game->pot==0){
        printf("Player became broke, thanks for make us richer\n");
        return 0;
    }
    return want_to_play();
}
int Hit_or_Stand(GameState *game){
    //printf("--------------------Hit or Stand?--------------------\n");
    while(1){
        int input=Hit_or_Stand_Handler();
        if(input){
            card_draw(&game->deck,&game->player_hand);
            print_hand(&game->player_hand,"Player: ",2);
            int handValue=hand_value(&game->player_hand);
            printf("| value:%d\n",handValue);
            if(handValue>21){
                printf("Busted player lost %d$\n",game->pot);
                game->pot=0;
                return 0;
            }
            else if(handValue==21)
                return 1;
        }
        else return 1;//continue to dealer draw phase
    }
}
void dealer_draw_phase(GameState *game){
    int playerHandValue=hand_value(&game->player_hand);
    int dealerHandValue=hand_value(&game->dealer_hand);
    printf("Dealer value: %d | Player value: %d\n", dealerHandValue, playerHandValue);
    print_hand(&game->player_hand,"Player: ",1);
    print_hand(&game->dealer_hand,"Dealer: ",0);
    if(dealerHandValue>playerHandValue){
        printf("Dealer wins!!\n");
        game->pot=0;
        return;
    }

    while((dealerHandValue<playerHandValue)&&(dealerHandValue<17)){
        Card* newCard=card_draw(&game->deck,&game->dealer_hand);
        print_card(newCard);
        dealerHandValue=hand_value(&game->dealer_hand);
        if((dealerHandValue<playerHandValue)&&(dealerHandValue<17))
            printf(", ");
        else 
            puts("");
        }
    if((dealerHandValue>21)){
        printf("Dealer Busted!!\nPlayer won %d$ and now have %d$",game->pot,game->cash);
        game->cash+=game->pot*2;
        game->pot=0;
    }
    else if(dealerHandValue==playerHandValue)
        {
            printf("Tie\n");
        }
    else if(dealerHandValue>playerHandValue){
        printf("Dealer wins!!\n");
        game->pot=0;
    }
    else{
        printf("Player won %d$ and now have %d$",game->pot,game->cash);
        game->cash+=game->pot*2;
        game->pot=0;
    }

}

/*          input/displayer handlers          */
int readPotHandler(int p_cash,int pot){
    while (1)
    {
        if(!pot && p_cash<10)
            return 0;
        int bet;
        printf("how much you willing to add to the pot?: ");
        int status = scanf(" %d", &bet);
        
        // 1. Check for EOF
        if (status == EOF)
        {
            printf("\nInput EOF line 152 ended. Exiting.\n");//check line at end
            exit(EXIT_FAILURE);
        }
        
        // 2. Check for invalid characters
        if (status < 1) 
        {
            printf("Error: Please enter valid number above 10.\n");
            while(getchar() != '\n'); // Clear the input buffer
            continue;
        }
        
        // 3. Check for decimals/junk after the second number
        char next = getchar();
        if (next != '\n' && next != EOF)
        {
            printf("Error: Decimals or extra characters detected.\n");
            while (getchar() != '\n');
            continue;
        }

        if(bet<10 && !pot)//pot is 0
            printf("You must put at least 10$ to start\n");
        else if(bet > p_cash)
            printf("You dont have that money\n");
        else if(bet % 10 !=0){
            printf("Bets must be made in multiples of 10");
        }
        else return bet;
    }
}
void print_card(const Card *card){
    printf("%s of %s", RANK_NAMES[get_rank(card)], SUIT_NAMES[get_suit(card)]);
}
int hand_value(const CardList * hand){
    int ace_flag=0;
    Card *currCard=hand->head;
    int value=0;
    while(currCard){
        value+=get_card_value(currCard);
        if(get_rank(currCard)==1){
            ace_flag=1;
        }
        currCard=currCard->next;
    }
    if(ace_flag&&value+10<=21)
        value+=10;
    return value;
}
void return_hand_to_deck(CardList *deck,CardList*handToReturn){
    Card *card=handToReturn->head,*nxt;
        while (card)
        {
            nxt=card->next;
            card_push(deck,card);
            card=nxt;
        }
        handToReturn->head=NULL;
        handToReturn->size=0;
}
int want_to_playold(void){
    puts("Do you want to continue gambling write 1 for continue 0 for no?: ");
    int a;
    while (1)
    {
        int status = scanf(" %d", &a);
        
        // 1. Check for EOF
        if (status == EOF)
        {
            printf("\nInput stream ended. Exiting.\n");
            exit(EXIT_SUCCESS);
        }
        
        // 2. Check for invalid characters
        if (status < 1) 
        {
            printf("Error: Please enter a valid integer (0 or 1).\n");
            while (getchar() != '\n'); 
            continue; 
        }
        
        // 3. Check for decimals/junk after the second number
        char extra = getchar();
        if (extra != '\n' && extra != EOF)
        {
            printf("Error: extra characters detected.\n");
            while (getchar() != '\n');
            continue;
        }
        if(a!=1&&a!=0)
            printf("please enter 1 for yes and 0 for no: ");
        else
            break;
    }
    return a;
}
int Hit_or_Stand_Handler(void){
    char input[32];
    printf("Do you want to hit or stand: ");
    while (1)
    {
        int status = scanf(" %31s", input);

        // 1. Check for EOF
        if(status == EOF)
        {
            printf("\nInput stream ended. Exiting.\n");
            exit(EXIT_SUCCESS);
        }

        // 2. Check for invalid characters
        if(status < 1)
        {
            printf("Error: Could not read input. Enter hit or stand ");
            while (getchar() != '\n' && !feof(stdin));
            continue;
        }

        // 3. Convert to lowercase
        for(int i = 0; input[i]; i++)
            input[i] = tolower((unsigned char)input[i]);

        // 4. Validate with strcmp
        if(strcmp(input, "hit") == 0 
            || strcmp(input, "h") == 0
            || strcmp(input, "1") == 0)
            return 1;
        else if(strcmp(input, "stand") == 0 
            || strcmp(input, "s") == 0
            || strcmp(input, "0") == 0)
            return 0;
        else
            printf("Error: Invalid input '%s'. Enter hit or stand: ", input);
    }
}
int want_to_play(void){
    char input[32];
    printf("Do you want to continue gambling? Enter yes or no: ");
    while (1)
    {
        int status = scanf(" %31s", input);

        // 1. Check for EOF
        if(status == EOF)
        {
            printf("\nInput stream ended. Exiting.\n");
            exit(EXIT_SUCCESS);
        }

        // 2. Check for invalid characters
        if(status < 1)
        {
            printf("Error: Could not read input. Enter yes/no, y/n, or 1/0: ");
            while (getchar() != '\n' && !feof(stdin));
            continue;
        }

        // 3. Convert to lowercase
        for(int i = 0; input[i]; i++)
            input[i] = tolower((unsigned char)input[i]);

        // 4. Validate with strcmp
        if(strcmp(input, "yes") == 0 || strcmp(input, "y") == 0 || strcmp(input, "1") == 0)
            return 1;
        else if(strcmp(input, "no") == 0 || strcmp(input, "n") == 0 || strcmp(input, "0") == 0)
            return 0;
        else
            printf("Error: Invalid input '%s'. Enter yes/no, y/n, or 1/0: ", input);
    }
}
void print_hand(const CardList *hand,const char* user,const int newLine){
    Card * card=hand->head;
    if (card == NULL) {
        return;
    }
    printf("%s: ",user);
    while(card->next){
        print_card(card);
        printf(", ");
        card=card->next;
    }
    print_card(card);
    if(newLine==1)
        printf("\n");
    else if(newLine==2)
        printf(" ");
    else printf(", ");
}
//end of file
