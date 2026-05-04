#include "GamePieces.h"

#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <string>
#include <algorithm>

#ifndef RULES_H
#define RULES_H

class Rule {
	static std::vector<Rule*> rules;
	
public:
	Rule() {
		rules.push_back(this);
	}
};

class Move {
	const int fromFile;
	const int fromRank;
	const int toFile;
	const int toRank;
	std::function<void(Move&, ChessBoard&)> apply = [](Move& move, ChessBoard& board) {  board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank); };

public:
	Move(int fromFile, int fromRank, int toFile, int toRank) : fromFile(fromFile), fromRank(fromRank), toFile(toFile), toRank(toRank) {}
	
	void execute(ChessBoard& board) { apply(*this, board); }

	void overwrite_apply(std::function<void(Move&, ChessBoard&)> newApply) { apply = newApply; }

	std::string to_string(const ChessBoard& board) const {
		std::stringstream ss;
		// FIXME: Adjust logic to match standard chess notation
		// This will be tricky once nonstandard rules are added due to the need for custom notation
		ss << std::toupper(board.at(fromFile, fromRank).to_char()) << " from " << char('a' + fromFile) << (fromRank + 1) << char('a' + toFile) << (toRank + 1);
		return ss.str();
	}
	
};

class MoveGenerator : public Rule {
public:
	virtual std::vector<Move> get_moves(int file, int rank) {}
};

class MoveTransformer : public Rule {
public:
	virtual void apply(std::vector<Move>& moves) {}
};

class MoveRestrictor : public Rule {
public:
	virtual void apply(std::vector<Move>& moves) {}
};

#endif

// Code to be executed when building as an executable for testing purposes
#ifdef BUILD_AS_EXE

#include <iostream>

int main() {
	std::cout << "Hello, world!" << std::endl;
	
	return 0;
}

#endif