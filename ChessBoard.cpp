#include"Pieces.h"
#include"ChessBoard.h"
#include<iostream>
#include<cstring>
#include<cctype>

using namespace std;

ChessBoard::ChessBoard() {
  // Initialise an 8x8 board with all pointers set to nullptr indicating empty squares
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      piecesBoard[row][col] = nullptr;
    }
  }
}

void ChessBoard::clearBoard() {
  // Loop through each square of the board and delete pieces to prevent memory leaks
  // Set pointers to nullptr after deletion to mark the squares as empty
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (piecesBoard[row][col] != nullptr) {
        delete piecesBoard[row][col];
        piecesBoard[row][col] = nullptr;
      }
    }
  }
}

ChessBoard::~ChessBoard() {
  // Ensure all dynamic memory is cleaned up
  clearBoard(); 
}

bool ChessBoard::canCastle(const int direction, const Colour colour) const {
  return canCastleArray[colour == White ?
			// If white and positive direction = whiteKingSide
			(direction == 1 ? whiteKingSide : whiteQueenSide) :
			// If black and positive direction = blackKingSide
			(direction == 1 ? blackKingSide : blackQueenSide)];
}

const char * ChessBoard::getPosType(const int pos[2]) const {
   return piecesBoard[pos[0]][pos[1]]->getType();
}

Colour ChessBoard::getPosColour(const int pos[2]) const {
    return piecesBoard[pos[0]][pos[1]]->getColour();
}

bool ChessBoard::isPosEmpty(const int pos[2]) const {
  return (isInsideBoard(pos[0], pos[1]) && piecesBoard[pos[0]][pos[1]] == nullptr);
}

void ChessBoard::rowColToString(char * square, const int pos[2]) const {
  // Check if the position is within the board limits before proceeding
  if (!isInsideBoard(pos[0], pos[1])) {
    return;
  }
  // Position[0] is the row and position[1] is the column
  // Chess ranks are 1-8, starting from the bottom
  int rank = 8 - pos[0]; 

  // Convert the column to the appropriate file in chess notation
  char file = 'A' + pos[1]; 
    
  // Write the file and rank into the square string
  square[0] = file;
  // Convert the integer rank to its ASCII character
  square[1] = '0' + rank;
  // Null-terminate the string
  square[2] = '\0'; 
}

void ChessBoard::movePiece(const int sourcePos[2], const int destinationPos[2]) {
   // Check if destination square is valid and handle capture if there's an opponent's piece
  if (isInsideBoard(destinationPos[0], destinationPos[1]) && !isPosEmpty(destinationPos)) {
    cout << " taking " << piecesBoard[destinationPos[0]][destinationPos[1]]->getColourString()
	 << "'s " << piecesBoard[destinationPos[0]][destinationPos[1]]->getType();
    delete piecesBoard[destinationPos[0]][destinationPos[1]]; // Delete the captured piece
  }
  // Move the piece pointer from the source to the destination square  
  piecesBoard[destinationPos[0]][destinationPos[1]] = piecesBoard[sourcePos[0]][sourcePos[1]];
  // Set the source square pointer to nullptr to indicate it's now empty
  piecesBoard[sourcePos[0]][sourcePos[1]] = nullptr;
}

void ChessBoard::loadState(const char * fen) {
  // Load a new board state from a FEN string representation
  cout << "A new board state is loaded!";
  // Clear the current board state to ensure no residual pieces
  clearBoard(); 
  boardToArray(fen);
  isGameOver = false;
  
  // Check if game over on load
  checkGameOver();
}

void ChessBoard::boardToArray(const char * fen) {
  int row = 0, col = 0;
  // Index to iterate through the FEN string
  int i = 0; 

  // Process each character in the FEN string
  while (fen[i] != ' ' && fen[i] != '\0') {
    if (fen[i] == '/') {
      // Move to the next row and reset column to the beginning
      row++;
      col = 0;
    } else if (isdigit(fen[i])) {
        // Handle consecutive empty squares
        int emptySquares = fen[i] - '0';
        for (int j = 0; j < emptySquares; j++) {
	  // No piece in the square
          piecesBoard[row][col++] = nullptr; 
        }
    } else {
        // Create the piece object directly and place it on the board
        Colour colour = isupper(fen[i]) ? White : Black; // If uppercase, set to White, else Black
        piecesBoard[row][col] = PieceFactory::createPiece(tolower(fen[i]), colour, this);
        col++;
      }
    i++;
  }

  // Skip the space after the board part of FEN
  i++;

  // Set the colour for the next player to move
  if (fen[i] != '\0') {
    this->colour = (fen[i] == 'w') ? White : Black;
  }

  i += 2; // Skip the "w"/"b" and white space

  // Ensure castling availability to false
  for (int k = 0; k < 4; k++) {
    canCastleArray[k] = false;
  }

  // Now process the castling availability
  if (fen[i] != ' ') {
    // If there is no castling available, skip the loop
    if (fen[i] == '-') {
      i++;
    } else {
      // Otherwise, process the characters for castling availability
      while (fen[i] != ' ' && fen[i] != '\0') {
	switch (fen[i]) {
        case 'K': canCastleArray[0] = true; break; // White kingside
        case 'Q': canCastleArray[1] = true; break; // White queenside
        case 'k': canCastleArray[2] = true; break; // Black kingside
        case 'q': canCastleArray[3] = true; break; // Black queenside
        default: break; // Other characters are not relevant
	}
	i++;
      }
    }
  }  
}

void ChessBoard::convertToRowCol(const char* square, int position[2]) const {
  if (square == nullptr || strlen(square) != 2) {
     // Handle invalid input by setting indices to -1
     position[0] = -1;
     position[1] = -1;
     return;
  }

  // Column: Convert 'A'-'H' to 0-7
  position[1] = toupper(square[0]) - 'A';

  // Row: Convert '1'-'8' to 7-0
  position[0] = '8' - square[1];

  // Validate the position is within the bounds of the board
  if (position[0] < 0 || position[0] > 7 || position[1] < 0 || position[1] > 7) {
     // Handle invalid board position
     position[0] = -1;
     position[1] = -1;
  }
}

const char * ChessBoard::getColourString() const {
  return (colour == Black) ? "Black" : "White";
}

bool ChessBoard::isKingInCheck(const Colour kingColour) {
  // Find king position
  int kingPos[2];
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      int tempPos[2] = {x, y};
      if (!isPosEmpty(tempPos) && getPosColour(tempPos) == kingColour && 
	  strcmp(piecesBoard[x][y]->getType(), "King") == 0) {
        kingPos[0] = x;
        kingPos[1] = y;
	break;
      }
    }
  }

  // Expected to be overwritten
  int currentPos[2] = {-1, -1};
    
  // Check if any opposing piece can move to the king's position
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (isInsideBoard(x, y)) {
	int tempPos[2] = {x, y};
        // If there is an opposing piece
        if (!isPosEmpty(tempPos) && getPosColour(tempPos) != kingColour) {
          currentPos[0] = x;
          currentPos[1] = y;
          // If opposing piece can move to the King's position, they are in check
          if (piecesBoard[x][y]->isValidMove(currentPos, kingPos)) {
	    // King is in check
	    return true; 
          }
        }
      }
    }
  }
  // King is not in check
  return false; 
}

void ChessBoard::setCastleArray(const int index, const bool value) {
  if (index >= 0 && index < 4) {
    canCastleArray[index] = value;
  }
}

bool ChessBoard::canEscapeCheck(const Colour kingColour) {
  // Iterate through all pieces of the given colour
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      Pieces* piece = piecesBoard[x][y];
      
      // If there is a piece on the same team/colour
      if (piece != nullptr && piece->getColour() == kingColour) {
        // Check all possible moves for this piece
        for (int destX = 0; destX < 8; destX++) {
          for (int destY = 0; destY < 8; destY++) {
            int sourcePos[2] = {x, y};
            int destPos[2] = {destX, destY};
            // For all possible positions check if it is a valid move
	    if (piece->isValidMove(sourcePos, destPos)) {
              // Simulate the move
	      Pieces* temp = piecesBoard[destX][destY];
              piecesBoard[destX][destY] = piece;
              piecesBoard[x][y] = nullptr;
                            
              // Check if the king is still in check after simulating the move
              bool stillInCheck = isKingInCheck(kingColour);

	      // Revert the move
              piecesBoard[x][y] = piece;
              piecesBoard[destX][destY] = temp;
                            
              if (!stillInCheck) {
		// Found a move that gets the king out of check or is a legal move
                return true;
              }
            }
          }
        }
      }
    }
  }
  // No move found to get out of check or no legal moves available
  return false; 
}

bool ChessBoard::doesMoveCauseCheck(const int sourcePos[2], int destinationPos[2], Colour colour) {
  if (!isInsideBoard(destinationPos[0], destinationPos[1]) && !isInsideBoard(sourcePos[0], sourcePos[1])) {
    // If not inside board, return true to ensure move not done
    return true;
  }
  Pieces* piece = piecesBoard[sourcePos[0]][sourcePos[1]];
  Pieces* capturedPiece = piecesBoard[destinationPos[0]][destinationPos[1]];

  // Simulate the move
  piecesBoard[destinationPos[0]][destinationPos[1]] = piece;
  piecesBoard[sourcePos[0]][sourcePos[1]] = nullptr;

  // Check if this move would put the player's king in check
  bool causesCheck = isKingInCheck(colour);

  // Revert the move
  piecesBoard[sourcePos[0]][sourcePos[1]] = piece;
  piecesBoard[destinationPos[0]][destinationPos[1]] = capturedPiece;

  return causesCheck;
}


bool ChessBoard::isInBounds(int * sourcePos, int * destinationPos) {
  // Check for out-of-range values
  // -1 is returned from ConvertRowToCol() for invalid squares e.g. "Q8"
  if (sourcePos[0] == -1 || sourcePos[1] == -1 || destinationPos[0] == -1 || destinationPos[1] == -1) {
    cout << "Not a valid input move." << endl;
    return false;
  }
  return true;
 }

bool ChessBoard::isValidStartPosition(Pieces* startPosition, const char* sourceSquare) {
  if (!startPosition) {
    cout << "There is no piece at position " << sourceSquare << "!";
    return false;
  }
  return true;
}

bool ChessBoard::isValidTurn(Pieces* startPosition) {
  if (colour != startPosition->getColour()) {
      cout << "\nIt is not " << startPosition->getColourString() << "'s turn to move!" << endl;
      return false;
  }
  return true;
}

bool ChessBoard::isValidMove(Pieces* startPosition, int sourcePos[], int destinationPos[], const char* destinationSquare) {
  // Uses overriden isValidMove() depending on the subclass of Piece e.g. Pawn
  if (!startPosition->isValidMove(sourcePos, destinationPos)) {
    cout << "\n" << startPosition->getColourString() << "'s " << startPosition->getType() << " cannot move to " << destinationSquare << "!" << endl;
    return false;
  }

  if (doesMoveCauseCheck(sourcePos, destinationPos, startPosition->getColour())) {
    cout << "That move would put you in check. Please try another move." << endl;
    return false;
  }
  
  return true;
}

void ChessBoard::executeMove(Pieces* startPosition, int sourcePos[], int destinationPos[]) {
    startPosition->makeMove(sourcePos, destinationPos);
    // Update the colour to go after executing the move
    this->colour = (colour == White) ? Black : White;
}

bool ChessBoard::checkGameOver() {
  if (!canEscapeCheck(colour)) {
    if (isKingInCheck(colour)) {
      cout << "\n" << getColourString() << " is in checkmate" << endl;
    } else {
      cout << "\nIt is a stalemate" << endl;
    }
    isGameOver = true;
    clearBoard();
    return true;
  }
  return false;
}

void ChessBoard::postMoveChecks() {
  const char* colourString = (colour == White) ? "White" : "Black";

  if (checkGameOver()) {
    // Skip print statement for in check
    return; 
  }

  if (isKingInCheck(colour)) {
    cout << "\n" << colourString << " is in check";
  }
}

void ChessBoard::submitMove(const char * sourceSquare, const char * destinationSquare) {
  // Prevent moves after end of game
  if (isGameOver){
    cout << "The Game is over" << endl;
    return;
  }
  
  // Convert strings to board positions and validate
  int sourcePos[2], destinationPos[2];
  convertToRowCol(sourceSquare, sourcePos);
  convertToRowCol(destinationSquare, destinationPos);

  // Ensure within bounds
  if (!isInBounds(sourcePos, destinationPos)) {
    return; 
  }

  // Ensures there is a piece at the starting position
  Pieces* startPosition = piecesBoard[sourcePos[0]][sourcePos[1]];
  if (!isValidStartPosition(startPosition, sourceSquare)) {
    return; 
  }

  // Check who's turn it is (Black/White)
  if (!isValidTurn(startPosition)) {
    return; 
  }

  // Validate the move, this calls an overriden function specific to the piece type
  if (!isValidMove(startPosition, sourcePos, destinationPos, destinationSquare)) {
    return; 
  }

  // Make the move and update the colour to go
  executeMove(startPosition, sourcePos, destinationPos);

  // Post-move checks (checkmate, stalemate, check)
  postMoveChecks();
}
  
