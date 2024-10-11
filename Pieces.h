#ifndef PIECES_H
#define PIECES_H

#include<iostream>

/** Global enum to represent piece colour, also used by the ChessBoard class.*/
enum Colour { White, Black };

/** Forward declaration */
class PieceFactory;
class IChessBoardActions;
class ChessBoard;

/** Parent for all chess pieces */
class Pieces {
public:
  /** Constructor for Pieces class.
   *  Initialises a chess piece with specified colour, board and hasMoved to false.
   *  @param _colour: The colour of the piece using the Colour enum(White/Black).
   *  @param _board: Pointer to the the abstract IChessBoardActions class that contains chess board the piece belongs to.
   */
  Pieces(Colour _colour, IChessBoardActions * _board) : pieceColour(_colour), hasMoved(false), board(_board) {}

  /** Virtual destructor for proper cleanup of derived classes
   *  Currently unused but included for proper cleanup in case of future extensions.
   */
  virtual ~Pieces() {}

  /** Pure virtual function to get the type of piece.
   *  @return Type of the piece as a string.
   */
  virtual const char* getType() const = 0;

  /** Pure virtual function overriden by each chess piece to check if the move is valid using piece-specific logic.
   *  Parameter positions are assumed to be 0 <= x < 8, for both row and column.
   *  @param sourcePos: Array containing the source position (row, column).
   *  @param destinationPos: Array containing the destination position (row, column).
   *  @return True if the move is valid, false otherwise.
   */
  virtual bool isValidMove(const int sourcePos[2], const int destinationPos[2]) const = 0;

  /** Retrieves the colour of the piece as a string.
   *  @return String representation of the piece's colour.
   */
  const char * getColourString() const;

  /** Gets the colour of the piece.
   *  @return The enum Colour of the piece (White/Black).
   */
  Colour getColour() const { return pieceColour; }

  /** Prints movement statement, calls ChessBoard movePiece() function to carry out the move.
   *  Updates the 'hasMoved' parameter and calls updateCastlingRights() overriden in the King and Rook.
   *  @param sourcePos: Array containing the source position (row, column).
   *  @param destinationPos: Array containing the destination position (row, column).
   */
  void makeMove(const int sourcePos[2], const int destinationPos[2]);

protected:
  Colour pieceColour;

  bool hasMoved;

  /** Pointer to a interface chessBoard object included in each Piece.*/
  IChessBoardActions * board;
  
  /** Virtual function for updating castling rights, overridden by King and Rook only.
   *  If the King is castling, it moves the rook to the appropriate position.
   *  @param sourcePos: Array containing the source position (row, column).
   *  @param destinationPos: Array containing the destination position (row, column).
   */
  virtual void updateCastlingRights(const int sourcePos[2], const int destinationPos[2]) {/* By default, do nothing */; }

  /** Updates the 'hasMoved' status of the piece to true*/
  virtual void updateMoved() { hasMoved = true; }
  
  /** Checks if the destination square is occupied by a piece of the same colour.
   *  @param destinationPos: Array containing the destination position (row, column).
   *  @return True if the destination has a piece of the same colour, false otherwise.
   */
  bool destinationSameColour(const int destinationPos[2]) const;

  /** Checks if the path is clear for a straight-line move (horizontal or vertical).
   *  @param sourcePos: Array containing the source position (row, column).
   *  @param destinationPos: Array containing the destination position (row, column).
   *  @return True if the path is clear, false otherwise.
   */
  bool isPathClearStraight(const int sourcePos[2], const int destinationPos[2]) const;

  /** Checks if the path is clear for a diagonal move.
   *  @param sourcePos: Array containing the source position (row, column).
   *  @param destinationPos: Array containing the destination position (row, column).
   *  @return True if the path is clear, false otherwise.
   */
  bool isPathClearDiagonal(const int sourcePos[2], const int destinationPos[2]) const;

  /** Calculates the change in position (delta x and delta y) for a move.
   *  @param sourcePos: Array containing the source position (row, column).
   *  @param destinationPos: Array containing the destination position (row, column).
   *  @param dyDxArray: Array stores the calculated dy and dx at index 0 and 1 respectively.
   */
  void calcDyDx(const int sourcePos[2], const int destinationPos[2], int dyDxArray[2]) const;
};


/** Subclass Declarations for Chess Pieces
 *  Each chess piece (Pawn, King, Rook, Bishop, Knight, Queen) inherits from the Pieces class.
 *  Constructor: Initialises a piece with a specified colour and an IChessBoardActions pointer to access specified board methods.
 *  isValidMove(): Contains piece-specific movement logic to determine the validity of a move.
 *  getType(): Returns the string representation of the piece type (e.g., "Pawn", "King").
 *  updateCastlingRights(): Implemented in King and Rook with specific logic for castling rights.
 *  Other pieces inherit the default behaviour (do nothing).
 */

class Pawn : public Pieces {
public:
  Pawn(Colour _colour, IChessBoardActions * _board) : Pieces(_colour, _board) {}
  const char* getType() const override { return "Pawn"; };
  bool isValidMove(const int sourcePos[2], const int destinationPos[2]) const override;
private:
};

class King : public Pieces {
public:
  King(Colour _colour, IChessBoardActions * _board) : Pieces(_colour, _board) {}
  const char* getType() const override { return "King"; }
  bool isValidMove(const int sourcePos[2], const int destinationPos[2]) const override;
private:
  /** Updates castling rights after a king's move.
   *  If castling occurs, this method calls the ChessBoard movePiece() on the appropriate rook.
   *  It updates the board's king position and sets castling availability to false.
   *  @param sourcePos: Array containing the king's initial position (row, column).
   *  @param destinationPos: Array containing the king's destination position (row, column).
   */
  void updateCastlingRights(const int sourcePos[2], const int destinationPos[2]) override ;
};

class Rook : public Pieces {
public:
  Rook(Colour _colour, IChessBoardActions * _board) : Pieces(_colour, _board) {}
  const char* getType() const override { return "Rook"; };
  bool isValidMove(const int sourcePos[2], const int destinationPos[2]) const override;
private:
  /** Updates the corresponding ChessBoard castling array to false based on the rook's initial position.
   *  It uses the ChessBoard global CastleDirection enum. 
   *  @param sourcePos: Array containing the rook's initial position (row, column).
   *  @param destinationPos: Array containing the rook's destination position (row, column).
   *  Note: The function only updates castling rights and does not involve moving the king.
   */
  void updateCastlingRights(const int sourcePos[2], const int destinationPos[2]) override ;
};

class Bishop : public Pieces {
public:
  Bishop(Colour _colour, IChessBoardActions * _board) : Pieces(_colour, _board) {}
  const char* getType() const override { return "Bishop"; }
  bool isValidMove(const int sourcePos[2], const int destinationPos[2]) const override;
private:
};

class Knight : public Pieces {
public:
  Knight(Colour _colour, IChessBoardActions * _board) : Pieces(_colour, _board) {}
  const char* getType() const override { return "Knight"; }
  bool isValidMove(const int sourcePos[2], const int destinationPos[2]) const override;
};

class Queen : public Pieces {
public:
  Queen(Colour _colour, IChessBoardActions * _board) : Pieces(_colour, _board) {}
  const char* getType() const override { return "Queen"; }
  bool isValidMove(const int sourcePos[2], const int destinationPos[2]) const override;
private:
};

/** PieceFactory Class
 *  Solely responsible for creating chess piece objects.
 *  This factory class ensures the encapsulation of the instantiation logic of chess pieces.
 *  It receives a raw pointer to the ChessBoard, which is acceptable as it only creates pieces 
 *  without modifying the board state or owning the board resource.
 */
class PieceFactory {
public:
  /** Creates and returns a new chess piece object based on input character and colour.
   *  It is a static method so it can be called without an instance of PieceFactory.
   *  @param c: Character representing the type of chess piece (e.g., 'p' for pawn).
   *  @param colour: The enum Colour of the piece to be created (White/Black).
   *  @param board: Raw pointer to the ChessBoard object on which the piece will be placed.
   *  @return Pointer to the created chess piece object, or nullptr if an invalid character is provided.
   *  Note: The function returns a nullptr for non-alphabetic characters or unsupported piece types.
   */
  static Pieces* createPiece(char c, Colour colour ,ChessBoard * board);
};

#endif // PIECES_H
