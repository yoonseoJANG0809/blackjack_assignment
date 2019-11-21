#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
int usercardCount[N_MAX_USER+1];			//how maony card player get


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
	char *strCardShape[] = {"CLV", "DIA", "HRT", "SPD"};
	char *strCardNumber[] = {"A ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "J ", "Q ", "K "};   //'10' occupy two place so other number spaced
	
	int cardShape;
	int cardNumber;
	
	cardShape = (cardnum / 13);				//quotient when cardnum/13
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
	int card_loc;							//The location of current card
	int rand_loc;							//The location of random
	int card_loc_num;						//The location of current card's number
	int rand_loc_num;						//The location of random's number
	
	for(i=0; i < N_CARDSET*N_CARD; i++)
		CardTray[i] = i;
		
	for(card_loc=0; card_loc < N_CARDSET*N_CARD; card_loc++){
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
		while(getchar() != '\n');								//https://hashcode.co.kr/questions/2958/c%EC%96%B8%EC%96%B4-%EC%A0%95%EC%88%98-%EC%9E%85%EB%A0%A5-scanf%EC%97%90%EC%84%9C-%EB%AC%B8%EC%9E%90%EC%97%B4-%EC%9E%85%EB%A0%A5-%EC%8B%9C-%EC%98%88%EC%99%B8-%EC%B2%98%EB%A6%AC
		//can't put word. Just number
		
		if(n_user <= 5 && n_user >= 1){
			
		}
		else if(n_user > 5){
			printf("Too many players!!!\n");
		}
		else{
			printf("invalid input players!\n");
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
	
	do{
		printf("\n ------- BETTING STEP ------- \n");
		printf("  -> your betting (total : $%d) : $", dollar[1]);
		scanf("%d", &getBetting);
		while(getchar() != '\n');								//https://hashcode.co.kr/questions/2958/c%EC%96%B8%EC%96%B4-%EC%A0%95%EC%88%98-%EC%9E%85%EB%A0%A5-scanf%EC%97%90%EC%84%9C-%EB%AC%B8%EC%9E%90%EC%97%B4-%EC%9E%85%EB%A0%A5-%EC%8B%9C-%EC%98%88%EC%99%B8-%EC%B2%98%EB%A6%AC
		//can't put word. Just number
	
		if(getBetting <= dollar[1] && getBetting >=1){
			
		}
		else if(getBetting > dollar[1]){
			printf("  -> you only have $%d! bet again\n", dollar[1]);
		}
		else{
			printf("  -> invalid input for betting\n");
		}
	}while(getBetting > dollar[1] || getBetting < 1);
	
	bet[1] = getBetting;
	
	//other players bet money randomly
	getBetting = 0;
	for(i=2;i<=n_user;i++){
		
		getBetting = rand()%9 + 1;								//other players bet between $1 to $9
		
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
		
		usercardCount[i] = 1;									//player get 2 cards. from 0
	}
	/*															//Server is user0
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
	
	printf("\n----------- CARD OFFERING ---------------\n");
	
		strcpy( card1, printCard(cardhold[0][0]) );
		strcpy( card2, printCard(cardhold[0][1]) );
		
		//printf("	server	:	%s %s\n", card1, card2);
		printf("	server	:	XX    %s\n", card2);
		
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

}

int SUM(int num1, int num2){
	int sum;
		
	if(num1 == 1 && num1 == 1){					  							//2card is ace
		sum = 1 + 11;														//11+11-->overflow. So 1+11
	}
	else if(num1 == 1 || num2 == 1){
		if((num1 + num2 + 10) >= 17 && (num1 + num2 + 10) <= 21){			//When the sum of the cards is between 17 and 21, ace is 11
			sum = (num1 + num2 + 10);
		}
		else if((num1 + num2) > 21){										//when the sum of the cards is over 21, ace is 1
			sum = (num1 + num2);
		}
		else{																//when the sum of the card is less 17, ace is 11
			sum = (num1 + num2 + 10);
		}
	}
	else{
		sum = num1 + num2;
	}
	return sum;
}

int getAction(int user) {
	char cardPrint[3];
	int GoorStay = 0;		//Go or Stay
	int Nextcardhold = 0;
	int i;
	
	if(user == 0){
		printf("server turn  :  ", cardSum[user]);
	}
	else if(user == 1){
		printf("your turn  :  ", cardSum[user]);
	}
	else{
		printf("player%d turn  :  ", user-1, cardSum[user]);
	}
	
	for(i=0; i <= usercardCount[user]; i++){
		strcpy(cardPrint, printCard(cardhold[user][i]));
		printf("%s ", cardPrint);
	}
	
	if(user == 1){
		printf(" ==> Action? (0 - go, others - stay) : ");
		scanf("%d", &GoorStay);
		while(getchar() != '\n');								//https://hashcode.co.kr/questions/2958/c%EC%96%B8%EC%96%B4-%EC%A0%95%EC%88%98-%EC%9E%85%EB%A0%A5-scanf%EC%97%90%EC%84%9C-%EB%AC%B8%EC%9E%90%EC%97%B4-%EC%9E%85%EB%A0%A5-%EC%8B%9C-%EC%98%88%EC%99%B8-%EC%B2%98%EB%A6%AC
		//can't put word. Just number
		
		if(GoorStay == 0){										//GO
			Nextcardhold = usercardCount[user] + 1;
			cardhold[user][Nextcardhold] = pullCard();
			usercardCount[user] = Nextcardhold;
			cardSum[user] = SUM (cardSum[user], getCardNum(cardhold[user][Nextcardhold]));

			if( cardSum[user] == 21){
				for(i=0;i<=usercardCount[user];i++){
						strcpy(cardPrint, printCard(cardhold[user][i]));
						printf("%s ", cardPrint);
					}
				printf("::: Black Jack!congratulation, you win!! --> +$%d ($%d)", bet[user]*2, dollar[user] + bet[user]*2);
				
				printf("\n\n");
				return 1;
			}
			else if( cardSum[user] > 21){
				for(i=0;i<=usercardCount[user];i++){
						strcpy(cardPrint, printCard(cardhold[user][i]));
						printf("%s ", cardPrint);
					}
				printf("::: DEAD (sum:%d) --> -$%d ($%d)", cardSum[user], bet[user], dollar[user] - bet[user]);
				
				printf("\n\n");
				return 1;
			}
			else{
				return 0;
			}
		}
		else{													//STAY
			printf("\n\n");
			return 1;
		}
	}
	else{
		if(cardSum[user] < 17){
			printf(" ==> Action? (0 - go, others - stay) : Go\n");	
			Nextcardhold = usercardCount[user]+1;
			cardhold[user][Nextcardhold] = pullCard();
			usercardCount[user] = Nextcardhold;
			cardSum[user] = SUM (cardSum[user], getCardNum(cardhold[user][Nextcardhold]));
						
			if( cardSum[user] == 21){
				for(i=0;i<=usercardCount[user];i++){
						strcpy(cardPrint, printCard(cardhold[user][i]));
						printf("%s ", cardPrint);
					}
				printf("::: Black Jack!congratulation, you win!! --> +$%d ($%d)\n", bet[user]*2, dollar[user] + bet[user]*2);
					
				printf("\n\n");
				return 1;
			}
			else if( cardSum[user] > 21){
				if(user == 0){
					for(i=0;i<=usercardCount[user];i++){
						strcpy(cardPrint, printCard(cardhold[user][i]));
						printf("%s ", cardPrint);
					}
					printf("::: server DEAD (sum:%d)", cardSum[user]);
				}
				else{
					for(i=0;i<=usercardCount[user];i++){
						strcpy(cardPrint, printCard(cardhold[user][i]));
						printf("%s ", cardPrint);
					}
					printf("::: DEAD (sum:%d) --> -$%d ($%d)\n", cardSum[user], bet[user], dollar[user] - bet[user]);
				}
				
				printf("\n\n");
				return 1;
			}
			else{
				return 0;
			}
			return 0;
		}
		else{
			printf(" ==> Action? (0 - go, others - stay) : Stay\n\n\n");
			return 1;
		}
	}
	
}


void printUserCardStatus(int user, int cardcnt) {
	int i;
	
	//printf("   -> card : ");
	for (i=0;i<cardcnt;i++)
		printCard(cardhold[user][i]);
	//printf("\t ::: ");
}




// calculate the card sum and see if : 1. under 21, 2. over 21, 3. blackjack
int calcStepResult() {
	int i = 0;
	int j;	
	int sumCard = 0;															//sum of all cards
	int currCard = 0;															//card to add
	int keepRound = 0;															//keep round: 0, stop reound: 1
	
	for(i=0;i<=n_user;i++){
		sumCard = getCardNum ( cardhold[i][0] );
		currCard = getCardNum ( cardhold[i][1] );
		sumCard = SUM (sumCard, currCard);
				
		for(j=2;j<=usercardCount[i];j++){
			currCard = getCardNum ( cardhold [i][j] );
			sumCard = SUM (sumCard, currCard);									//add by accumulating the card 
		}
		cardSum[i] = sumCard;
		if( cardSum[i] == 21 ){
			keepRound = 1;
		}
	}
	return keepRound;
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
	int i;
	int winnerMoney = 0;					//winner's money
	
	printf(" -------------------------------------------\n\n -------------------------------------------\n\n -------------------------------------------\n\n -------------------------------------------\n\ngame end!");
	
	for(i=1;i<=n_user;i++){
		if(i == 1){
			printf("\n\nyour money : $%d\n", dollar[i]);
		}
		else{
			printf("player %d money : $%d\n", i-1, dollar[i]);
		}
			
		if(winnerMoney <= dollar[i]){
			winnerMoney = dollar[i];
		}
	}


	for(i=1;i<=n_user;i++){
		if(winnerMoney == dollar[i]){
			if(i == 1){
				printf("you win");
			}
			else{
				printf("player %d win", i-1);
			}
		}
	}
}



int main(int argc, char *argv[]) {
	int roundIndex = 0;
	int max_user;
	int i;
	int keepRound = 0;						//continue round: 0, stop round: 1
	int userGoorStay = 0;					//0: Go, 1: Stay
	
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
		
		keepRound = calcStepResult();
		if(keepRound == 1){														//if two card is black jack
			checkResult();
		}
		
		//each player's turn
		else{
		
			for (i=1;i<=n_user;i++) 											//each player
			{
				userGoorStay = 0;
				do{ 														   	//do until the player dies or player says stop
					printUserCardStatus(i, usercardCount[i]);					//print current card status printUserCardStatus();
					if(cardIndex == (N_CARDSET*N_CARD-1)){						//game end if thers is no card
						goto Endpoint;
					}
					userGoorStay = getAction(i);								//keep round if at least one player says go
				}while(userGoorStay == 0);
			}
			
			userGoorStay = 0;
			do{ 														    	//do until the player dies or player says stop
					printUserCardStatus(0, usercardCount[0]);					//print current card status printUserCardStatus();
					if(cardIndex == (N_CARDSET*N_CARD-1)){						//game end if thers is no card
						goto Endpoint;
					}
					userGoorStay = getAction(0);								//keep round if at least one player says go
				}while(userGoorStay == 0);
			//result
			checkResult();
	}
		for(i=1; i <= n_user; i++){												//no moneu_players
			if(dollar[i] == 0){
				gameEnd = 1;
			}
		}
		
		if((N_CARDSET*N_CARD-cardIndex) < ((n_user+1)*2)){						//no card
			gameEnd = 1;
		}
	} while (gameEnd == 0);
	
	Endpoint:
	checkWinner();
	
	
	return 0;
}
