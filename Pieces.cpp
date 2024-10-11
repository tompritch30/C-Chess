#include"Pieces.h"
#include"ChessBoard.h"
#include<iostream>
#include<cstring>
#include<cctype>

// Forward declare chessboard for Piece factory
class ChessBoard;

Pieces* PieceFactory::createPiece(char c, Colour colour, ChessBoard* board) {
  // Return nullptr if character is not alphabetic, indicating an invalid piece type.
  if (!isalpha(c)) {
    return nullptr; 
  }
  // Normalise character to lowercase to simplify switch-case structure.
  c = tolower(c); 

  // Switch-case to create a chess piece based on the character input.
  switch (c) {
    case 'p': return new Pawn(colour, board);
    case 'k': return new King(colour, board);
    case 'r': return new Rook(colour, board);
    case 'b': return new Bishop(colour, board);
    case 'n': return new Knight(colour, board);
    case 'q': return new Queen(colour, board);
    default : return nullptr; // Return nullptr for unsupported piece types.
  }
}

const char * Pieces::getColourString() const {
  // Returns the string representation of the piece's colour.
  return pieceColour == White ? "White" : "Black";
}

void Pieces::calcDyDx(const int sourcePos[2], const int destinationPos[2], int dyDxArray[2]) const {
   // Calculate the change in x (dx) and y (dy) from source to destination.
  dyDxArray[0] = destinationPos[0] - sourcePos[0]; // dy
  dyDxArray[1] = destinationPos[1] - sourcePos[1]; // dx
}

void Pieces::makeMove(const int sourcePos[2], const int destinationPos[2]){
  // Ensure is a valid move
  if (sourcePos && destinationPos && !board->isInsideBoard(destinationPos[0], destinationPos[1]) && !board->isInsideBoard(sourcePos[0], sourcePos[1])) {
    return;
  }
  // Common move logic
  char sourceSquare[3] = {'\0'};
  char destinationSquare[3] = {'\0'};;
  board->rowColToString(sourceSquare, sourcePos);
  board->rowColToString(destinationSquare, destinationPos);
 
  std::cout << "\n" << this->getColourString() << "'s " << this->getType() << " moves from " << sourceSquare << " to " << destinationSquare;

  // If we are capturing a piece, print statement and delete piece
  board->movePiece(sourcePos, destinationPos);
  
  // Update hasMoved for the moving piece
  this->updateMoved();

  // Call helper method for castling rights, is overridden in King and Rook
  updateCastlingRights(sourcePos, destinationPos);
}

bool Pieces::destinationSameColour(const int destinationPos[2]) const {
  // Check if attempting to check Colour of a Piece outside board bounds.
  if (!board->isInsideBoard(destinationPos[0], destinationPos[1])) {
    return false;
  }
  // If the position is not empty
  if (!board->isPosEmpty(destinationPos)){
    // If colour is the same return true
    return (board->getPosColour(destinationPos) == this->pieceColour);
  }
  // No piece at the destination
  return false; 
}


void King::updateCastlingRights(const int sourcePos[2], const int destinationPos[2]) {
  Colour colour = this->pieceColour;
  // Updates the castle array at the index
  board->setCastleArray(colour == White ? whiteKingSide : blackKingSide, false);
  board->setCastleArray(colour == White ? whiteQueenSide : blackQueenSide, false);

  // Check for castling and move the rook if castling occurs
  int dyDxArray[2];
  // Modifies the dyDxArray
  calcDyDx(sourcePos, destinationPos, dyDxArray);
  int dy = dyDxArray[0];
  int dx = dyDxArray[1];

  // If the King is castling
  if (abs(dx) == 2 && dy == 0) {
    // Positive dx shows kingside castling, and rook should be in col 7.
    int rookCol = dx == 2 ? 7 : 0;
    int newRookCol = dx == 2 ? 5 : 3;
     
    // Get the rook's current position in integer array form
    int rookSourcePos[2] = {sourcePos[0], rookCol};
    int rookDestinationPos[2] = {sourcePos[0], newRookCol};
	
    // To handle mid-game FEN positions
    if (!board->isPosEmpty(rookSourcePos) && colour == board->getPosColour(rookSourcePos)){
      // Now make the move for the rook, bypassing the hasMoved and updateCastleRights as castling occured.
      board->movePiece(rookSourcePos, rookDestinationPos);
    }
  }
}

void Rook::updateCastlingRights(const int sourcePos[2], const int destinationPos[2]) {
  // Check for castling and move the rook if castling occurs.
  if (this->pieceColour == White) {
    // White queen side rook
    if (sourcePos[1] == 0) {
      board->setCastleArray(whiteQueenSide, false);
    }
    else { // White king side rook
      board->setCastleArray(whiteKingSide, false);
    }
  }
  else {
    // Black queen side rook
    if (sourcePos[1] == 0) {
      board->setCastleArray(blackQueenSide, false);
    }
    else { // Black king side rook
      board->setCastleArray(blackKingSide, false);
    }
  }
}

// Pawn specific rules applied for move validity
bool Pawn::isValidMove(const int sourcePos[2], const int destinationPos[2]) const {
  if (destinationSameColour(destinationPos)) {
      return false;
  }
  
  int dyDxArray[2];
  // Modifies the dyDxArray
  calcDyDx(sourcePos, destinationPos, dyDxArray);
  int dy = dyDxArray[0];
  int dx = dyDxArray[1];

  // White pawns move from higher row indexes to lower.
  int direction = (pieceColour == White) ? -1 : 1;
  // If on starting row to allow loading of all FEN strings
  bool isStartingRow = (pieceColour == White) ? sourcePos[0] == 6 : sourcePos[0] == 1;
  
  // Double move, if it hasn't moved, is in the starting row and the path is clear
  if (isStartingRow && !this->hasMoved && dy == 2 * direction && dx == 0 && isPathClearStraight(sourcePos, destinationPos) && board->isPosEmpty(destinationPos)) {
    return true;
  } // Single move forward
  else if (dy == 1 * direction && dx == 0 && board->isPosEmpty(destinationPos)) {
    return true;
  } // Diagonal capture (one square diagonal has opposing piece)
  else if (dy == 1 * direction && abs(dx) == 1 && !board->isPosEmpty(destinationPos) && 
	   board->getPosColour(destinationPos) != this->pieceColour) {
    return true;
  }
  // Invalid move
  return false;
}

bool King::isValidMove(const int sourcePos[2], const int destinationPos[2]) const {
  if (destinationSameColour(destinationPos)) {
    return false;
  }
  int dyDxArray[2];
  // Modifies the dyDxArray
  calcDyDx(sourcePos, destinationPos, dyDxArray);
  int dy = dyDxArray[0];
  int dx = dyDxArray[1];

  // King can move one square in any direction
  if ((abs(dy) == 1 && abs(dx) <= 1) || (abs(dx) == 1 && abs(dy) <= 1)) {
    return true;
  }

  // Castling Logic 
  // If king is in the correct starting position
  int blackKingStartPos[2] = {0, 4};
  int whiteKingStartPos[2] = {7, 4};
  if (this->pieceColour == White) {
    if (sourcePos[0] != whiteKingStartPos[0] || sourcePos[1] != whiteKingStartPos[1]) {
      return false;
    }
  }
  else {
    if (sourcePos[0] != blackKingStartPos[0] || sourcePos[1] != blackKingStartPos[1]) {
      return false;
    }
  }
  
  // If hasnt moved, a lateral move 2 and not in check
  if (!this->hasMoved && dy == 0 && abs(dx) == 2 && !board->isKingInCheck(this->pieceColour)) {
    // King or Queen side castle
    int rookColumn = dx == 2 ? 7 : 0; 
    int rookPosition[2] = {sourcePos[0], rookColumn};

    // If not inside board, the position is empty, not a rook of the same colour
    if (!board->isInsideBoard(rookPosition[0], rookPosition[1]) || board->isPosEmpty(rookPosition) ||  board->getPosColour(rookPosition) != this->getColour() || 
    strcmp(board->getPosType(rookPosition), "Rook") != 0) {
      return false;
    }
    
    int direction = (rookColumn == 7) ? 1 : -1; 

    // Check the castling array 
    if (board->canCastle(direction, this->getColour() )) {
      int pathStart[2] = {sourcePos[0], sourcePos[1]};
      int pathEnd[2] = {sourcePos[0], rookColumn};

      // The path clear from pathStart to Rook position (not inclusive)
      if (!this->isPathClearStraight(pathStart, pathEnd)) {
	return false; 
      }

      // Check for no checks on passing squares
      for (int i = 1; i <= abs(dx); ++i) {
	int destinationPos[2] = {sourcePos[0], sourcePos[1] + i * direction};
	if (board->doesMoveCauseCheck(sourcePos, destinationPos, this->getColour())) {
	  // King passes through or lands on a square that is under attack
	  return false;
        }
      }
      // All castling conditions met
      return true;
    }
    // Castling conditions not met
    return false;
  }
  // Invalid King Move
  return false;
}

bool Rook::isValidMove(const int sourcePos[2], const int destinationPos[2]) const {
  if (destinationSameColour(destinationPos)) {
    return false;
   }
  // Rook moves in straight lines
  if (sourcePos[0] == destinationPos[0] || sourcePos[1] == destinationPos[1]) {
    // Returns true is the path is clear
    return isPathClearStraight(sourcePos, destinationPos);
  }
  // If not a straight line
  return false;
}

bool Bishop::isValidMove(const int sourcePos[2], const int destinationPos[2]) const {
  if (destinationSameColour(destinationPos)) {
    return false;
  }
  int dyDxArray[2];
  // Modifies the dyDxArray
  calcDyDx(sourcePos, destinationPos, dyDxArray);
  int dy = dyDxArray[0];
  int dx = dyDxArray[1];

  // int dx = destinationPos[1] - sourcePos[1];
  // Bishop moves diagonally
  if (abs(dy) == abs(dx)) {
    // Returns true is the path is clear
    return isPathClearDiagonal(sourcePos, destinationPos);
  }
  // If not a straight line
  return false;
}

bool Knight::isValidMove(const int sourcePos[2], const int destinationPos[2]) const {  
  if (destinationSameColour(destinationPos)) {
    return false;
  }
  int dyDxArray[2];
  // Modifies the dyDxArray
  calcDyDx(sourcePos, destinationPos, dyDxArray);
  // absolute dy dx for different directions
  int dy = abs(dyDxArray[0]);
  int dx = abs(dyDxArray[1]);
  // Knight moves in an L shape
  return (dy == 2 && dx == 1) || (dy == 1 && dx == 2);
}
bool Queen::isValidMove(const int sourcePos[2], const int destinationPos[2]) const {
  if (destinationSameColour(destinationPos)) {
    return false;
  }

  int dyDxArray[2];
  // Modifies the dyDxArray
  calcDyDx(sourcePos, destinationPos, dyDxArray);
  int dy = dyDxArray[0];
  int dx = dyDxArray[1];

  // If path is a straight, diagonal and clear
  bool diagonalGood = (abs(dy) == abs(dx) && isPathClearDiagonal(sourcePos, destinationPos));

  // If path is a straight (horizontal or vertical) and clear
  bool straightGood = ((sourcePos[0] == destinationPos[0] || sourcePos[1] == destinationPos[1]) && isPathClearStraight(sourcePos, destinationPos));

  // False if no clear diagonal and straight paths
  return (diagonalGood || straightGood); 
}

bool Pieces::isPathClearStraight(const int sourcePos[2], const int destinationPos[2]) const {
  int xStart = sourcePos[0];
  int yStart = sourcePos[1];
  int xEnd = destinationPos[0];
  int yEnd = destinationPos[1];

  int currentPos[2];
  // Horizontal movement
  if (xStart == xEnd) {
    // The direction
    int step = (yEnd > yStart) ? 1 : -1;
    // Start from the square after first step
    for (int y = yStart + step; y != yEnd; y += step) {
      currentPos[0] = xStart;
      currentPos[1] = y;
      // If position not empty
      if (!board->isPosEmpty(currentPos)) {
        return false;
      }
    }
  } else if (yStart == yEnd) {  // Vertical movement
    int step = (xEnd > xStart) ? 1 : -1;
    // Start from the square after first step
    for (int x = xStart + step; x != xEnd; x += step) {
      currentPos[0] = x;
      currentPos[1] = yStart;
      // If position not empty
      if (!board->isPosEmpty(currentPos)) {
        return false;
      }
    }
  } else { // Not a straight line
      return false; 
  }
  
  return true;
}

bool Pieces::isPathClearDiagonal(const int sourcePos[2], const int destinationPos[2]) const {
  int xStart = sourcePos[0];
  int yStart = sourcePos[1];
  int xEnd = destinationPos[0];
  int yEnd = destinationPos[1];

  if (abs(xEnd - xStart) != abs(yEnd - yStart)) {
    // Not a diagonal move
    return false;
  }

  // Direction of the diagonal
  int dy = (yEnd > yStart) ? 1 : -1;
  int dx = (xEnd > xStart) ? 1 : -1;
  
  int x = xStart + dx;
  int y = yStart + dy;
  int currentPos[2];

  // Loop up until the final position
  while (x != xEnd && y != yEnd) {
    currentPos[0] = x;
    currentPos[1] = y;
    // If a position is not empty
    if (!board->isPosEmpty(currentPos)) {
      return false;
    }
    x += dx;
    y += dy;
  }

  return true;
}

