#ifndef RULES_H
#define RULES_H

#include "GamePieces.h"

#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>

#include <iostream>

class Rule {};

class RuleSet { // FIXME (maybe): use dynamic_cast to sort rules upon construction
    std::vector<Rule*> rules;

public:
    RuleSet(std::vector<Rule*> rules) : rules(rules) {
    	get_rule_sets().push_back(this);
    }

	~RuleSet() {
		for (int i = 0; i < rules.size(); i++) {
			delete rules.at(i);
		}
	}
	
	static std::vector<RuleSet*>& get_rule_sets() {
		static std::vector<RuleSet*> ruleSets;
		return ruleSets;
		}

    const std::vector<Rule*> get_rules() const { return rules; }
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

class MoveGrid {
	std::vector<Move> grid[8][8];
public:
	std::vector<Move>& at(int file, int rank) {
		if (file < 0 || file > 7 || rank < 0 || rank > 7) {
			throw std::out_of_range("Invalid MoveGrid coordinates");
		}
		return grid[file][rank];
	}

	void clear() {
		for (int file = 0; file < 8; file++) {
			for (int rank = 0; rank < 8; rank++) {
				grid[file][rank].clear();
			}
		}
	}
};



class MoveGenerator : public Rule {
	std::function<void(ChessBoard&, MoveGrid&, int, int)> generate;
	
public:
	MoveGenerator(std::function<void(ChessBoard&, MoveGrid&, int, int)> generate) : generate(generate) {}
	
	void get_moves(ChessBoard& board, MoveGrid& moveGrid, int file, int rank) {
		generate(board, moveGrid, file, rank);
	}
};


class AttackGenerator : public Rule {
	std::function<void(ChessBoard&, MoveGrid&, int, int)> generate;

public:
	AttackGenerator(std::function<void(ChessBoard&, MoveGrid&, int, int)> generate) : generate(generate) {}

	void get_attacks(ChessBoard& board, MoveGrid& moveGrid, int file, int rank) {
		generate(board, moveGrid, file, rank);
	}
};



class MoveTransformer : public Rule {
	std::function<void(ChessBoard& board, MoveGrid&)> transform;
	
public:
	MoveTransformer(std::function<void(ChessBoard& board, MoveGrid&)> transform) : transform(transform) {}
	
	void apply(ChessBoard& board, MoveGrid& moveGrid) {
		transform(board, moveGrid);
	}
};



class MoveRestrictor : public Rule {
	std::function<void(ChessBoard& board, MoveGrid&)> restrict;
public:
	MoveRestrictor(std::function<void(ChessBoard& board, MoveGrid&)> restrict) : restrict(restrict) {}
	
	void apply(ChessBoard& board, MoveGrid& moves) {
		restrict(board, moves);
	}
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