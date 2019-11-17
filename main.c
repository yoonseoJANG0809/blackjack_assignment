#include <stdio.h>
#include <stdlib.h>

#define N_CARDSET			1
#define N_CARD				52
#define N_DOLLAR			50


#define N_MAX_CARDNUM		13
#define N_MAX_USER			5
#define N_MAX_CARDHOLD		10
#define N_MAX_GO			17
#define N_MAX_BET			5

#define N_MIN_ENDCARD		30


//card tray object
int CardTray[N_CARDSET*N_CARD];
int cardIndex = 0;							


//player info
int dollar[N_MAX_USER+1];					//dollars that each player has. server=0, you=1, player1=2, ...
int n_user;									//number of users


//play yard information
int cardhold[N_MAX_USER+1][N_MAX_CARDHOLD];	//cards that currently the players and server hold
int cardSum[N_MAX_USER+1];					//sum of the cards of server and players (include you)
int bet[N_MAX_USER+1];						//current betting 
int gameEnd = 0; 							//game end flag
int cardsuit[];							// return cardshape that apply to cardnum
int roundNo = 0;								//show which round is it


//card processing functions ---------------

//calculate the actual card number in the blackjack game
int getCardNum(int cardnum) {
	if ((cardnum%13+1) >= 10)
		return 10;
	else
		return (cardnum%13+1);
}

//print the card information (e.g. DiaA)
void printCard(int cardnum) {
	char *strCardShape[] = {"¢À", "¡ß", "¢¾", "¢¼"};
	char *strCardNumber[] = {"A ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "J ", "Q ", "K "};   //'10' occupy two place so other number spaced
	
	int cardshape;
	int cardnumber;
	
	cardshape = (cardnum / 13);			//quotient when cardnum/13
	cardnumber = (cardnum % 13);	
	
	strcpy(cardsuit, "");
	strcat(cardsuit, strCardShape[cardshape]);
	strcat(cardsuit, strCardNumber[cardnumber]);
	
	return cardsuit;
}


//card array controllers -------------------------------

//mix the card sets and put in the array
int mixCardTray(void) {

}

//get one card from the tray
int pullCard(void) {
}


//playing game functions -----------------------------

//player settiing
int configUser(void) {
	do{
		printf("input the number of players(Max:5) :" );
		
		scanf("%d", &n_user);
		
		if(n_user <= 5 && n_user >= 1){
			
		}
		else if(n_user > 5){
			printf("Too many players!!!\n");
		}
		else{
			printf("invalid inpur players!\n");
		}
	}while(n_user > 5 || n_user < 1);
	
}


//betting
int betDollar(void) {
	int getBetting;
	int i;
	
	roundNo = roundNo + 1;
	printf("\n------------------------------------------------------\n");
	printf("-------ROUND %d  (Card Index : %d)-------", roundNo, cardIndex);
	printf("\n------------------------------------------------------\n");
	
	printf(" ------- BETTING STEP ------- \n");
	do{
		printf("  -> your betting (total : $%d) : $", dollar[1]);
		scanf("%d", &getBetting);
		
		if(getBetting <= dollar[1] && getBetting >=1){
			
		}
		else if(getBetting > dollar[1]){
			printf("  -> you only have $50! bet again\n");
		}
		else{
			printf("  -> invalid input for betting\n");
		}
	}while(getBetting > dollar[1] || getBetting <1);
	
	bet[1] = getBetting;
	
	//other players bet money randomly
	getBetting = 0;
	for(i=2;i<=n_user;i++){
		getBetting = rand()%9 + 1;			//other players bet between $1 to $9
		if(getBetting > dollar[i]){
			bet[i] = dollar[i];
		}
		else{
			bet[i] = getBetting;
		}
		printf("  -> player%d bets $%d (out of %d)\n", i-1, dollar[i], bet[i]);
	}
}


//offering initial 2 cards
void offerCards(void) {
	int i;
	//1. give two card for each players
	for (i=0;i<n_user;i++)
	{
		cardhold[i][0] = pullCard();
		cardhold[i][1] = pullCard();
	}
	//2. give two card for the operator
	cardhold[n_user][0] = pullCard();
	cardhold[n_user][1] = pullCard();
	
	return;
}

//print initial card status
void printCardInitialStatus(void) {
	
}

int getAction(void) {
	
}


void printUserCardStatus(int user, int cardcnt) {
	int i;
	
	printf("   -> card : ");
	for (i=0;i<cardcnt;i++)
		printCard(cardhold[user][i]);
	printf("\t ::: ");
}




// calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
int calcStepResult() {
	
}

int checkResult() {
	
}

int checkWinner() {
	
}



int main(int argc, char *argv[]) {
	int roundIndex = 0;
	int max_user;
	int i;
	
	srand((unsigned)time(NULL));
	
	//set the number of players
	configUser();


	//Game initialization --------
	//1. players' dollar
	for(i=1; i<=n_user; i++){
		dollar[i]=N_DOLLAR;
	}
	
	//2. card tray
	mixCardTray();



	//Game start --------
	do {
		
		betDollar();
		offerCards(); //1. give cards to all the players
		
		printCardInitialStatus();
		printf("\n------------------ GAME start --------------------------\n");
		
		//each player's turn
		/*for (;;;) //each player
		{
			while () //do until the player dies or player says stop
			{
				//print current card status printUserCardStatus();
				//check the card status ::: calcStepResult()
				//GO? STOP? ::: getAction()
				//check if the turn ends or not
			}
		}
		*/
		//result
		checkResult();
	} while (gameEnd == 0);
	
	checkWinner();
	
	
	return 0;
}
