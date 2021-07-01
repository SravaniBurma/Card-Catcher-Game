/************************************
* Description : Card Matching Game
************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_CARD 52
#define MAX_TYPE 13

/* node definition for card data type */
typedef struct card {
    int cardNo, cardType;
    struct card *next;
}node;

/* create a list node */
node *createNode(int n, int t)
{
    node *new;
    if( (new=(node *)malloc(sizeof(node *))) == NULL )
        printf("Malloc Error!\n");
    else{
        new->cardNo = n;
        new->cardType = t;
        new->next = NULL;
    }
    return new;
}

/* delete the front node of the list and return that */
node *deleteFront(node **pnode)
{
    node *delNode = *pnode;
    (*pnode) = (*pnode)->next;
    delNode->next = NULL;
    return delNode;
}

/* print the number (2-10 and Jack/J, Queen/Q, King/K, Ace/A) of cards */
void printCardNo(int no)
{
    switch(no){
        /* 2-10 */
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            printf("%d", no+2);
            break;
        case 9: /* jack */
            printf("J");
            break;
        case 10: /* queen */
            printf("Q");
            break;
        case 11: /* king */
            printf("K");
            break;
        case 12: /* ace */
            printf("A");
            break;
        default: /* wrong entry number */
            printf("N-W");
            break;
    }
}

/* print the type (Spade/S, Hearts/H, Diamond/D, Clubs/C) of cards */
void printCardType(int type)
{
    switch(type){
        case 0: /* spade */
            printf("S");
            break;
        case 1: /* hearts */
            printf("H");
            break;
        case 2: /* diamond */
            printf("D");
            break;
        case 3: /* clubs */
            printf("C");
            break;
        default: /* wrong entry type */
            printf("T-W");
            break;
    }
}

/* print the set of card (hand) */
void printHand(node *hand)
{
    node *cur;
    for(cur=hand; cur!=NULL; cur=cur->next){
        printf(" --> [ ");
        printCardType(cur->cardType);
        printf(" ");
        printCardNo(cur->cardNo);
        printf(" ]");
    }
    printf(" --> END\n");
}

/* check whether card <t n> is already served */
int matchCard(node *h1, node *h2, int n, int t)
{
    node *cur;
    for(cur=h1; cur!=NULL; cur=cur->next){
        if( (cur->cardNo==n) && (cur->cardType==t) ){
            break;
        }
    }
    if(cur == NULL){
        for(cur=h2; cur!=NULL; cur=cur->next){
            if( (cur->cardNo==n) && (cur->cardType==t) ){
                break;
            }
        }
    }
    return (cur != NULL);
}

int main()
{
    int i, card, number, type, turn;
    node *hand1=NULL, *hand2=NULL, *endHand1=NULL, *endHand2=NULL, *newNode, *matchNode=NULL, *endMatchNode=NULL;
    
    srand(getpid());
    
    /* random shuffle of cards and one-by-one hand organization */
    for(i=0; i<MAX_CARD; i++){
        do{
            card = rand()%MAX_CARD;
            number = card%MAX_TYPE;
            type = card/MAX_TYPE;
        }while(matchCard(hand1, hand2, number, type));
        
        newNode = createNode(number, type);
        if(i%2){ /* populate hand-2 with random cards */
            if(hand2 == NULL){
                hand2 = newNode;
                endHand2 = hand2;
            }
            else{
                endHand2->next = newNode;
                endHand2 = endHand2->next;
            }
        }
        else{ /* populate hand-1 with random cards */
            if(hand1 == NULL){
                hand1 = newNode;
                endHand1 = hand1;
            }
            else{
                endHand1->next = newNode;
                endHand1 = endHand1->next;
            }
        }
    }
    
    printf("\nPlayer-1 Hand:");
    printHand(hand1);
    printf("\nPlayer-2 Hand:");
    printHand(hand2);

    printf("\n*** Game Starts ***\n");
    turn = 0; i=0;
    while( (hand1!=NULL) && (hand2!=NULL) ){
        /* print contesting cards from both players */
        printf("\nPlay No. %d :: [ ", i+1);
        printCardType(hand1->cardType);
        printf(" ");
        printCardNo(hand1->cardNo);
        printf(" ] vs [ ");
        printCardType(hand2->cardType);
        printf(" ");
        printCardNo(hand2->cardNo);
        printf(" ]\n");
        
        /* matching the cards and appending the played set of cards to the player who takes */
        if(hand1->cardNo > hand2->cardNo){ /* player-1 takes */
            if(matchNode != NULL){ /* append earlier matched set of cards */
                endHand1->next = matchNode;
                endHand1 = endMatchNode;
            }
            if(turn){ /* turn=1 meaning player-2 played first */
                newNode = deleteFront(&hand2);
                newNode->next = deleteFront(&hand1);
            }
            else{ /* turn=0 meaning player-1 played first */
                newNode = deleteFront(&hand1);
                newNode->next = deleteFront(&hand2);
            }
            if(hand1 == NULL){ /* last card played by player-1 */
                hand1 = newNode;
                endHand1 = newNode->next;
            }
            else{
                endHand1->next = newNode;
                endHand1 = (endHand1->next)->next;
            }
            turn = 0; /* player-1 gets to play first in the next turn */
            matchNode = NULL; /* resets matched set of cards */
            
            printf("+++ Player-1 Takes +++\n");
            printf("\nPlayer-1 Hand:");
            printHand(hand1);
            printf("\nPlayer-2 Hand:");
            printHand(hand2);
        }
        else if(hand1->cardNo < hand2->cardNo){ /* player-2 takes */
            if(matchNode != NULL){ /* append earlier matched set of cards */
                endHand2->next = matchNode;
                endHand2 = endMatchNode;
            }
            if(turn){ /* turn=1 meaning player-2 played first */
                newNode = deleteFront(&hand2);
                newNode->next = deleteFront(&hand1);
            }
            else{ /* turn=0 meaning player-1 played first */
                newNode = deleteFront(&hand1);
                newNode->next = deleteFront(&hand2);
            }
            if(hand2 == NULL){ /* last card played by player-2 */
                hand2 = newNode;
                endHand2 = newNode->next;
            }
            else{
                endHand2->next = newNode;
                endHand2 = (endHand2->next)->next;
            }
            turn = 1; /* player-2 gets to play first in the next turn */
            matchNode = NULL; /* resets matched set of cards */
            
            printf("+++ Player-2 Takes +++\n");
            printf("\nPlayer-1 Hand:");
            printHand(hand1);
            printf("\nPlayer-2 Hand:");
            printHand(hand2);
        }
        else{ /* tie between card numbers */
            if(turn){ /* turn=1 meaning player-2 played first */
                newNode = deleteFront(&hand2);
                newNode->next = deleteFront(&hand1);
            }
            else{ /* turn=0 meaning player-1 played first */
                newNode = deleteFront(&hand1);
                newNode->next = deleteFront(&hand2);
            }
            /* building list of matched cards */
            if(matchNode == NULL){
                matchNode = newNode;
                endMatchNode = matchNode->next;
            }
            else{
                endMatchNode->next = newNode;
                endMatchNode = (endMatchNode->next)->next;
            }
            
            printf("+++ Players Equal -- Next Turn +++\n");
        }
        i++; /* maintaining play round (iteration) count */
    }
    
    printf("\n***** Game Ends *****\n");
    if( (hand1==NULL) && (hand2==NULL) ){ /* match tied */
        printf("=== Match Tied ===\n\n");
    }
    else if(hand1 != NULL){ /* player-1 wins */
        printf("=== Player-1 Wins ===\n\n");
    }
    else if(hand2 != NULL){ /* player-2 wins */
        printf("=== Player-2 Wins ===\n\n");
    }

    return 0;
}
