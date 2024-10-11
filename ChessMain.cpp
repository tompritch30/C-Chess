#include"ChessBoard.h"
#include"Pieces.h"

#include<iostream>

using std::cout;

int main() {

	cout << "========================\n";
	cout << "Testing the Chess Engine\n";
	cout << "========================\n\n";

	ChessBoard cb;
	cb.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
	
	cout << '\n';

	cb.submitMove("D7", "D6");
	cout << '\n';

	cb.submitMove("D4", "H6");
	cout << '\n';

	cb.submitMove("D2", "D4");
	cout << '\n';

	cb.submitMove("F8", "B4");
	cout << '\n';

      	cout << "=========================\n";
	cout << "Alekhine vs. Vasic (1931)\n";
	cout << "=========================\n\n";

	cb.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
	cout << '\n';

	cb.submitMove("E2", "E4");
	cb.submitMove("E7", "E6");
	cout << '\n';

	cb.submitMove("D2", "D4");
	cb.submitMove("D7", "D5");
	cout << '\n';

	cb.submitMove("B1", "C3");
	cb.submitMove("F8", "B4");
	cout << '\n';

	cb.submitMove("F1", "D3");
	cb.submitMove("B4", "C3");
	cout << '\n';

	cb.submitMove("B2", "C3");
	cb.submitMove("H7", "H6");
	cout << '\n';

	cb.submitMove("C1", "A3");
	cb.submitMove("B8", "D7");
	cout << '\n';

	cb.submitMove("D1", "E2");
	cb.submitMove("D5", "E4");
	cout << '\n';

	cb.submitMove("D3", "E4");
	cb.submitMove("G8", "F6");
	cout << '\n';

	cb.submitMove("E4", "D3");
	cb.submitMove("B7", "B6");
	cout << '\n';

	cb.submitMove("E2", "E6");
	cb.submitMove("F7", "E6");
	cout << '\n';

	cb.submitMove("D3", "G6");
	cout << '\n';

	cout << "=========================\n";
	cout << "Potekmin vs. Alekhine (1912)\n";
	cout << "=========================\n\n";

	cb.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
	cout << '\n';

	cb.submitMove("E2", "E4"); cb.submitMove("C7", "C5");
cb.submitMove("G2", "G3"); cb.submitMove("G7", "G6");
cb.submitMove("F1", "G2"); cb.submitMove("F8", "G7");
cb.submitMove("G1", "E2"); cb.submitMove("B8", "C6");
cb.submitMove("C2", "C3"); cb.submitMove("G8", "F6");
cb.submitMove("B1", "A3"); cb.submitMove("D7", "D5");
cb.submitMove("E4", "D5"); cb.submitMove("F6", "D5");
cb.submitMove("A3", "C2"); cb.submitMove("E8", "G8");
cb.submitMove("D2", "D4"); cb.submitMove("C5", "D4");
cb.submitMove("C3", "D4"); cb.submitMove("C8", "G4");
cb.submitMove("F2", "F3"); cb.submitMove("G4", "F5");
cb.submitMove("C2", "E3"); cb.submitMove("D8", "A5");
cb.submitMove("E1", "F2"); cb.submitMove("D5", "B4");
cb.submitMove("E3", "F5"); cb.submitMove("A5", "F5");
cb.submitMove("G3", "G4"); cb.submitMove("B4", "D3");
cb.submitMove("F2", "G3"); cb.submitMove("C6", "D4");
cb.submitMove("G4", "F5"); cb.submitMove("D4", "F5");
cb.submitMove("G3", "G4"); cb.submitMove("H7", "H5");
cb.submitMove("G4", "H3"); cb.submitMove("D3", "F2");


	cout << "=========================\n";
	cout << "Stalemate game\n";
	cout << "=========================\n\n";

	cb.loadState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
	cout << '\n';

cb.submitMove("D2", "D4"); cb.submitMove("G8", "F6"); cout << "\n";
cb.submitMove("C2", "C4"); cb.submitMove("E7", "E6"); cout << "\n";
cb.submitMove("G2", "G3"); cb.submitMove("F8", "B4"); cout << "\n";
cb.submitMove("C1", "D2"); cb.submitMove("D8", "E7"); cout << "\n";
cb.submitMove("F1", "G2"); cb.submitMove("B4", "D2"); cout << "\n";
cb.submitMove("D1", "D2"); cb.submitMove("D7", "D6"); cout << "\n";
cb.submitMove("B1", "C3"); cb.submitMove("E8", "G8"); cout << "\n";
cb.submitMove("G1", "F3"); cb.submitMove("E6", "E5"); cout << "\n";
cb.submitMove("E1", "G1"); cb.submitMove("F8", "E8"); cout << "\n";
cb.submitMove("E2", "E4"); cb.submitMove("C8", "G4"); cout << "\n";
cb.submitMove("D4", "D5"); cb.submitMove("G4", "F3"); cout << "\n";
cb.submitMove("G2", "F3"); cb.submitMove("B8", "D7"); cout << "\n";
cb.submitMove("B2", "B4"); cb.submitMove("A7", "A5"); cout << "\n";
cb.submitMove("A2", "A3"); cb.submitMove("A8", "A6"); cout << "\n";
cb.submitMove("C3", "B5"); cb.submitMove("D7", "B6"); cout << "\n";
cb.submitMove("A1", "C1"); cb.submitMove("A5", "B4"); cout << "\n";
cb.submitMove("A3", "B4"); cb.submitMove("E7", "D7"); cout << "\n";
cb.submitMove("D2", "D3"); cb.submitMove("A6", "A4"); cout << "\n";
cb.submitMove("D3", "B3"); cb.submitMove("E8", "A8"); cout << "\n";
cb.submitMove("F1", "D1"); cb.submitMove("H7", "H5"); cout << "\n";
cb.submitMove("H2", "H4"); cb.submitMove("G7", "G6"); cout << "\n";
cb.submitMove("C1", "B1"); cb.submitMove("F6", "G4"); cout << "\n";
cb.submitMove("F3", "E2"); cb.submitMove("D7", "E7"); cout << "\n";
cb.submitMove("B1", "C1"); cb.submitMove("C7", "C6"); cout << "\n";
cb.submitMove("D5", "C6"); cb.submitMove("B7", "C6"); cout << "\n";
cb.submitMove("C4", "C5"); cb.submitMove("D6", "C5"); cout << "\n";
cb.submitMove("B4", "C5"); cb.submitMove("B6", "D7"); cout << "\n";
cb.submitMove("B5", "D6"); cb.submitMove("D7", "F6"); cout << "\n";
cb.submitMove("E2", "C4"); cb.submitMove("G4", "F2"); cout << "\n";
cb.submitMove("G1", "F2"); cb.submitMove("A4", "A3"); cout << "\n";
cb.submitMove("C4", "F7"); cb.submitMove("G8", "G7"); cout << "\n";
cb.submitMove("B3", "E6"); cb.submitMove("A3", "A2"); cout << "\n";
cb.submitMove("F2", "G1"); cb.submitMove("A8", "A3"); cout << "\n";
cb.submitMove("D6", "E8"); cb.submitMove("G7", "H6"); cout << "\n";
cb.submitMove("E8", "F6"); cb.submitMove("A3", "G3"); cout << "\n";
cb.submitMove("G1", "H1"); cb.submitMove("E7", "F7"); cout << "\n";
cb.submitMove("D1", "D7"); cb.submitMove("F7", "F6"); cout << "\n";
cb.submitMove("E6", "F6"); cb.submitMove("A2", "H2"); cout << "\n";
cb.submitMove("H1", "H2"); cb.submitMove("G3", "H3"); cout << "\n";
cb.submitMove("H2", "H3");


	
       

return 0;
}
