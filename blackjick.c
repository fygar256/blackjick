#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>

typedef struct {
    int mark; // 0:no card 1:heart 2:dia 3:club 4:spade 5:joker
    int number; // 1 ~ 13 0:joker
} card;

#define TRUMP (53)
#define DRAWMAX (12)    		// maximum number of cards per player

card cards[TRUMP];
int cardidx=0;
card comp_cards[DRAWMAX];
card player_cards[DRAWMAX];

card id2card(int c)
{
    card cd;
    cd.mark=(c==0)?5:(c-1)/13+1;
    cd.number=(c==0)?0:(c-1)%13+1;
    return cd;
}

void shuffle()
{
    int r[TRUMP]= {0},ss[TRUMP],c;
    srand((unsigned)time(NULL));
    for(int k=0; k<TRUMP; k++) {
        while(r[c=rand()%TRUMP]);
        r[ss[k]=c]=1;
    }
    for(int i=0; i<TRUMP; i++)
        cards[i]=id2card(ss[i]);
}

void draw_card(card cd[])
{
    int i=0;
    while(cd[i].mark) i++;
    if (cardidx<TRUMP && i<DRAWMAX)
        cd[i]=cards[cardidx++];
}

void init_players()
{
    for(int i=0; i<DRAWMAX; i++)
        comp_cards[i].mark=player_cards[i].mark=0;
    draw_card(comp_cards);
    draw_card(comp_cards);
    draw_card(player_cards);
    draw_card(player_cards);
}

int number_of_cards(card cd[])
{
    int idx=0;
    while(cd[idx].mark) idx++;
    return idx;
}

int isjoker(card cd[])
{
    int idx=0;
    while(cd[idx].mark)
        if (cd[idx++].mark==5)
            return 1;
    return 0;
}

void printcard(card cd)
{
    static char *marks[]= {"","♥","♦","♣","♠","Joker"};
    static char *numbers[]= {"","A","2","3","4","5","6","7","8","9","10",
                             "J","Q","K" };
    printf("[%s %s] ",marks[cd.mark],numbers[cd.number]);
}

int count_cards(card cd[])
{
    int n,i=0,v=0;
    while(cd[i].mark) {
        n=cd[i++].number;
        v+=n<=10?n:10;
    }
    return v;
}

int eval_cards(card cd[])
{
    int v=count_cards(cd),jv=isjoker(cd)?(v<=21?(21-v>=11?10:21-v):0):0;
    return v+jv;
}

void print_players_line(card cd[])
{
    int t,i=0;
    while(cd[i].mark)
        printcard(cd[i++]);
    printf(" total=%3d ",t=eval_cards(cd));
    if (t==21)
        printf("*BlackJick*");
    printf("\n");
}

void print_comp()
{
    int idx=0;
    while(comp_cards[idx++].mark)
        printf("[###] ");
    printf("%d cards.\n",idx-1);
}

int game()
{
    // game
    int status,cv=eval_cards(comp_cards),pv=eval_cards(player_cards);
    if (cv==21 && pv==21|| cv==pv)
        if (isjoker(comp_cards))
            status=2;
        else if (isjoker(player_cards))
            status=1;
        else
            status=0;
    else if (cv>=22 && pv>=22)
        status=0;
    else if (cv>=22 && pv<=21)
        status=1;
    else if (cv<=21 && pv>=22)
        status=2;
    else if (cv<pv)
        status=1;
    else
        status=2;
    return status;
}

double expected_value_of_next_card(card self[])
{
    int noc=TRUMP-number_of_cards(self),n=count_cards(self);
    int j=isjoker(self)?0:(n>=21?0:(n<11?10:21-n));
    return ((double)340+j-n)/noc;
}

int todrawp(card self[])
{
    return eval_cards(self)>=21?0:(count_cards(self)+expected_value_of_next_card(self))<=21.00?1:0;
}

int computer_turn()
{
    if (todrawp(comp_cards)) {
        printf("Computer drew a card.\n");
        draw_card(comp_cards);
        return 1;
    }
    return 0;
}

void play()
{
    int d=0;
    do {
        double e;
        printf("---------------------------------------------------\n");
        printf("Pile : %d cards\n",TRUMP-cardidx);
        printf("Computer's cards : ");
        print_comp();
        printf("Player's cards : ");
        print_players_line(player_cards);
        e=expected_value_of_next_card(player_cards);
        printf("Expected value of next 1 card is %1.8lf.\n",e);
        printf("Have to draw ? %s.\n",todrawp(player_cards)?"Yes":"No");
        printf("1. Draw a card.\n2. Game.\n: ");
        scanf("%d",&d);
        if (d==1)
            draw_card(player_cards);
        computer_turn();
    } while(d!=2);
    while(computer_turn());
}

void result(int g) {
    printf("====================Result====================\n");
    printf("Computer : ");
    print_players_line(comp_cards);
    printf("Player : ");
    print_players_line(player_cards);
    printf("%s.\n",(g==0)?"Draw game":(g==1)?"Player win":"Computer win");
}

int main()
{
    printf("**********BlackJick ver 1.0**********\n");
    shuffle();
    init_players();
    play();
    result(game());
}

