#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include <ostream>
#include "util/GL-math.hpp"

using namespace std;
using namespace GL;

#define MAX_DELETED_LETTERS 30

class Board {
public:

	Board() { }

	void clear() { *this = Board(); }

	bool isEmpty() { return letters.size() - numDeletedLetters == 0; }

	char operator () (ivec2);

	void addLetter(char c, ivec2 pos);

	void addWord(string s, ivec2 start, bool vertical);

	string toString();

protected:

	struct letter { char c; ivec2 pos; bool isDeleted = false; };
	vector<letter> letters;
	int numDeletedLetters = 0;
	vector<char> board;
	bool needsRasterize = true;
	ivec2 boardSize;
	ivec2 lbound;

	void cleanup();

	void rasterize();

	int flatten(ivec2);

};

char Board::operator () (ivec2 pos) {

	rasterize();
	if (isEmpty()) return 0;
	if (any(pos - lbound < 0 || pos - lbound >= boardSize)) return 0;
	return board[flatten(pos)];

}

void Board::addLetter(char c, ivec2 pos) {

	for (letter& l : letters) if (all(l.pos == pos) && !l.isDeleted) { l.isDeleted = true; numDeletedLetters++; break; }
	letters.push_back(letter{ c, pos });
	needsRasterize = true;

}

void Board::addWord(string s, ivec2 start, bool vertical) {

	for (int i = 0; i < s.size(); i++) addLetter(s[i], start + (vertical ? ivec2(0, i) : ivec2(i, 0)));
	if (numDeletedLetters >= MAX_DELETED_LETTERS) cleanup();

}

string Board::toString() {

	if (isEmpty()) return "empty board :(\n";
	rasterize();

#define _Board_wall "----------\n"

	string bs = _Board_wall;
	for (int i = 0; i < board.size(); i++) { bs += board[i]; if (i % boardSize.x == boardSize.x - 1) bs += "\n"; }
	return bs + _Board_wall;

}

void Board::cleanup() {

	vector<letter> cleanvec;
	for (letter l : letters) if (!l.isDeleted) cleanvec.push_back(l);
	letters = cleanvec;
	numDeletedLetters = 0;

}

void Board::rasterize() {

	if (isEmpty()) return;
	if (!needsRasterize) return;

	lbound = letters[0].pos;
	ivec2 ubound = lbound;
	for (letter l : letters) if (!l.isDeleted) { lbound = min(lbound, l.pos); ubound = max(ubound, l.pos); }
	
	boardSize = 1 + ubound - lbound;
	board = vector<char>(boardSize.x * boardSize.y, ' ');
	for (letter l : letters) if (!l.isDeleted) { board[flatten(l.pos)] = l.c; }
	needsRasterize = false;

}

int Board::flatten(ivec2 pos) { return boardSize.x * (pos.y - lbound.y) + (pos.x - lbound.x); }

ostream& operator << (ostream& os, Board& b) { os << b.toString(); return os; }

#endif