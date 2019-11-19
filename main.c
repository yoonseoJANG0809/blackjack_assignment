#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int gameEnd = 0; 							//game end flag . No money or No card 
char cardsuit[3];							// return cardshape that apply to cardnum. shape and number occupy max 3 places
int roundNo = 0;							//show which round is it
int usercardCount[N_MAX_USER+1];

//card processing functions ---------------

//calculate the actual card number in the blackjack game
int getCardNum(int cardnum) {
	if ((cardnum%13+1) >= 10)
		return 10;
	else
		return (cardnum%13+1);
}

//print the card information (e.g. DiaA)
char *printCard(int cardnum) {
	char *strCardShape[] = {"¢À", "¡ß", "¢¾", "¢¼"};
	char *strCardNumber[] = {"A ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "J ", "Q ", "K "};   //'10' occupy two place so other number spaced
	
	int cardShape;
	int cardNumber;
	
	cardShape = (cardnum / 13);			//quotient when cardnum/13
	cardNumber = (cardnum % 13);	
	
	strcpy(cardsuit, "");
	strcat(cardsuit, strCardShape[cardShape]);
	strcat(cardsuit, strCardNumber[cardNumber]);
	
	return cardsuit;
}


//card array controllers -------------------------------

//mix the card sets and put in the array
//rand funstion can occur overlap. So select one card and change position with other card
int mixCardTray(void) {
	int i;
	int card_loc;				//The location of current card
	int rand_loc;				//The location of random
	int card_loc_num;			//The location of current card's number
	int rand_loc_num;			//The location of random's number
	
	for(i=0;i<N_CARDSET*N_CARD;i++)
		CardTray[i] = i;
	for(card_loc=0;card_loc<N_CARDSET*N_CARD;card_loc++){
		rand_loc = rand() % (N_CARDSET*N_CARD);
		
		//Before chage
		card_loc_num = CardTray[card_loc];
		rand_loc_num = CardTray[rand_loc];
		
		//change
		CardTray[card_loc] = rand_loc_num;
		CardTray[rand_loc] = card_loc_num;
	}
}

//get one card from the tray
int pullCard(void) {
	int i;
	
	i = CardTray[cardIndex];
	cardIndex = cardIndex + 1;
	
	return i;
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
			printf("  -> you only have $%d! bet again\n", dollar[1]);
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
		printf("  -> player%d bets $%d (out of %d)\n", i-1, bet[i], dollar[i]);
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
	/*													//Server is user0
	//2. give two card for the operator
	cardhold[n_user][0] = pullCard();
	cardhold[n_user][1] = pullCard();
	*/
	return;
}

//print initial card status
void printCardInitialStatus(void) {
	int i;
	char card1[3];
	char card2[3];
	int sumcard1;
	int sumcard2;
	int user;
	
	printf("\n----------- CARD OFFERING ---------------\n");
	
		strcpy( card1, printCard(cardhold[0][0]) );
		strcpy( card2, printCard(cardhold[0][1]) );
		
		printf("	server	:	%s %s\n", card1, card2);
	for(i=1;i<n_user;i++){
		strcpy( card1, printCard(cardhold[i][0]) );
		strcpy( card2, printCard(cardhold[i][1]) );
		
		if(i == 1){
			printf("	you	:	%s %s\n", card1, card2);
		}
		else{
			printf("	player%d :	%s %s\n", i-1, card1, card2);
		}
	}	
	
	if(sumcard1 == 1 && sumcard2 == 1){					  							//2card is ace
		cardSum[user] = 1+11;														//11+11-->overflow. So 1+11
	}
	else if(sumcard1 == 1 || sumcard2 == 1){
		if((sumcard1 + sumcard2 + 10) >= 17 && (sumcard1 + sumcard2 + 10) <= 21){	//When the sum of the cards is between 17 and 21, ace is 11
			cardSum[user] = (sumcard1 + sumcard2 + 10);
		}
		else if((sumcard1 + sumcard2) > 21){										//when the sum of the cards is over 21, ace is 1
			cardSum[user] = (sumcard1 + sumcard2);
		}
		else{																		//when the sum of the card is less 17, ace is 11
			cardSum[user] = (sumcard1 + sumcard2 + 10);
		}
	}
}

int getAction(void) {
	char cardPrint[3];
	int user;
	int GoorStay;		//Go or Stay
	int Nextcardhold;
	
	if(user == 0){
		printf("servers turn  :  (sum=%d)", cardSum[user]);
	}
	else if(user == 1){
		printf("your turn  :  (sum=%d)", cardSum[user]);
	}
	else{
		printf("player%d turn  :  (sum=%d)", user-1, cardSum[user]);
	}
	
	if(user == 1){
		printf(" ==> Action? (0 - go, others - stay) : ");
		scanf("%d", &GoorStay);
		
		if(GoorStay == 0){												//GO
			Nextcardhold = usercardCount[user]+1;
			cardhold[user][Nextcardhold] = pullCard();
			usercardCount[user] = Nextcardhold;
			if( cardSum[user] == 21){
				printf("::: Black Jack!congratulation, you win!! --> +$%d ($52)", bet[user]*2, dollar[user]);
			}
			else if( cardSum[user] > 21){
				printf("::: DEAD (sum:%d) --> -$%d ($%d)", cardSum[user], bet[user]*2, dollar[user]);
			}
			else{
				
			}
		}
		else{															//STAY
			
		}
	}
	else{
		if(cardSum[user] > 17){
			printf(" ==> Action? (0 - go, others - stay) : Go\n");	
			Nextcardhold = usercardCount[user]+1;
			cardhold[user][Nextcardhold] = pullCard();
			usercardCount[user] = Nextcardhold;
			if( cardSum[user] == 21){
				printf("::: Black Jack!congratulation, you win!! --> +$%d ($%d)", bet[user]*2, dollar[user]);
			}
			else if( cardSum[user] > 21){
				printf("::: DEAD (sum:%d) --> -$%d ($%d)", cardSum[user], bet[user]*2, dollar[user]);
			}
			else{
				
			}
		}
		else{
			printf(" ==> Action? (0 - go, others - stay) : Stay\n");
		}
	}
	
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
	int i;
	int j;
	int sumCard;			//sum of all cards
	int currCard;			//card to add
	int keepround = 0;		//keep round: 0, stop reound: 1
	
	for(i=0;i<=n_user;i++){
		for(j=1;j<=usercardCount;j++){
			currCard = getCardNum( cardhold [i][j] );
			sumCard = sumCard + currCard;					//add by accumulating the card 
		}
		cardSum[i] = sumCard;
		if( cardSum[i] == 21 ){
			keepround = 1;
		}
	}
	return keepround;
}

int checkResult() {
	int i;
	
	printf("\n -------------------- ROUND %d result ....\n", roundNo);
	
	for(i=1;i<=n_user;i++){
		if( i == 1 ){
			printf("   -> your result : ");
		}
		else{
			printf("   -> %d'th player's result : ", i-1);
		}
		
		
		if( cardSum[i] == 21 ){
			dollar[i] = dollar[i] + bet[i] * 2;									//blackjack -> get bet dollar * 2
			printf(" BlackJack! win ($%d)\n", dollar[i]);
		}
		else if( cardSum[i] > 21 ){
			dollar[i] = dollar[i] - bet[i];
			printf(" lose due to overflow! ($%d)\n", dollar[i]);
		}
		else if( cardSum[0] > 21 && cardSum[i] <21){
			dollar[i] = dollar[i] + bet[i];
			printf(" win (sum:%d) --> $%d\n", cardSum[i], dollar[i]);
		} 
		else if(cardSum[0] <= 21 && cardSum[i] < cardSum[0]){
			dollar[i] = dollar[i] - bet[i];
			printf(" lose! (sum:%d) --> $%d\n", cardSum[i], dollar[i]);
		}
		else if(cardSum[i] < 21 && cardSum[i] >= cardSum[0]){
			dollar[i] = dollar[i] + bet[i];
			printf(" win (sum:%d) --> $%d\n", cardSum[i], dollar[i]);
		}
	}
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
		gameEnd == 1;
	} while (gameEnd == 0);
	
	checkWinner();
	
	
	return 0;
}
