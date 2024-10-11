#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include"Pieces.h"
#include<iostream>
#include<cstring>
#include<cctype>

/** Castle Direction indexes represent the indexes in canCastleArray. */
enum CastleDirection {whiteKingSide, whiteQueenSide, blackKingSide, blackQueenSide};

/** Abstract class so only Pieces can call the functions below */
class IChessBoardActions {
public:
  virtual ~IChessBoardActions() = default;
  virtual void setCastleArray(const int index, const bool value) = 0;
  virtual bool canCastle(const int direction, const Colour colour) const = 0;
  virtual bool isInsideBoard(const int row, const int col) const = 0;
  virtual bool isPosEmpty(const int pos[2]) const = 0;
  virtual Colour getPosColour(const int pos[2]) const = 0;
  virtual void movePiece(const int sourcePos[2], const int destinationPos[2]) = 0;
  virtual bool doesMoveCauseCheck(const int sourcePos[2], int destinationPos[2], Colour colour) = 0;
  virtual void convertToRowCol(const char* square, int position[2]) const = 0;
  virtual void rowColToString(char * square, const int position[2]) const = 0;
  virtual const char * getPosType(const int pos[2]) const = 0;
  virtual bool isKingInCheck(const Colour kingColour) = 0;
};

class ChessBoard : public IChessBoardActions {
public:
 
  /** Default constructor for ChessBoard.
   *  Initialises an empty 8x8 chessboard with all squares set to nullptr.
   */
  ChessBoard();

  /** Destructor for ChessBoard.
   *  Deallocates all dynamically allocated pieces and clears the board.
   */
  ~ChessBoard();

  /** Loads the board state from a given FEN string, a valid board state is assumed.
   *  Clears the current board state before setting up the new state.
   *  Calls the PiecesFactory createPiece() function to create specific piece objects (e.g. Pawns, Kings)
   *  @param fen: The FEN string representing the board state.
   */
  void loadState(const char* fen);

  /** Validates a chess move, based on the board state, if the move would cause check
   *  and uses the Piece's specific isValid().
   *  @param sourceSquare: The source square in algebraic notation (e.g., "e2").
   *  @param destinationSquare: The destination square in algebraic notation (e.g., "e4").
   */
  void submitMove(const char* sourceSquare, const char* destinationSquare);  
  
protected:
  /** Converts the FEN string to the board array.
   *  Sets up the board and updates the current player's colour based on the FEN string.
   *  @param fen: The FEN string representing the board state.
   */
  void boardToArray(const char* fen);

  /**
   * Functions only accessible to Pieces class via abstract Class IChessBoardActions: 
   */

  /** Checks if a given position is inside the board boundaries (0 <= x < 8)
   *  @param row: The row index of the position.
   *  @param col: The column index of the position.
   *  @return True if the position is within the 8x8 board, false otherwise.
   */
  bool isInsideBoard(const int row, const int col) const override { return row >= 0 && row < 8 && col >= 0 && col < 8;}

  /** Checks if a given position on the board is empty.
   *  @param pos: Array containing the position (row, column) to check.
   *  @return True if the position is empty (nullptr), false if it contains a piece.
   */
  bool isPosEmpty(const int pos[2]) const override;

  /** Retrieves the colour of the piece at a given position.
   *  @param pos: Array containing the position (row, column) to check.
   *  @return The colour of the piece at the given position.
   */
  Colour getPosColour(const int pos[2]) const override;

  /** Retrieves the type of the piece at a given position.
   *  @param pos: Array containing the position (row, column) to check.
   *  @return A string of the piece type at the given position.
   */
  const char * getPosType(const int pos[2]) const override;

  /** Determines if a move causes a check for the player's own king.
   *  Simulates a move and checks if it results in the player's king being in check.
   *  @param sourcePos: Array containing the source position (row, column).
   *  @param destinationPos: Array containing the destination position (row, column).
   *  @param colour: The colour of the player making the move.
   *  @return True if the move causes a check, false otherwise.
   */
  bool doesMoveCauseCheck(const int sourcePos[2], int destinationPos[2], Colour colour) override;
  
  /** Moves a piece from the source position to the destination position.
   *  Handles piece capture if the destination contains an opponent's piece.
   *  @param sourcePos: Array containing the source position (row, column).
   *  @param destinationPos: Array containing the destination position (row, column).
   */
  void movePiece(const int sourcePos[2], const int destinationPos[2]) override;

  /** Checks if the king of a specified colour is in check.
   *  @param kingColour: The colour of the king to check.
   *  @return True if the king is in check, false otherwise.
   */
  bool isKingInCheck(const Colour kingColour) override;
    
  /** Sets the availability of castling in a specific direction.
   *  @param index: The index representing the castling direction.
   *  @param value: Boolean value indicating the availability of castling in that direction.
   */
  void setCastleArray(const int index, const bool value) override;

  /** Checks if castling is available in a specific direction for a given colour.
   *  @param direction: The direction of castling.
   *  @param colour: The enum colour of the player attempting to castle.
   *  @return True if castling is available in the specified direction, False otherwise.
   */
  bool canCastle(const int direction, const Colour colour) const override;

  /** Converts a square in algebraic notation to its row and column indices. E.g. "E4" to {4, 4} 
   *  @param square: A string representing the square in algebraic notation (e.g., "e4").
   *  @param position: An array of two integers to store the converted row and column indices.
   */
  void convertToRowCol(const char* square, int position[2]) const override;

  /** Converts a board position to a string in algebraic notation.
   *  @param square: A string to store the algebraic notation of the square.
   *  @param position: An array of two integers specifying the position (row and column).
   */
  void rowColToString(char * square, const int position[2]) const override;
  
private:
  /** Each element points to a chess piece or is nullptr for an empty square. */
  Pieces* piecesBoard[8][8];

  /** Enum Colour of the player who is currently to move, White or Black. */
  Colour colour;

  /** Array storing the availability of castling for both players and both sides.
   *  Index 0 and 1 correspond to white's king-side and queen-side castling.
   *  Index 2 and 3 correspond to black's king-side and queen-side castling.
   */
  bool canCastleArray[4] = {false, false, false, false};

  /** Flag indicating whether the game is over. 
   *  Set to true when the game reaches checkmate or stalemate.
   */
  bool isGameOver = false;
  
  /** Clears the chessboard, deallocating all pieces.
   *  Iterates over the board and deletes any dynamically allocated piece, setting pointers to nullptr.
   */
  void clearBoard();  

  /** Checks if a player can escape from check.
   *  Evaluates if any move can remove the king from check.
   *  @param kingColour: The colour of the king to check.
   *  @return True if there's a move to escape check, false otherwise.
   */
  bool canEscapeCheck(const Colour kingColour);

    /** Returns the current player's colour as a string.
   *  @return A string representing the current player's colour ("White" or "Black").
   */
  const char* getColourString() const;

  /** Helper functions for submitMove() */

  /**
   * Prints statement if the board coordinates are -1, using the output from ConvertRowToCol().
   * @param sourcePos Array containing the source position's row and column indices.
   * @param destinationPos Array containing the destination position's row and column indices.
   * @return true if both source and destination are within the chessboard bounds.
   *         If false, an appropriate error message is printed to the console.
   */ 
  bool isInBounds(int * sourcePos, int * destinationPos);

  /**
   * Validates if there is a piece at the start position.
   * @param startPosition Pointer to the piece at the start position.
   * @param sourceSquare The source square in string format (e.g., "E2").
   * @return true if there is a piece at the start position, false otherwise.
   */
  bool isValidStartPosition(Pieces* startPosition, const char* sourceSquare);

  /**
   * Checks if it is the correct turn for the piece at the start position.
   * @param startPosition Pointer to the piece at the start position.
   * @return true if it is the correct turn for the piece, false otherwise.
   */
  bool isValidTurn(Pieces* startPosition);

  /**
   * Validates if the move from the source position to the destination position is valid.
   * @param startPosition Pointer to the piece at the start position.
   * @param sourcePos Array representing the source position.
   * @param destinationPos Array representing the destination position.
   * @param destinationSquare The destination square in string format (e.g., "E4").
   * @return true if the move is valid, false otherwise.
   */
  bool isValidMove(Pieces* startPosition, int sourcePos[], int destinationPos[], const char* destinationSquare);

  /**
   * Executes the move from the source position to the destination position.
   * @param startPosition Pointer to the piece at the start position.
   * @param sourcePos Array representing the source position.
   * @param destinationPos Array representing the destination position.
   */
  void executeMove(Pieces* startPosition, int sourcePos[], int destinationPos[]);

  /**
   * Checks if the game is over due to checkmate or stalemate.
   * @return true if the game is over, false otherwise.
   */
  bool checkGameOver();

  /**
   * Performs checks after a move is made, such as check, checkmate, or stalemate.
   */
  void postMoveChecks();
};

#endif // CHESSBOARD_H
