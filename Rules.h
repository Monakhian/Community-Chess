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
	// Functions below mimic having mutable member functions
	// Allows for rules to modify the behavior of moves without needing to create new move types for every rule change
	// Example 1: Mutating "apply" allows for pawn promotion
	// Example 2: Mutating "string_rep" allows for castling to be denoted as O-O and O-O-O instead of Kg1 and Kc1
	// Further, this allows for custom rules such as swap chess and duck chess to define their own notation
	std::function<void(const Move&, ChessBoard&)> apply = [](const Move& move, ChessBoard& board) {  board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank); };
	std::function<std::string(const Move&, const ChessBoard&)> string_rep = [](const Move& move, const ChessBoard& board) {
		std::stringstream ss;
		// FIXME: Adjust logic to match standard chess notation
		ss << std::toupper(board.at(move.fromFile, move.fromRank).to_char()) << char('a' + move.fromFile) << (move.fromRank + 1) << char('a' + move.toFile) << (move.toRank + 1);
		return ss.str();
	};

public:
	const int fromFile;
	const int fromRank;
	const int toFile;
	const int toRank;

	Move(int fromFile, int fromRank, int toFile, int toRank) : fromFile(fromFile), fromRank(fromRank), toFile(toFile), toRank(toRank) {}
	
	void execute(ChessBoard& board) const { apply(*this, board); }

	void overwrite_apply(std::function<void(const Move&, ChessBoard&)> newApply) { apply = newApply; }

	std::string to_string(const ChessBoard& board) const { return string_rep(*this, board); }

	void overwrite_string_rep(std::function<std::string(const Move&, const ChessBoard&)> newStringRep) { string_rep = newStringRep; }
	
};

class MoveGenerator : public Rule {
public:
	virtual std::vector<Move> get_moves(int file, int rank) = 0;
};

class MoveTransformer : public Rule {
public:
	virtual void apply(std::vector<Move>& moves) = 0;
};

class MoveRestrictor : public Rule {
public:
	virtual void apply(std::vector<Move>& moves) = 0;
};

#endif

// Code to be executed when building as an executable for testing purposes
// only works with gcc/clang compilers
// notably does not work with MSVC (visual studio code)
#if __INCLUDE_LEVEL__ == 0

#include <iostream>

int main() {
	std::cout << "Hello, world!" << std::endl;
	
	return 0;
}

#endif