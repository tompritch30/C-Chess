chess: ChessMain.o ChessBoard.o Pieces.o
	g++ -Wall -g ChessMain.o ChessBoard.o Pieces.o -o chess

ChessMain.o: ChessMain.cpp ChessBoard.h Pieces.h
	g++ -Wall -g -c ChessMain.cpp

ChessBoard.o: ChessBoard.cpp ChessBoard.h
	g++ -Wall -g -c ChessBoard.cpp

Pieces.o: Pieces.cpp Pieces.h
	g++ -Wall -g -c Pieces.cpp

clean:
	rm -f *.o chess
