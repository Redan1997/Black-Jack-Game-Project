/*written by: Redan */
#include "Black-Jack Game.h"
int main(void) {
    puts("\033[2J\033[H");//clean screen for better user experience
    GameState game;
    int continueFlag=1;
    srand((unsigned int)time(NULL));

    init_cardList(&game.deck);
    fill_deck(&game.deck);
    init_cardList(&game.dealer_hand);
    init_cardList(&game.player_hand);
    game.cash=1000;
    game.pot=0;

    while (continueFlag){
        if(!betting_handler(&game))
            break;
        initial_Deal_Handler(&game);
        if(!Black_Jack_Check(&game))
            if(Hit_or_Stand(&game))
                dealer_draw_phase(&game);
        continueFlag=Reset_Cards(&game);
    }

    clear_cardList(&game.dealer_hand);
    clear_cardList(&game.player_hand);
    clear_cardList(&game.deck);
    printf("Table closed thank you for playing!\n");
    return 0;
}