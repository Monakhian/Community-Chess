#include "GamePieces.h"

#include <functional>
#include <vector>
#include <string>
#include <sstream>

#ifndef RULES_H
#define RULES_H

class Rule {
	static std::vector<Rule*> rules;
	
public:
	Rule() {
		rules.append(this);
	}
};

struct Move {
	int fromFile;
	int fromRank;
	int toFile;
	int toRank;
	string type;
	function<void(ChessBoard& board)> execute;
	string to_string(const ChessBoard& board) {
		stringstream ss;
		
	}
};

class MoveGenerator : public Rule {
public:
	virtual std::vector<Move> get_moves(int file, int rank) {
		
	}
};

#endif