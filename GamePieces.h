#ifndef GAME_PIECES_H
#define GAME_PIECES_H

#include <string>
#include <optional>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <sstream>

#include <iostream>

enum Color { WHITE, BLACK };

enum Piece { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

class ChessPiece {
	enum Color color;
	enum Piece type;
	
public:
	ChessPiece(Color color, Piece type):
		color(color), type(type) {}
	
	ChessPiece(std::string name) {
		// Convert string to lowercase
		std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
		
		// Check first two words for color and piece type
		std::istringstream iss(name);
		std::string namePart;
		
		iss >> namePart;
		if (namePart == "white") {
				color = WHITE;
		} else if (namePart == "black") {
				color = BLACK;
		} else {
				throw std::invalid_argument("Invalid color");
		}
		iss >> namePart;
		if (namePart == "pawn") {
				type = PAWN;
		} else if (namePart == "knight") {
				type = KNIGHT;
		} else if (namePart == "bishop") {
				type = BISHOP;
		} else if (namePart == "rook") {
				type = ROOK;
		} else if (namePart == "queen") {
				type = QUEEN;
		} else if (namePart == "king") {
				type = KING;
		} else {
				throw std::invalid_argument("Invalid piece type");
		}
	}
	
	Color get_color() const { return color; }
	Piece get_type() const { return type; }
	
	std::string to_string() const {
		std::string colors[] = { "white", "black" };
		std::string types[] = { "pawn", "knight", "bishop", "rook", "queen", "king" };
		return colors[color] + " " + types[type];
	}
	
	char to_char() const {
		if (color == WHITE) {
			char chars[] = { 'P', 'N', 'B', 'R', 'Q', 'K' };
			return chars[type];
		} else if (color == BLACK) {
			char chars[] = { 'p', 'n', 'b', 'r', 'q', 'k' };
			return chars[type];
		} else {
			throw std::invalid_argument("Can't convert piece to char: Piece is somehow neither white nor black");
		}
	}
};

class ChessBoard {
	using Square = std::optional<ChessPiece>;
	Square board[8][8];
	
public:
	ChessBoard() { reset(); }

	void reset() {
		Piece backRank[8] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };
		for (int file = 0; file < 8; file++) {
			board[file][0] = ChessPiece(WHITE, backRank[file]);
			board[file][1] = ChessPiece(WHITE, PAWN);
			board[file][6] = ChessPiece(BLACK, PAWN);
			board[file][7] = ChessPiece(BLACK, backRank[file]);
		}
	}
	
	void remove(int file, int rank) {
		if (file < 0 || file > 7 || rank < 0 || rank > 7) { throw std::out_of_range("Invalid board coordinates"); }
		board[file][rank].reset();
	}
	
	void place(int file, int rank, ChessPiece piece) {
		if (file < 0 || file > 7 || rank < 0 || rank > 7) { throw std::out_of_range("Invalid board coordinates"); }
		board[file][rank] = piece;
	}
	
	void place(int file, int rank, Color color, Piece type) {
		if (file < 0 || file > 7 || rank < 0 || rank > 7) { throw std::out_of_range("Invalid board coordinates"); }
		board[file][rank] = ChessPiece(color, type);
	}
	
	void place(int file, int rank, std::string name) {
		if (file < 0 || file > 7 || rank < 0 || rank > 7) { throw std::out_of_range("Invalid board coordinates"); }
		board[file][rank] = ChessPiece(name);
	}
	
	bool has_piece(int file, int rank) const {
		if (file < 0 || file > 7 || rank < 0 || rank > 7) { throw std::out_of_range("Invalid board coordinates"); }
		return board[file][rank].has_value();
	}

	ChessPiece at(int file, int rank) const {
		if (file < 0 || file > 7 || rank < 0 || rank > 7) { throw std::out_of_range("Invalid board coordinates"); }
		return board[file][rank].value();
	}
	
	void move_piece(int fileStart, int rankStart, int fileEnd, int rankEnd) {
		if (fileStart < 0 || fileStart > 7
			|| rankStart < 0 || rankStart > 7
			|| fileEnd < 0 || fileEnd > 7
			|| rankEnd < 0 || rankEnd > 7) {
			throw std::out_of_range("Invalid board coordinates");
		}
		if (!board[fileStart][rankStart].has_value()) {
			throw std::invalid_argument("No piece found at " + std::to_string(rankStart) + ", " + std::to_string(rankEnd));
		}
		place(fileEnd, rankEnd, at(fileStart, rankStart));
		remove(fileStart, rankStart);
	}
	
	std::string to_json() {
		std::stringstream ss;
		ss << "{\"board\": [";
		for (int file = 0; file < 8; file++) {
			ss << "[";
			for (int rank = 0; rank < 8; rank++) {
				std::string square;
				if (board[file][rank].has_value()) {
					square = at(file, rank).to_char();
				} else {
					square = "";
				}
				ss << "\"" << square << "\"";
				if (rank < 7) { ss << ","; }
			}
			ss << "]";
			if (file < 7) { ss << ","; }
		}
		ss << "]}";
		return ss.str();
	}
};

#endif