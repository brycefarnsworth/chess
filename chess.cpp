#include <iostream>
#include <string>
using namespace std;

char board[8][8];

char setup[8][8] = { {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
				     {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
				     {'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X'},
				     {'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O'},
			   	     {'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X'},
				     {'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O'},
				     {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
				     {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
				   };

string color[2] = { "White", "Black" };

int king[2][2];

int player;

bool kingMoved[2];

bool rookAMoved[2];

bool rookHMoved[2];

bool gameover;

bool isLegalCastle(int b[]);

void setboard() {

	for( int i = 0; i < 8; i++ ) {
		for( int j = 0; j < 8; j++ ) {

			board[i][j] = setup[i][j];

		}
	}

	king[0][0] = 7;
	king[0][1] = 4;
	king[1][0] = 0;
	king[1][1] = 4;

	player = 0;

	kingMoved[0] = false;
	kingMoved[1] = false;

	rookAMoved[0] = false;
	rookAMoved[1] = false;

	rookHMoved[0] = false;
	rookHMoved[1] = false;

	gameover = false;

}

void printboard() {

	if( !player )
		cout << "White's Turn" << endl;
	else
		cout << "Black's Turn" << endl;
	for( int i = 0; i < 8; i++ ) {
		cout << 8 - i << " ";
		for( int j = 0; j < 8; j++ ) {
			cout << "[ " << board[i][j] << " ]";
		}
		cout << endl;
	}

	cout << "    a    b    c    d    e    f    g    h" << endl << endl;

}

bool isOccupied(int row, int col) {

	return ( board[row][col] != 'X' && board[row][col] != 'O' ) ;

}

bool isLegalKing(int a[], int b[]) {
	
	if( (abs(b[0] - a[0]) <= 1 && abs(b[1] - a[1]) <= 1) ||
		isLegalCastle(b) ) {
	
		return true;

	}
	return false;

}

bool isLegalRook(int a[], int b[]) {

	if( b[0] == a[0] ) {

		for( int i = ((b[1]-a[1])/abs(b[1]-a[1]));
			 abs(i) < abs(b[1]-a[1]);
			 i = i + ((b[1]-a[1])/abs(b[1]-a[1])) ) {
				 
				 /* cout << "i = " << i << endl;
				 cout << "abs(b[1]-a[1]) = " << abs(b[1]-a[1]) << endl;
				 cout << "Checking " << char(a[1]+i+'a') << char((8-a[0])+'0') << endl; */
				 if( isOccupied(a[0], a[1]+i) ) {
					 // cout << char(a[1]+i+'a') << char((8-a[0])+'0') << " is occupied." << endl;
					 return false;
				 }

		}
		return true;
	
	}
	else if( b[1] == a[1] ) {

		for( int i = ((b[0]-a[0])/abs(b[0]-a[0]));
			 abs(i) < abs(b[0]-a[0]);
			 i = i + ((b[0]-a[0])/abs(b[0]-a[0])) ) {
				 
				 /* cout << "i = " << i << endl;
				 cout << "abs(b[0]-a[0]) = " << abs(b[0]-a[0]) << endl;
				 cout << "Checking " << char(a[1]+'a') << char((8-(a[0]+i))+'0') << endl; */
				 if( isOccupied(a[0]+i, a[1]) ) {
					 // cout << char(a[1]+'a') << char((8-(a[0]+i))+'0') << " is occupied." << endl;
					 return false;
				 }

		}
		return true;
	
	}
	return false;

}

bool isLegalBishop(int a[], int b[]) {

	int diff = abs(b[0] - a[0]);
	if( abs(b[1] - a[1]) == diff ) {
		
		for( int i = (b[0]-a[0])/diff, j = (b[1]-a[1])/diff;
			 abs(i) < diff && abs(j) < diff;
			 i = i + (b[0]-a[0])/diff, j = j + (b[1]-a[1])/diff ) {

				 // cout << "Checking " << char(a[1]+j+'a') << char((8-(a[0]+i))+'0') << endl;
				 if( isOccupied( a[0]+i, a[1]+j ) ) {
					 // cout << char(a[1]+j+'a') << char((8-(a[0]+i))+'0') << " is occupied." << endl;
					 return false;
				 }

		}
		return true;

	}
	return false;

}

bool isLegalQueen(int a[], int b[]) {

	return ( isLegalRook(a, b) || isLegalBishop(a, b) ) ;

}

bool isLegalKnight(int a[], int b[]) {

	return ( ( abs(b[0] - a[0]) == 2 && abs(b[1] - a[1]) == 1 ) ||
			 ( abs(b[0] - a[0]) == 1 && abs(b[1] - a[1]) == 2 ) ) ;

}

bool isLegalWhitePawn(int a[], int b[]) {

	return ( ( b[0] == a[0] - 1 && b[1] == a[1] && !isOccupied(b[0], b[1]) ) ||
			 ( a[0] == 6 && b[0] == a[0] - 2 && b[1] == a[1] && 
			   !isOccupied(b[0], b[1]) && !isOccupied(b[0]+1, b[1]) ) ||
			 ( b[0] == a[0] - 1 && abs(b[1] - a[1]) == 1 && 
			 ( board[b[0]][b[1]] == 'p' || board[b[0]][b[1]] == 'n' ||
			   board[b[0]][b[1]] == 'b' || board[b[0]][b[1]] == 'r' ||
			   board[b[0]][b[1]] == 'q' ) ) ) ;

}

bool isLegalBlackPawn(int a[], int b[]) {

	return ( ( b[0] == a[0] + 1 && b[1] == a[1] && !isOccupied(b[0], b[1]) ) ||
			 ( a[0] == 1 && b[0] == a[0] + 2 && b[1] == a[1] && 
			 !isOccupied(b[0], b[1]) && !isOccupied(b[0]-1, b[1]) ) ||
			 ( b[0] == a[0] + 1 && abs(b[1] - a[1]) == 1 && 
			 ( board[b[0]][b[1]] == 'P' || board[b[0]][b[1]] == 'N' ||
			   board[b[0]][b[1]] == 'B' || board[b[0]][b[1]] == 'R' ||
			   board[b[0]][b[1]] == 'Q' ) ) ) ;

}

bool check(int p) {

	int a[2];

	if( !p ) { // Check if white is in check

		for( int i = 0; i < 8; i++ ) {
			for( int j = 0; j < 8; j++ ) {

				a[0] = i;
				a[1] = j;
				switch( board[i][j] ) {

				case 'k':
					if( isLegalKing(a, king[p]) ) 
						return true;
					break;
				case 'q':
					if( isLegalQueen(a, king[p]) ) 
						return true;
					break;
				case 'r':
					if( isLegalRook(a, king[p]) ) 
						return true;
					break;
				case 'b':
					if( isLegalBishop(a, king[p]) ) 
						return true;
					break;
				case 'n':
					if( isLegalKnight(a, king[p]) ) 
						return true;
					break;
				case 'p':
					if( king[p][0] == a[0] + 1 &&
						(king[p][1] == a[1] + 1 ||
						 king[p][1] == a[1] - 1) )
						 return true;
					break;
				}

			}
		}
		return false;

	}

	else { // Check if black is in check

		for( int i = 0; i < 7; i++ ) {
			for( int j = 0; j < 7; j++ ) {

				a[0] = i;
				a[1] = j;
				switch( board[i][j] ) {

				case 'K':
					if( isLegalKing(a, king[p]) )
						return true;
					break;
				case 'Q':
					if( isLegalQueen(a, king[p]) ) 
						return true;
					break;
				case 'R':
					if( isLegalRook(a, king[p]) ) 
						return true;
					break;
				case 'B':
					if( isLegalBishop(a, king[p]) ) 
						return true;
					break;
				case 'N':
					if( isLegalKnight(a, king[p]) ) 
						return true;
					break;
				case 'P':
					if( king[p][0] == a[0] - 1 &&
						(king[p][1] == a[1] + 1 ||
						 king[p][1] == a[1] - 1) )
						 return true;
					break;

				}

			}
		}

		return false;

	}

}

bool movesIntoCheck(int a[], int b[]) {

	char tmp = board[b[0]][b[1]];
	
	if( board[a[0]][a[1]] == 'K' || board[a[0]][a[1]] == 'k' ) {

		king[player][0] = b[0];
		king[player][1] = b[1];

	}

	board[b[0]][b[1]] = board[a[0]][a[1]];

	if( (a[0] + a[1]) % 2 == 0 )
		board[a[0]][a[1]] = 'O';
	else
		board[a[0]][a[1]] = 'X';

	bool inCheck = check(player);

	board[a[0]][a[1]] = board[b[0]][b[1]];
	board[b[0]][b[1]] = tmp;

	if( board[a[0]][a[1]] == 'K' || board[a[0]][a[1]] == 'k' ) {

		king[player][0] = a[0];
		king[player][1] = a[1];

	}

	return inCheck;

}

bool isLegalCastle(int b[]) {

	if( !player ) { // White Castle

		if( !kingMoved[player] ) {
			
			int kingCopy[2] = {king[player][0], king[player][1]};

			if( b[0] == 7 && b[1] == 6 ) { // Kingside Castle

				int square1[2] = {7,5};
				int square2[2] = {7,6};

				return ( !rookHMoved[player] && !isOccupied(7,5) && !isOccupied(7,6) &&
						 !check(player) && !movesIntoCheck(kingCopy, square1) && 
						 !movesIntoCheck(kingCopy, square2) );

			}

			else if( b[0] == 7 && b[1] == 2 ) { // Queenside Castle

				int square1[2] = {7,3};
				int square2[2] = {7,2};
				int square3[2] = {7,1};

				return ( !rookAMoved[player] && !isOccupied(7,3) && !isOccupied(7,2) &&
						 !isOccupied(7,1) && !check(player) && !movesIntoCheck(kingCopy, square1) && 
						 !movesIntoCheck(kingCopy, square2) );

			}

		}

		return false;

	}

	else { // Black Castle

		if( !kingMoved[player] ) {

			int kingCopy[2] = {king[player][0], king[player][1]};
			
			if( b[0] == 0 && b[1] == 6 ) { // Kingside Castle

				int square1[2] = {0,5};
				int square2[2] = {0,6};

				return ( !rookHMoved[player] && !isOccupied(0,5) && !isOccupied(0,6) &&
						 !check(player) && !movesIntoCheck(king[player], square1) && 
						 !movesIntoCheck(king[player], square2) );

			}

			else if( b[0] == 0 && b[1] == 2 ) { // Queenside Castle

				int square1[2] = {0,3};
				int square2[2] = {0,2};
				int square3[2] = {0,1};

				return ( !rookAMoved[player] && !isOccupied(0,3) && !isOccupied(0,2) &&
						 !isOccupied(0,1) && !check(player) && !movesIntoCheck(king[player], square1) && 
						 !movesIntoCheck(king[player], square2) );

			}

		}

		return false;

	}

}

bool isLegal(int a[], int b[]) {

	if( ( b[0] == a[0] && b[1] == a[1] ) ||					// Did not move
		b[0] < 0 || b[0] > 7 || b[1] < 0 || b[1] > 7 ||		// Out of bounds
		a[0] < 0 || a[1] > 7 || a[1] < 0 || a[1] > 7 )
		return false;

	if( !player && ( board[b[0]][b[1]] == 'K' || board[b[0]][b[1]] == 'Q' ||		// White moves on a white piece
		board[b[0]][b[1]] == 'R' || board[b[0]][b[1]] == 'B' ||
		board[b[0]][b[1]] == 'N' || board[b[0]][b[1]] == 'P' ) )
		return false;
	else if( player && ( board[b[0]][b[1]] == 'k' || board[b[0]][b[1]] == 'q' ||	// Black moves on a black piece
		board[b[0]][b[1]] == 'r' || board[b[0]][b[1]] == 'b' ||
		board[b[0]][b[1]] == 'n' || board[b[0]][b[1]] == 'p' ) )
		return false;

	if( movesIntoCheck(a, b) )
		return false;

	if( !player ) {

		switch( board[a[0]][a[1]] ) {

		case 'K':
			return isLegalKing(a, b);
			break;
		case 'Q':
			return isLegalQueen(a, b);
			break;
		case 'R':
			return isLegalRook(a, b);
			break;
		case 'B':
			return isLegalBishop(a, b);
			break;
		case 'N':
			return isLegalKnight(a, b);
			break;
		case 'P':
			return isLegalWhitePawn(a, b);
			break;
		default:
			return false;
			break;

		}

	}

	else {

		switch( board[a[0]][a[1]] ) {

		case 'k':
			return isLegalKing(a, b);
			break;
		case 'q':
			return isLegalQueen(a, b);
			break;
		case 'r':
			return isLegalRook(a, b);
			break;
		case 'b':
			return isLegalBishop(a, b);
			break;
		case 'n':
			return isLegalKnight(a, b);
			break;
		case 'p':
			return isLegalBlackPawn(a, b);
			break;
		default:
			return false;
			break;

		}

	}

}

bool checkmate(int p) {

	int a[2];
	int b[2];

	if( !p ) {

		for( int i = 0; i < 8; i++ ) {
			for( int j = 0; j < 8; j++ ) {

				if( board[i][j] == 'K' || board[i][j] == 'Q' ||
					board[i][j] == 'R' || board[i][j] == 'B' ||
					board[i][j] == 'N' || board[i][j] == 'P' ) {

						for( int m = 0; m < 8; m++ ) {
							for( int n = 0; n < 8; n++ ) {

								a[0] = i;
								a[1] = j;
								b[0] = m;
								b[1] = n;
								
								if( isLegal(a, b) )
									return false;

							}
						}

				}

			}
		}

		return true;

	}

	else {

		for( int i = 0; i < 8; i++ ) {
			for( int j = 0; j < 8; j++ ) {

				if( board[i][j] == 'k' || board[i][j] == 'q' ||
					board[i][j] == 'r' || board[i][j] == 'b' ||
					board[i][j] == 'n' || board[i][j] == 'p' ) {

						for( int m = 0; m < 8; m++ ) {
							for( int n = 0; n < 8; n++ ) {

								a[0] = i;
								a[1] = j;
								b[0] = m;
								b[1] = n;
								
								if( isLegal(a, b) )
									return false;

							}
						}

				}

			}
		}

		return true;

	}

}

void move(char ab[]) {

	int acol = int(ab[0]) - 'a';
	int arow = 8 - (int(ab[1]) - '0');
	int bcol = int(ab[2]) - 'a';
	int brow = 8 - (int(ab[3]) - '0');

	int a[] = {arow, acol};
	int b[] = {brow, bcol};

	if( isLegal(a, b) ) {

		if( arow == king[player][0] && acol == king[player][1] ) {

			if( isLegalCastle(b) ) {

				if( bcol == 6 ) {

					char tmp = board[brow][5];
					board[brow][5] = board[brow][7];
					board[brow][7] = tmp;
					rookHMoved[player] = true;

				}

				else if( bcol == 0 ) {

					char tmp = board[brow][2];
					board[brow][3] = board[brow][0];
					board[brow][0] = tmp;
					rookAMoved[player] = true;

				}

			}
			king[player][0] = brow;
			king[player][1] = bcol;
			kingMoved[player] = true;

		}

		if( acol == 0 && (arow == 0 || arow == 7) )
			rookAMoved[player] = true;
		if( acol == 7 && (arow == 0 || arow == 7) )
			rookHMoved[player] = true;
		
		board[brow][bcol] = board[arow][acol];

		if( (arow + acol) % 2 == 0 )
			board[arow][acol] = 'O';
		else
			board[arow][acol] = 'X';
	
		player = !player;

		if( check(player) ) {

			if( checkmate(player) ) {

				cout << "Checkmate. " << color[!player] << " wins." << endl;
				gameover = true;

			}
			else
				cout << "Check" << endl;

		}

	}

	else cout << "Illegal move" << endl;

}

int main() {
	
	setboard();
	printboard();
	char m[4];
	cin >> m[0];
	while( m[0] != 'q' && !gameover ) {
		cin >> m[1] >> m[2] >> m[3];
		move(m);
		printboard();
		cin >> m[0];
	}

	return 0;

}