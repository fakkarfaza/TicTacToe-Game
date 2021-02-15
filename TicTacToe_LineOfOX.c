#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <pthread.h>
#include <time.h>


// Global Variable
	typedef struct{
		char name[100];
		int win;
		int lose;
		int draw;
	}Data;

	int tableSCORE[7][7]; //bunshin table for scoring for minimax purpose
	char tableBOT[7][7];//duplicate of table mark
	Data PlayerHistory[11];
	Data BotHistory[11];

//Modul
int CheckWin(int board, char mark[7][7]); // Condition if someone win the game
int Plyr(); //Choose your opponent
int SelectBoard (); //pick board size
int SelectMenu(); //To continue from main menu to pre game
void Board(int board,char table[7][7],char string1[], char string2[], int turn); //Summon game board
void BotTurn(int board, char table[7][7], char BotDiff[10], int turn); //BOT movement
void drawBoard (char table[7][7], int *turn, int board); //To take player input to move and give mark on the game board 
void EndGame(int PlayStatus, char Player1[100], char Player2[100]); //UI when The Game Ends
void footer(); //UI footer
void header(); //UI header
void help(); //Help / Instruction to play the game
void MainMenu(); //UI of Main Menu
void PlyrName (char string1[], char string2[], int result, char BotDiff[]); //Naming the players
void PlyrTurn(int *turn); //give turn a player to play
void quickmath(char table[7][7], int turn, int board, int *pass); //to make player do quick math before entering a move
void Scoring(int board, int step, int turn, char BotDiff[10]); //for Minimax scoring
void sortdata(); //for sorting player history record based on player wins
void scoreboard(); // leaderboard for top 10 player
void storedata(char player1[25],char player[25],int result,int mode); // store data score board
void savedata(); // save data in score board



int main () {
	
	system("color 30");
	//srand(time(NULL)); //for BOT random move
	//Data and data type declaration
	char BotDiff[10] = {}; //a parameter to be used in module, to decide bot's difficulty
	char mark[7][7]; //Table which contain the 'X' and 'O' mark inside the board
	char Player1[100]={}; //PLayer 1 Name
	char Player2[100]={}; //Player 2 Name
	int board; //a parameter to be used in a module, to determine board size
	int functional; //
	int i,j; //for index purposes
	int menu=1; //
	int PlayStatus; //to look at the game situation at the moment
	int result; // 
	int turn=1; //a parameter to be used in a module, to give turn to players
	int pass;
	
	for (i=0;i<11;i++){ //initialization of scoreboard
		strcpy(PlayerHistory[i].name,"-");
		PlayerHistory[i].win = 0;
		PlayerHistory[i].lose = 0;
		PlayerHistory[i].draw = 0;
		strcpy(BotHistory[i].name,"-");
		BotHistory[i].win = 0;
		BotHistory[i].lose = 0;
		BotHistory[i].draw = 0;
	}
		
	while(menu == 1){ //run game
	
	for (i=0;i<7;i++){ //Make the table empty clear
		for (j=0;j<7;j++){
			mark[i][j]=' ';
		}
	}	
		
	// Main Menu
	
	system ("cls");
	MainMenu();
	menu = SelectMenu();
	if (menu == 0){ //if user don't choose the available option on the menu
		menu=1;
		continue;
	}
	else if (menu == 2){ //open help command to paly the game (rules and instructions)
		help();
		menu=1;
		continue;
	}
	else if (menu == 3){ //open scoreboard
		scoreboard();
		menu=1;
		continue;
	}
	else if (menu == 4) { //exit the game
		system ("cls");
		printf ("See you next time UwU\n\n\n\n\n");
		return 0;
	}
	
	/// Pre-Game
	result = Plyr(); // Pick Opponent
	if (result == 0) { //Pick Difficulty
		j=1;
		while (j!=0) {
		printf ("\nPick Your Difficulty : ");
		scanf ("%s", BotDiff);
		j=0;
		for (i=0; BotDiff[i]; i++) {
			BotDiff[i] = tolower(BotDiff[i]);
		}
			if (!(strcmp(BotDiff, "easy") == 0 || strcmp(BotDiff, "medium") == 0 || strcmp(BotDiff, "hard") == 0)) {
				printf ("\nPlease input suitable input");
				j=1;
			}
		}
	}
	PlyrName(Player1, Player2, result, BotDiff); //Naming the player
	board = SelectBoard(); //select the type of board to play
	Board(board,mark, Player1, Player2, turn); //summoning game board
	
	/// In-Game
	if (result == 1) {  // versus Player
		PlayStatus = -1;
		while (PlayStatus == -1) {
			quickmath(mark,turn,board,&pass);
			if (pass == 1){
			Board(board,mark, Player1, Player2, turn);
			drawBoard(mark, &turn, board);
			}
			PlyrTurn(&turn);
			Board(board,mark, Player1, Player2, turn);
			PlayStatus = CheckWin (board, mark);
		}
	}
	else if (result == 0) { // versus BOT
		PlayStatus = -1;
		while (PlayStatus == -1) {
			quickmath(mark,turn,board,&pass);
			if (pass == 1){
			Board(board,mark, Player1, Player2, turn);
			drawBoard(mark, &turn, board);
			}
			PlayStatus = CheckWin (board, mark);
			if (PlayStatus != -1) 
				break;
			BotTurn(board, mark, BotDiff, 2);
			Board(board,mark, Player1, Player2, turn);
			PlayStatus = CheckWin (board, mark);
		}
	}
	storedata(Player1,Player2,PlayStatus,result);
	sortdata();
	savedata();
	EndGame(PlayStatus,Player1,Player2);
	
	footer();
		
	printf("\n\t\t     ");
	system ("pause");

	turn = 1;
	}
	return 0;					
}
int Plyr () {
	
	char Opponent [10]={};
	int i;
	
	while (1) {
	system ("cls");
	header();
	printf ("\n\t\t\tPick your opponent!\n\n");
	printf ("\t\tPlayer\t\t\t    BOT\n");
	scanf ("%s", Opponent);
	for (i=0; Opponent[i]; i++) {
		Opponent[i] = tolower(Opponent[i]);
	}
		if (strcmp (Opponent, "player") == 0)
			return 1;
		else if (strcmp (Opponent, "bot") == 0)
			return 0;
		}
}
	
void PlyrName(char string1[], char string2[], int result, char BotDiff[]) {
	
	int i;
	
	fflush(stdin);
	system ("cls");
	header();
	
	printf ("\nInput Player 1 Name : ");
	scanf ("%[^\n]%*c", string1);
	for (i=0; string1[i]; i++) { //make player 1 name all uppercase
		string1[i] = toupper(string1[i]);
	}
	
	if (result == 1) {
		while (1) {
		printf ("\nInput Player 2 Name : ");
		fflush(stdin);
		scanf ("%[^\n]%*c", string2);
		for (i=0; string2[i]; i++) { //make player 2 name all uppercase
			string2[i] = toupper(string2[i]);
		}
		if (strcmp (string2, string1) != 0)
			break;
		else 
			printf ("Can't use the two same names for one game!");
		}
	}
	
	if (result == 0) { //Player 2 name automatically named BOT if Player pick BOT as opponent
		strcpy (string2, "BOT ");
		strcat (string2, BotDiff);
	}
	


}
		
void Board(int board,char table[7][7], char string1[], char string2[], int turn) {
	
	system ("cls");
	// PRINT Board
	if (turn == 1) { // Player 1 Turn 
		if (board == 3) {
			printf ("\t     1      2      3\n");
			printf ("\t  o-------------------o\n");
			printf("\t  |      |     |      |     %s'S TURN (X) \n", string1);
			printf ("\t1 |   %c  |  %c  |  %c   |\n", table[0][0],table[0][1],table[0][2]);
			printf("\t  | _____|_____|_____ |\n");
			printf("\t  |      |     |      |\n");
			printf ("\t2 |   %c  |  %c  |  %c   |\n", table[1][0],table[1][1],table[1][2]);
			printf("\t  | _____|_____|_____ |    %s\n", string2);
			printf("\t  |      |     |      |\n");
			printf ("\t3 |   %c  |  %c  |  %c   |\n", table[2][0],table[2][1],table[2][2]);
			printf("\t  |      |     |      |\n");
			printf("\t  o-------------------o\n");
		}
		if (board == 5) {
			printf ("\t\t     1      2      3     4     5\n");
			printf ("\t\t  o-------------------------------o\n");
			printf("\t\t  |      |     |     |     |      |   %s'S TURN (X) \n", string1);
			printf ("\t\t1 |   %c  |  %c  |  %c  |  %c  |  %c   |\n", table[0][0],table[0][1],table[0][2],table[0][3],table[0][4]);
			printf("\t\t  | _____|_____|_____|_____|_____ |\n");
			printf("\t\t  |      |     |     |     |      |\n");
			printf ("\t\t2 |   %c  |  %c  |  %c  |  %c  |  %c   |\n", table[1][0],table[1][1],table[1][2],table[1][3],table[1][4]);
			printf("\t\t  | _____|_____|_____|_____|_____ |   %s\n", string2);
			printf("\t\t  |      |     |     |     |      |\n");
			printf ("\t\t3 |   %c  |  %c  |  %c  |  %c  |  %c   |\n", table[2][0],table[2][1],table[2][2],table[2][3],table[2][4]);
			printf("\t\t  | _____|_____|_____|_____|_____ |\n");
			printf("\t\t  |      |     |     |     |      |\n");
			printf ("\t\t4 |   %c  |  %c  |  %c  |  %c  |  %c   |\n", table[3][0],table[3][1],table[3][2],table[3][3],table[3][4]);
			printf("\t\t  | _____|_____|_____|_____|_____ |\n");
			printf("\t\t  |      |     |     |     |      |\n");
			printf ("\t\t5 |   %c  |  %c  |  %c  |  %c  |  %c   |\n", table[4][0],table[4][1],table[4][2],table[4][3],table[4][4]);
			printf("\t\t  |      |     |     |     |      |\n");
			printf ("\t\t  o-------------------------------o\n");
		}
		if (board == 7) {
			printf ("\t\t\t     1      2      3     4     5     6     7\n");
			printf ("\t\t\t  o-------------------------------------------o\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |   %s'S TURN (X) \n", string1);
			printf ("\t\t\t1 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[0][0],table[0][1],table[0][2],table[0][3],table[0][4],table[0][5],table[0][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t2 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[1][0],table[1][1],table[1][2],table[1][3],table[1][4],table[1][5],table[1][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |    %s\n", string2);
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t3 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[2][0],table[2][1],table[2][2],table[2][3],table[2][4],table[2][5],table[2][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t4 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[3][0],table[3][1],table[3][2],table[3][3],table[3][4],table[3][5],table[3][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t5 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[4][0],table[4][1],table[4][2],table[4][3],table[4][4],table[4][5],table[4][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t6 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[5][0],table[5][1],table[5][2],table[5][3],table[5][4],table[5][5],table[5][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t7 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[6][0],table[6][1],table[6][2],table[6][3],table[6][4],table[6][5],table[6][6]);
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t  o-------------------------------------------o\n");
		}
	}
	else if (turn == 2) { // Player 1 Turn 
		if (board == 3) {
			printf ("\t     1      2      3\n");
			printf ("\t  o-------------------o\n");
			printf("\t  |      |     |      |     %s\n", string1);
			printf ("\t1 |   %c  |  %c  |  %c   |\n", table[0][0],table[0][1],table[0][2]);
			printf("\t  | _____|_____|_____ |\n");
			printf("\t  |      |     |      |\n");
			printf ("\t2 |   %c  |  %c  |  %c   |\n", table[1][0],table[1][1],table[1][2]);
			printf("\t  | _____|_____|_____ |    %s'S TURN (O) \n", string2);
			printf("\t  |      |     |      |\n");
			printf ("\t3 |   %c  |  %c  |  %c   |\n", table[2][0],table[2][1],table[2][2]);
			printf("\t  |      |     |      |\n");
			printf("\t  o-------------------o\n");
		}
		if (board == 5) {
			printf ("\t\t     1      2      3     4     5\n");
			printf ("\t\t  o-------------------------------o\n");
			printf("\t\t  |      |     |     |     |      |   %s\n", string1);
			printf ("\t\t1 |   %c  |  %c  |  %c  |  %c  |  %c   |\n", table[0][0],table[0][1],table[0][2],table[0][3],table[0][4]);
			printf("\t\t  | _____|_____|_____|_____|_____ |\n");
			printf("\t\t  |      |     |     |     |      |\n");
			printf ("\t\t2 |   %c  |  %c  |  %c  |  %c  |  %c   |\n", table[1][0],table[1][1],table[1][2],table[1][3],table[1][4]);
			printf("\t\t  | _____|_____|_____|_____|_____ |   %s'S TURN (O) \n", string2);
			printf("\t\t  |      |     |     |     |      |\n");
			printf ("\t\t3 |   %c  |  %c  |  %c  |  %c  |  %c   |\n", table[2][0],table[2][1],table[2][2],table[2][3],table[2][4]);
			printf("\t\t  | _____|_____|_____|_____|_____ |\n");
			printf("\t\t  |      |     |     |     |      |\n");
			printf ("\t\t4 |   %c  |  %c  |  %c  |  %c  |  %c   |\n", table[3][0],table[3][1],table[3][2],table[3][3],table[3][4]);
			printf("\t\t  | _____|_____|_____|_____|_____ |\n");
			printf("\t\t  |      |     |     |     |      |\n");
			printf ("\t\t5 |   %c  |  %c  |  %c  |  %c  |  %c   |\n", table[4][0],table[4][1],table[4][2],table[4][3],table[4][4]);
			printf("\t\t  |      |     |     |     |      |\n");
			printf ("\t\t  o-------------------------------o\n");
		}
		if (board == 7) {
			printf ("\t\t\t     1      2      3     4     5     6     7\n");
			printf ("\t\t\t  o-------------------------------------------o\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |   %s\n", string1);
			printf ("\t\t\t1 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[0][0],table[0][1],table[0][2],table[0][3],table[0][4],table[0][5],table[0][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t2 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[1][0],table[1][1],table[1][2],table[1][3],table[1][4],table[1][5],table[1][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |    %s'S TURN (O)\n", string2);
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t3 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[2][0],table[2][1],table[2][2],table[2][3],table[2][4],table[2][5],table[2][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t4 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[3][0],table[3][1],table[3][2],table[3][3],table[3][4],table[3][5],table[3][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t5 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[4][0],table[4][1],table[4][2],table[4][3],table[4][4],table[4][5],table[4][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t6 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[5][0],table[5][1],table[5][2],table[5][3],table[5][4],table[5][5],table[5][6]);
			printf("\t\t\t  | _____|_____|_____|_____|_____|_____|_____ |\n");
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t7 |   %c  |  %c  |  %c  |  %c  |  %c  |  %c  |  %c   |\n", table[6][0],table[6][1],table[6][2],table[6][3],table[6][4],table[6][5],table[6][6]);
			printf("\t\t\t  |      |     |     |     |     |     |      |\n");
			printf ("\t\t\t  o-------------------------------------------o\n");
		}
	}
	
}

void PlyrTurn(int *turn) { 

	// Change Player Turn
	if (*turn == 1) 
		*turn = 2;
	else if (*turn == 2)
		*turn = 1;
	
}
	
int CheckWin(int board, char mark[7][7]) {

// Player 1 win = return 1
// Player 2 win = return 2
// Draw = return 0
// Continue = return -1 	
	
	if (board == 3) {
	
		int i,j;
		int count = 0;
	
	
		for (i=0; i<3; i++) {
			for (j=0; j<3; j++) {
			
			//horizontally
			if (mark[i][0] == 'X' && mark[i][1] == 'X' && mark[i][2] == 'X') 
				return 1;
			else if (mark[i][0] == 'O' && mark[i][1] == 'O' && mark[i][2] == 'O')
				return 2;
			// vertically
			else if (mark[0][i] == 'X' && mark[1][i] == 'X' && mark[2][i] == 'X')
				return 1;
			else if (mark[0][i] == 'O' && mark[1][i] == 'O' && mark[2][i] == 'O')
				return 2;
			// diagonally
			else if ((mark[0][0] == 'X' && mark[1][1] == 'X' && mark[2][2] == 'X') || (mark[0][2] == 'X' && mark[1][1] == 'X' && mark[2][0] == 'X' ))
				return 1;
			else if ((mark[0][0] == 'O' && mark[1][1] == 'O' && mark[2][2] == 'O') || (mark[0][2] == 'O' && mark[1][1] == 'O' && mark[2][0] == 'O' ))
				return 2;
			// draw
			else if (mark [i][j] != ' ')
				count++;
			}
		}
		
		if ( count == 9)
			return 0;
		else 
			return -1;			
	}	
	else if (board == 5){
		int i,j,k;
		int count = 0;
		for (k=0;k<2;k++){
			for (i=0;i<5;i++){
				for (j=0;j<5;j++){
					//horizontally
					if (mark[i][k]=='X' && mark[i][k+1]=='X' && mark [i][k+2]=='X' && mark[i][k+3]=='X')
						return 1;
					else if (mark[i][k]=='O' && mark[i][k+1]=='O' && mark [i][k+2]=='O' && mark[i][k+3]=='O')
						return 2;
					//vertically
					else if (mark[k][i]=='X' && mark[k+1][i]=='X' && mark [k+2][i]=='X' && mark[k+3][i]=='X')
						return 1;
					else if (mark[k][i]=='O' && mark[k+1][i]=='O' && mark [k+2][i]=='O' && mark[k+3][i]=='O')
						return 2;
					//diagonally
					else if (mark[0+i][0+j]=='X' && mark[1+i][1+j]=='X' && mark[2+i][2+j]=='X' && mark[3+i][3+j]=='X')
						return 1;
					else if (mark[0+i][0+j]=='O' && mark[1+i][1+j]=='O' && mark[2+i][2+j]=='O' && mark[3+i][3+j]=='O')
						return 2;
					else if (mark[3+i][0+j]=='X' && mark[2+i][1+j]=='X' && mark[1+i][2+j]=='X' && mark[0+i][3+j]=='X')
						return 1;
					else if (mark[3+i][0+j]=='O' && mark[2+i][1+j]=='O' && mark[1+i][2+j]=='O' && mark[0+i][3+j]=='O')
						return 2;
					//draw
					else if (mark[i][j]!=' ')
						count++;
				}
			}
		}
		if (count==50)
			return 0;
		else
			return -1;
	}
	else if (board == 7){
		int i,j,k;
		int count = 0;
		for (k=0;k<3;k++){
			for (i=0;i<7;i++){
				for (j=0;j<7;j++){
					//horizontally
					if (mark[i][k]=='X' && mark[i][k+1]=='X' && mark [i][k+2]=='X' && mark[i][k+3]=='X' && mark[i][k+4]=='X')
						return 1;
					else if (mark[i][k]=='O' && mark[i][k+1]=='O' && mark [i][k+2]=='O' && mark[i][k+3]=='O' && mark[i][k+4]=='O')
						return 2;
					//vertically
					else if (mark[k][i]=='X' && mark[k+1][i]=='X' && mark [k+2][i]=='X' && mark[k+3][i]=='X' && mark[k+4][i]=='X')
						return 1;
					else if (mark[k][i]=='O' && mark[k+1][i]=='O' && mark [k+2][i]=='O' && mark[k+3][i]=='O' && mark[k+4][i]=='O')
						return 2;
					//diagonally
					else if (mark[0+i][0+j]=='X' && mark[1+i][1+j]=='X' && mark[2+i][2+j]=='X' && mark[3+i][3+j]=='X' && mark[4+i][4+j]=='X')
						return 1;
					else if (mark[0+i][0+j]=='O' && mark[1+i][1+j]=='O' && mark[2+i][2+j]=='O' && mark[3+i][3+j]=='O' && mark[4+i][4+j]=='O')
						return 2;
					else if (mark[4+i][0+j]=='X' && mark[3+i][1+j]=='X' && mark[2+i][2+j]=='X' && mark[1+i][3+j]=='X' && mark[0+i][4+j]=='X')
						return 1;
					else if (mark[4+i][0+j]=='O' && mark[3+i][1+j]=='O' && mark[2+i][2+j]=='O' && mark[1+i][3+j]=='O' && mark[0+i][4+j]=='O')
						return 2;
					//draw
					else if (mark[i][j]!=' ')
						count++;
				}
			}
		}
		if (count==49*3)
			return 0;
		else
			return -1;
	}
	return 0;		
}
	
int SelectMenu(){
	
	int i;
	
	char menu[255];
	scanf("%s",&menu);
	for (i=0; menu[i]; i++) {
		menu[i] = tolower(menu[i]);
	}
		
	if (strcmp(menu,"start")==0){
		return 1;
	}
	else if (strcmp(menu,"help")==0){
		return 2;
	}
	else if (strcmp(menu,"scoreboard")==0){
		return 3;
	}
	else if (strcmp(menu,"exit")==0){
		return 4;
	}
	return 0;
}

int SelectBoard() {
	
	int j=1;
	int board;
	
	while (j!=0) {
	printf ("\nPick Your Board : ");
	scanf ("%d", &board);
	j=0;
		if (!(board == 3 || board == 5 || board==7)) {
				printf ("\nPlease input suitable input");
				j=1;
		}
	}
	return board;	
}	
	
void drawBoard (char table[7][7], int *turn, int board)	{
	int x,y;
	int loop = 0;

	while (loop == 0) {
		x=0;
		y=0;
		fflush(stdin);
		printf ("Insert your move : ");
		int awal = clock();
		scanf ("%d %d", &x, &y);
		int akhir = clock();
		if (akhir - awal > 10000){ //if player input more than 10 secs
			do {
				x = 1 + (rand() % board);
				y = 1 + (rand() % board);
			} while (table[x-1][y-1] != ' '); 
			printf ("YOU HAVE EXCEEDED THE TIME, PENALTY WILL BE GIVEN TO YOU \nPRESS ENTER TO CONTINUE");
			while (!kbhit()){}
		}
		if (x<1 || y<1 || x>board || y>board) {
				printf("Please input suitable number!!!\n"); 
				continue;
			}
		loop = (table [x-1][y-1] != ' ') ? 0 : 1;
	}
		
	table[x-1][y-1] = (*turn == 1) ? 'X' : 'O'; // assignment X or O in to the board
}
	
void help(){
	
	int language;

	while (language != 1 && language != 2) {
	fflush(stdin);
	system("cls");
	header();
	printf("\n\t\t    WHAT LANGUAGE DO YOU PREFER?\n\n");
	printf("\t\t  [1]English \t[2]Bahasa Indonesia\n");
	printf("INPUT: ");
	scanf ("%d", &language);
	}
	footer();
	
	if (language == 1) { //if english
		system("cls");
		header();
		printf("\n\t\t\t     HOW TO PLAY:\n\n");
		printf("1. In Main Menu, type 'start' to play the game\n\n");
		printf("2. Choose the opponent that you want to play with.Type 'Player' \n   if you want to play againts your friend, or type 'Bot' if you \n   play this game alone\n");
		printf("   2.5. If you pick BOT as your oppponent, pick the difficulty you\n        want to play against. There are three choices you can make:\n        'easy','medium', and 'hard'\n\n");
		printf("3. Enter the player Name and input your friend name if you choose\n   player as your opponent\n\n");
		printf("4. Choose the board size that you want to play. Type '3' if you\n   want to play on 3x3 board, Type '5' for 5x5, and type '7' for 7x7\n\n");
		printf("5. Now the real game begins \n\n");
		printf("6. To make a move, you need to do 2 things. \n   First, You have to solve a simple math problem under 15 seconds \n   Second, type the desired square to make a move. e.g '1 1', '2 5', \n   '7 4'. You have 10 second to make your move\n\n");
		printf("7. If you make an incorrect answer to the math problem, penalty will \n   be given\n\n");
		printf("8. If you pay attention to point 6, you will notice that there \n   is time limit for everything. \n   IF you can't do the system want you to do before the time reach \n   the spesific amount, you will be given a Penalty. \n\n");
		printf("9. The penalty which has been mentioned above is the player will \n   given a random move on the board, rather than the input given by the player \n\n");
		printf("10. Have Fun ^_^ \n");
		printf("\n\n\t\t     Press enter key to continue\n");
		footer();
	}
	else if (language == 2) { //if bahasa
		system("cls");
		header();
		printf("\n\t\t\t     CARA BERMAIN:\n\n");
		printf("1. Pada menu awal, ketik 'start' untuk memulai permainan\n\n");
		printf("2. Pilih lawan yang anda ingin lawan. Ketik 'player' jika akan\n   bermain bersama seorang pemain, ketik 'bot' jika anda akan\n   memainkan permainan ini sendiri\n\n");
		printf("   2.5. Jika anda memilih bot sebagai lawan anda, pilih tingkat\n        kesulitan bot yang anda akan lawan. Ada 3 pilihan yang\n        bisa anda pilih:'easy','medium', and 'hard'\n\n");
		printf("3. Ketik nama pemain yang akan bermain\n\n");
		printf("4. Pilih papan bermain yang anda akan gunakan. Ada 3 pilhan papan\n   yang bisa dimainkan: '3','5','7'\n\n");
		printf("5. Sekarang, permainan yang sesungguhnya dimulai\n\n");
		printf("6. Untuk melakukan sebuah gerakan, kamu perlu melakukan 2 hal\n   Satu, kamu harus menyelesaikan satu permasalahan \n   matematika sederhana dalam 15 detik \n   Dua, Ketik lokasi kotak yang kamu mau dalam 10 detik. \n   Contoh '1 1', '2 5', '7 4'\n\n");
		printf("7. Jika pemain salah menjawab pertanyaan matematika, penalti akan \n   diberikan\n\n");
		printf("8. Jika kamu memerhatikan poin 6, kamu akan menyadari bahwa ada \n   batas waktu untuk segala hal \n   Jika kamu tidak bisa melakukan apa yang permainan mau dalam \n   waktu yang ditentukan, penalti akan diberikan\n\n");
		printf("9. Penalti yang disebut di poin-poin sebelumnya adalah gerakan\n   acak terlepas dari inputan yang diberikan nanti oleh pemain\n\n");
		printf("10.Selamat Bermain ^_^");
		printf("\n\n\t\t     Tekan enter untuk melanjutkan\n");
		footer();
	}
	while (!kbhit()) {}
}

void BotTurn(int board, char table[7][7], char BotDiff[10],int turn) {
	
	int i,j;
	int x,y;
	char table2[7][7];
	int min=1000000;
	
	//int tabel[7][7] dikosongin; variabel global 
	for (i=0; i<board; i++) {
		for (j=0; j<board; j++) {
				tableBOT[i][j] = table[i][j]; //tableBOT for BOT for original table bunshin
				tableSCORE[i][j] = 1000000;
				if (tableBOT[i][j] != ' ') //if not empty then add value to tableSCORE 
					tableSCORE[i][j]++;
		}
	}
	
	
	Scoring(board, 1, 2, BotDiff); //examining the best move for next step, the best step will be a minimal value


	for (i=0; i<board; i++) {
			for (j=0; j<board; j++) {
				if (tableSCORE[i][j] < min)
					min = tableSCORE[i][j]; //minimal value as best move
		}
	}
	
	while (1) { //check tableSCORE value randomly
		x = rand() % board;
		y = rand() % board;
		
		if (tableSCORE[x][y] == min) {
			table[x][y] = 'O';
			break;
		}
	}
		
}	

void Scoring(int board, int step, int turn, char BotDiff[10]) {
	
	
	int difficulty = 0;
	int i,j;
	int winner = 0;
	

	if (strcmp(BotDiff,"easy") == 0)
		difficulty = 1;
	else if (strcmp(BotDiff,"medium") == 0)
		difficulty = 2;
	else if (strcmp(BotDiff,"hard") == 0)
		difficulty = 3;
	
		
	for (i=0; i<board; i++) {
		for (j=0; j<board; j++) {
			if (tableBOT[i][j] == ' ') {
				tableBOT[i][j] = (turn == 1) ? 'X' : 'O';
				winner = CheckWin(board, tableBOT); //to check if the BOT wins in the next move
				if (winner == turn) { //if in the next turn bot is winning
					if (tableSCORE[i][j] == 1000000 || tableSCORE[i][j] > step) { //to check possible moves on the empty slot
						tableSCORE[i][j] = step; //giving minimum score to shadow table
					}
				}
				else if (step < difficulty + 1){ //the BOT predict moves x step ahead if the next turn the bot isn't winning
					if (turn == 1)
						Scoring (board, step+1, 2, BotDiff);
					else if (turn == 2)
						Scoring (board, step+1, 1,BotDiff);
				}
				tableBOT[i][j] = ' ';
			}
		}
	}
	return;
}

void MainMenu() {
	
	
	header();
	printf("\n\t\t\t     MAIN MENU\n\n");
	printf("Please select one of the availabe option below!\n");
	printf("Type 'help' if you need any hint about how to play this game\n");
	printf ("Type 'start' to play the game\n");
	printf("Type 'scoreboard' to show player history\n");
	printf("Type 'exit' to quit the game\n");
	printf("\nINPUT: ");
}

void EndGame(int PlayStatus, char Player1[100],char Player2[100]) {
	
	system ("cls");
	header();
		if (PlayStatus == 1)
			printf ("\n\n\t\t   CONGRATULATION %s, YOU WIN!", Player1);
		else if (PlayStatus == 2)
			printf ("\n\n\t\t   CONGRATULATION %s, YOU WIN!", Player2);
		else if (PlayStatus == 0)
			printf ("\n\n\t\t\t     IT'S A DRAW \n      \t\t\tGO FOR ANOTHER ROUND?'");

}

void footer () {
	
	printf ("\n________________________________________________________________ \n");
	printf ("  ____________________________________________________________\n");
}

void header () {
	
	printf ("  ____________________________________________________________\n");
	printf ("________________________________________________________________ \n\n");
	printf("---------------------------- LINE OF OX -------------------------\n");
}

void sortdata(){
	Data temp;
	int i;
	int j;
	for (i=0;i<10;i++){
		for (j=i;j<11;j++){
			if (BotHistory[j].win > BotHistory[i].win){
				strcpy(temp.name,BotHistory[i].name);
				temp.win = BotHistory[i].win;
				temp.lose = BotHistory[i].lose;
				temp.draw = BotHistory[i].draw;
				strcpy(BotHistory[i].name,BotHistory[j].name);
				BotHistory[i].win = BotHistory[j].win;
				BotHistory[i].lose = BotHistory[j].lose;
				BotHistory[i].draw = BotHistory[j].draw;
				strcpy(BotHistory[j].name,temp.name);
				BotHistory[j].win = temp.win;
				BotHistory[j].lose = temp.lose;
				BotHistory[j].draw = temp.draw;
			}
			if (PlayerHistory[j].win > PlayerHistory[i].win){
				strcpy(temp.name,PlayerHistory[i].name);
				temp.win = PlayerHistory[i].win;
				temp.lose = PlayerHistory[i].lose;
				temp.draw = PlayerHistory[i].draw;
				strcpy(PlayerHistory[i].name,PlayerHistory[j].name);
				PlayerHistory[i].win = PlayerHistory[j].win;
				PlayerHistory[i].lose = PlayerHistory[j].lose;
				PlayerHistory[i].draw = PlayerHistory[j].draw;
				strcpy(PlayerHistory[j].name,temp.name);
				PlayerHistory[j].win = temp.win;
				PlayerHistory[j].lose = temp.lose;
				PlayerHistory[j].draw = temp.draw;
			}
		}
	}
}

void scoreboard(){
	char select[10]={};
	int i;
	while (1){
		system("cls");
		printf("Select History Mode: BOT or Player\n");
		printf("INPUT: ");
		scanf("%s",select);
		for (i=0;select[i];i++){
			select[i] = tolower(select[i]);
		}
		if (strcmp(select,"bot") == 0 || strcmp(select,"player") == 0){
			break;
		}
		printf("\n\nPlease input suitable input!!!");
		printf("\n\nPress enter to continue!");
		while(!kbhit()){}
		system("cls");
	}
	if (strcmp(select,"bot") == 0){
		FILE *bot = fopen("scoreboard_bot.dat","r");
		if (bot == NULL){
			printf("No Data Record Found, Please play the game First");
			printf("\n\nPress enter to continue");
			while(!kbhit()){}
			return;
		}
		else {
			fread(BotHistory, sizeof (Data), 11, bot);
			system("cls");
			printf("TOP 10 VS BOT SCOREBOARD\n\n");
			printf("N\tNAME\t\t\t\t\tWIN\t\tLOSE\t\tDRAW");
			for (i=0;i<10;i++){
				printf("\n%d\t%-15s\t\t\t\t%d\t\t%d\t\t%d",i+1,BotHistory[i].name,BotHistory[i].win,BotHistory[i].lose,BotHistory[i].draw);
			}
		}
		fclose(bot);
	}
	else if (strcmp(select,"player") == 0){
		FILE *player = fopen("scoreboard_player.dat","r");
		if (player == NULL){
			printf("No Data Record Found, Please play the game First");
			printf("\n\nPress enter to continue");
			while(!kbhit()){}
			return;
		}
		else {
			fread(PlayerHistory, sizeof (Data), 11, player);
			system("cls");
			printf("TOP 10 VS PLAYER SCOREBOARD\n\n");
			printf("N\tNAME\t\t\t\t\tWIN\t\tLOSE\t\tDRAW");
			for (i=0;i<10;i++){
				printf("\n%d\t%-15s\t\t\t\t%d\t\t%d\t\t%d",i+1,PlayerHistory[i].name,PlayerHistory[i].win,PlayerHistory[i].lose,PlayerHistory[i].draw);
			}
		}
		fclose(player);
	}
	printf("\n\nPress enter to continue!");
	while(!kbhit()){}
}

void storedata(char player1[25],char player2[25],int result,int mode){
	int i;
	if (mode == 0){ //Player vs BOT
		FILE *bot = fopen("scoreboard_bot.dat","r");
		fread(BotHistory, sizeof (Data), 11 , bot);
		if (result == 1){ //if Player wins
			int slot1 = 1;
			int slot2 = 1;
			for (i=0;i<11;i++){
				if (strcmp(player1,BotHistory[i].name)==0){
					BotHistory[i].win++;
					slot1 = 0;
					break;
				}
				else if (strcmp(BotHistory[i].name,"-")==0){
					strcpy(BotHistory[i].name,player1);
					BotHistory[i].win++;
					slot1 = 0;
					break;
				}
			}
			for (i=0;i<11;i++){
				if (strcmp(player2,BotHistory[i].name)==0){
					BotHistory[i].lose++;
					slot2 = 0;
					break;
				}
				else if (strcmp(BotHistory[i].name,"-")==0){
					strcpy(BotHistory[i].name,player2);
					BotHistory[i].lose++;
					slot2 = 0;
					break;
				}
			}
			if (slot1 == 1){
				strcpy(BotHistory[11].name,player1);
				BotHistory[10].win = 1;
				BotHistory[10].lose = 0;
				BotHistory[10].draw = 0;
			}
			else if(slot2 == 1){
				strcpy(BotHistory[10].name,player2);
				BotHistory[10].win = 0;
				BotHistory[10].lose = 1;
				BotHistory[10].draw = 0;
			}
		}
		else if (result == 2){ //if BOT wins
			int slot1 = 1;
			int slot2 = 1;
			for (i=0;i<11;i++){
				if (strcmp(player1,BotHistory[i].name)==0){
					BotHistory[i].lose++;
					slot1 = 0;
					break;
				}
				else if (strcmp(BotHistory[i].name,"-")==0){
					strcpy(BotHistory[i].name,player1);
					BotHistory[i].lose++;
					slot1 = 0;
					break;
				}
			}
			for (i=0;i<11;i++){
				if (strcmp(player2,BotHistory[i].name)==0){
					BotHistory[i].win++;
					slot2 = 0;
					break;
				}
				else if (strcmp(BotHistory[i].name,"-")==0){
					strcpy(BotHistory[i].name,player2);
					BotHistory[i].win++;
					slot2 = 0;
					break;
				}
			}
			if (slot1 == 1){
				strcpy(BotHistory[11].name,player1);
				BotHistory[10].win = 0;
				BotHistory[10].lose = 1;
				BotHistory[10].draw = 0;
			}
			else if(slot2 == 1){
				strcpy(BotHistory[10].name,player2);
				BotHistory[10].win = 1;
				BotHistory[10].lose = 0;
				BotHistory[10].draw = 0;
			}
		}
		else if (result == 0){ //if Draw
			int slot1 = 1;
			int slot2 = 1;
			for (i=0;i<11;i++){
				if (strcmp(player1,BotHistory[i].name)==0){
					BotHistory[i].draw++;
					slot1 = 0;
					break;
				}
				else if (strcmp(BotHistory[i].name,"-")==0){
					strcpy(BotHistory[i].name,player1);
					BotHistory[i].draw++;
					slot1 = 0;
					break;
				}
			}
			for (i=0;i<11;i++){
				if (strcmp(player2,BotHistory[i].name)==0){
					BotHistory[i].draw++;
					slot2 = 0;
					break;
				}
				else if (strcmp(BotHistory[i].name,"-")==0){
					strcpy(BotHistory[i].name,player2);
					BotHistory[i].draw++;
					slot2 = 0;
					break;
				}
			}
			if (slot1 == 1){
				strcpy(BotHistory[11].name,player1);
				BotHistory[10].win = 0;
				BotHistory[10].lose = 0;
				BotHistory[10].draw = 1;
			}
			else if(slot2 == 1){
				strcpy(BotHistory[10].name,player2);
				BotHistory[10].win = 0;
				BotHistory[10].lose = 0;
				BotHistory[10].draw = 1;
			}
		}
		fclose(bot);
	}
	else if (mode == 1){ //Plyaer vs Player
		FILE *player = fopen("scoreboard_player.dat","r");
		fread(PlayerHistory, sizeof(Data), 11, player);
		if (result == 1){ //if Player 1 wins
			int slot1 = 1;
			int slot2 = 1;
			for (i=0;i<11;i++){
				if (strcmp(player1,PlayerHistory[i].name)==0){
					PlayerHistory[i].win++;
					slot1 = 0;
					break;
				}
				else if (strcmp(PlayerHistory[i].name,"-")==0){
					strcpy(PlayerHistory[i].name,player1);
					PlayerHistory[i].win++;
					slot1 = 0;
					break;
				}
			}
			for (i=0;i<11;i++){
				if (strcmp(player2,PlayerHistory[i].name)==0){
					PlayerHistory[i].lose++;
					slot2 = 0;
					break;
				}
				else if (strcmp(PlayerHistory[i].name,"-")==0){
					strcpy(PlayerHistory[i].name,player2);
					PlayerHistory[i].lose++;
					slot2 = 0;
					break;
				}
			}
			if (slot1 == 1){
				strcpy(PlayerHistory[10].name,player1);
				PlayerHistory[10].win = 1;
				PlayerHistory[10].lose = 0;
				PlayerHistory[10].draw = 0;
			}
			else if(slot2 == 1){
				strcpy(PlayerHistory[10].name,player2);
				PlayerHistory[10].win = 0;
				PlayerHistory[10].lose = 1;
				PlayerHistory[10].draw = 0;
			}
		}
		else if (result == 2){ //if Player 2 wins
			int slot1 = 1;
			int slot2 = 1;
			for (i=0;i<11;i++){
				if (strcmp(player2,PlayerHistory[i].name)==0){
					PlayerHistory[i].win++;
					slot2 = 0;
					break;
				}
				else if (strcmp(PlayerHistory[i].name,"-")==0){
					strcpy(PlayerHistory[i].name,player2);
					PlayerHistory[i].win++;
					slot2 = 0;
					break;
				}
			}
			for (i=0;i<11;i++){
				if (strcmp(player1,PlayerHistory[i].name)==0){
					PlayerHistory[i].lose++;
					slot1 = 0;
					break;
				}
				else if (strcmp(PlayerHistory[i].name,"-")==0){
					strcpy(PlayerHistory[i].name,player1);
					PlayerHistory[i].lose++;
					slot1 = 0;
					break;
				}
			}
			if (slot2 == 1){
				strcpy(PlayerHistory[10].name,player2);
				PlayerHistory[10].win = 1;
				PlayerHistory[10].lose = 0;
				PlayerHistory[10].draw = 0;
			}
			else if(slot1 == 1){
				strcpy(PlayerHistory[10].name,player1);
				PlayerHistory[10].win = 0;
				PlayerHistory[10].lose = 1;
				PlayerHistory[10].draw = 0;
			}
		}
		else if (result == 0){ //if Draw
			int slot1 = 1;
			int slot2 = 1;
			for (i=0;i<11;i++){ //insert player 1 name in the scoreboard
				if (strcmp(player1,PlayerHistory[i].name)==0){ // if player 1 is already registered in the scoreboard
					PlayerHistory[i].draw++;
					slot1 = 0;
					break;
				}
				else if (strcmp(PlayerHistory[i].name,"-")==0){ // if player isn't in the scoreboard
					strcpy(PlayerHistory[i].name,player1);
					PlayerHistory[i].draw++;
					slot1 = 0;
					break;
				}
			}
			for (i=0;i<11;i++){ //insert player 2 nae in the scoreboard
				if (strcmp(player2,PlayerHistory[i].name)==0){ // if player 2 is already registeredd in the scoreboard 
					PlayerHistory[i].draw++;
					slot2 = 0;
					break;
				}
				else if (strcmp(PlayerHistory[i].name,"-")==0){ // if player 2 isn't in the scoreboard
					strcpy(PlayerHistory[i].name,player2);
					PlayerHistory[i].draw++;
					slot2 = 0;
					break;
				}
			}
			if (slot1 == 1){
				strcpy(PlayerHistory[10].name,player1);
				PlayerHistory[10].win = 0;
				PlayerHistory[10].lose = 0;
				PlayerHistory[10].draw = 1;
			}
			else if(slot1 == 1){
				strcpy(PlayerHistory[10].name,player2);
				PlayerHistory[10].win = 0;
				PlayerHistory[10].lose = 0;
				PlayerHistory[10].draw = 1;
			}
		}
		fclose(player);
	}
}

void savedata(){
	FILE *bot = fopen("scoreboard_bot.dat","w");
	fwrite(BotHistory, sizeof (Data), 11, bot);
	fclose(bot);
	FILE *player = fopen("scoreboard_player.dat","w");
	fwrite(PlayerHistory, sizeof (Data), 11 , player);
	fclose(player);
}

void quickmath(char table[7][7], int turn, int board, int *pass){
	int a,b;
	int x,y;
	*pass = 0;
	int result;
	int operation;
	int answer;
	operation = rand() % 2;
	a = rand() % 50;
	b = rand() % 50;
	printf("Answer This Question to continue: \n");
	if (operation == 0){
		result = a + b;
		printf("%d + %d = \nAnswer:",a,b);
	}
	else if (operation == 1){
		result = a - b;
		printf("%d - %d = \nAnswer:",a,b);
		}
	fflush(stdin);
	int awal = clock();
	scanf("%d",&answer);
	int akhir = clock();
	int time = akhir - awal;
	if (answer == result && time < 15000){
		*pass = 1;
	}
	else if (answer != result && time < 15000) {
		printf("\nYour Answer is Wrong, Penalty Given!!!\n");
		do {
			x = 1 + (rand() % board);
			y = 1 + (rand() % board);
		} while (table[x-1][y-1] != ' ');
		table[x-1][y-1] = (turn == 1) ? 'X' : 'O';
		printf("Press enter key to continue!");
		while(!kbhit()){}
	}
	else if (time > 15000){
		printf("Time is up, Penalty Given!!!");
		do {
			x = 1 + (rand() % board);
			y = 1 + (rand() % board);
		} while (table[x-1][y-1] != ' ');
		table[x-1][y-1] = (turn == 1) ? 'X' : 'O';
		printf("Press enter key to continue!");
		while(!kbhit()){}
	}
}
	

