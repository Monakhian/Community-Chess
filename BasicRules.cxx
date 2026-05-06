#include "GamePieces.h"
#include "Rules.h"

#include <vector>

RuleSet testRuleSet(std::vector<Rule*>{
    new MoveGenerator([](ChessBoard& board, int file, int rank) { // Pawn move
        std::vector<Move> moves;
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == PAWN) {
            if (board.at(file, rank).get_color() == WHITE && rank < 7) {
                if (!board.has_piece(file, rank + 1)) {
                    moves.emplace_back(Move{file, rank, file, rank + 1});
                }
            } else if (board.at(file, rank).get_color() == BLACK && rank > 0) {
                if (!board.has_piece(file, rank - 1)) {
                    moves.emplace_back(Move{file, rank, file, rank - 1});
                }
            }
        }
        return moves;
    }),
    new MoveGenerator([](ChessBoard& board, int file, int rank) { // Pawn capture
        std::vector<Move> moves;
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == PAWN) {
            Color color = board.at(file, rank).get_color();
            int direction = (color == WHITE) ? 1 : -1;
            // Capture to the left
            if (file > 0 && board.has_piece(file - 1, rank + direction) && board.at(file - 1, rank + direction).get_color() != color) {
                moves.emplace_back(Move{file, rank, file - 1, rank + direction});
            }
            // Capture to the right
            if (file < 7 && board.has_piece(file + 1, rank + direction) && board.at(file + 1, rank + direction).get_color() != color) {
                moves.emplace_back(Move{file, rank, file + 1, rank + direction});
            }
        }
        return moves;
    }),
    new MoveGenerator([](ChessBoard& board, int file, int rank) { // Knight move
        std::vector<Move> moves;
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == KNIGHT) {
            std::vector<std::pair<int, int>> knightMoves = {
                {1, 2}, {2, 1}, {2, -1}, {1, -2},
                {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
            };
            for (const auto& [df, dr] : knightMoves) {
                int newFile = file + df;
                int newRank = rank + dr;
                if (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (!board.has_piece(newFile, newRank) || board.at(newFile, newRank).get_color() != board.at(file, rank).get_color()) {
                        moves.emplace_back(Move{file, rank, newFile, newRank});
                    }
                }
            }
        }
        return moves;
    }),
    new MoveGenerator([](ChessBoard& board, int file, int rank) { // Bishop move
        std::vector<Move> moves;
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == BISHOP) {
            std::vector<std::pair<int, int>> directions = {
                {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
            };
            for (const auto& [df, dr] : directions) {
                int newFile = file + df;
                int newRank = rank + dr;
                while (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (!board.has_piece(newFile, newRank)) {
                        moves.emplace_back(Move{file, rank, newFile, newRank});
                    } else {
                        if (board.at(newFile, newRank).get_color() != board.at(file, rank).get_color()) {
                            moves.emplace_back(Move{file, rank, newFile, newRank});
                        }
                        break;
                    }
                    newFile += df;
                    newRank += dr;
                }
            }
        }
        return moves;
    }),
    new MoveGenerator([](ChessBoard& board, int file, int rank) { // Rook move
        std::vector<Move> moves;
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == ROOK) {
            std::vector<std::pair<int, int>> directions = {
                {1, 0}, {-1, 0}, {0, 1}, {0, -1}
            };
            for (const auto& [df, dr] : directions) {
                int newFile = file + df;
                int newRank = rank + dr;
                while (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (!board.has_piece(newFile, newRank)) {
                        moves.emplace_back(Move{file, rank, newFile, newRank});
                    } else {
                        if (board.at(newFile, newRank).get_color() != board.at(file, rank).get_color()) {
                            moves.emplace_back(Move{file, rank, newFile, newRank});
                        }
                        break;
                    }
                    newFile += df;
                    newRank += dr;
                }
            }
        }
        return moves;
    }),
    new MoveGenerator([](ChessBoard& board, int file, int rank) { // Queen move
        std::vector<Move> moves;
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == QUEEN) {
            std::vector<std::pair<int, int>> directions = {
                {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
            };
            for (const auto& [df, dr] : directions) {
                int newFile = file + df;
                int newRank = rank + dr;
                while (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (!board.has_piece(newFile, newRank)) {
                        moves.emplace_back(Move{file, rank, newFile, newRank});
                    } else {
                        if (board.at(newFile, newRank).get_color() != board.at(file, rank).get_color()) {
                            moves.emplace_back(Move{file, rank, newFile, newRank});
                        }
                        break;
                    }
                    newFile += df;
                    newRank += dr;
                }
            }
        }
        return moves;
    }),
    new MoveGenerator([](ChessBoard& board, int file, int rank) { // King move
        std::vector<Move> moves;
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == KING) {
            std::vector<std::pair<int, int>> kingMoves = {
                {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
            };
            for (const auto& [df, dr] : kingMoves) {
                int newFile = file + df;
                int newRank = rank + dr;
                if (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (!board.has_piece(newFile, newRank) || board.at(newFile, newRank).get_color() != board.at(file, rank).get_color()) {
                        moves.emplace_back(Move{file, rank, newFile, newRank});
                    }
                }
            }
        }
        return moves;
    }),
    new MoveTransformer([](ChessBoard& board, std::vector<Move>& moves) { // Pawn promotion
        for (auto& move : moves) {
            if (board.at(move.fromFile, move.fromRank).get_type() == PAWN && (
                (move.toRank == 7 && board.at(move.fromFile, move.fromRank).get_color() == WHITE) ||
                (move.toRank == 0 && board.at(move.fromFile, move.fromRank).get_color() == BLACK)
            )) {
                move.overwrite_apply([](const Move& move, ChessBoard& board) {
                    board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank);
                    // FIXME: This currently promotes to queen by default, but the player should be able to choose
                    board.place(move.toFile, move.toRank, board.at(move.toFile, move.toRank).get_color(), QUEEN);
                });
                move.overwrite_string_rep([](const Move& move, const ChessBoard& board) {
                    std::stringstream ss;
                    // FIXME: Once we have proper promotion selection, adjust this to reflect the piece the pawn is being promoted to instead of always showing "=Q"
                    ss << std::toupper(board.at(move.fromFile, move.fromRank).to_char()) << char('a' + move.fromFile) << (move.fromRank + 1) << char('a' + move.toFile) << (move.toRank + 1) << "=Q";
                    return ss.str();
                });
            }
        }
    }),
    new MoveGenerator([](ChessBoard& board, int file, int rank) { // Castling move
        // FIXME: Add logic to detect when king/rooks have moved and to check for check conditions that would prevent castling
        std::vector<Move> moves;
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == KING) {
            Color color = board.at(file, rank).get_color();
            int backRank = (color == WHITE) ? 0 : 7;
            // Kingside castling
            if (file == 4 && rank == backRank &&
                board.has_piece(7, backRank) && board.at(7, backRank).get_type() == ROOK &&
                !board.has_piece(5, backRank) && !board.has_piece(6, backRank)) {
                moves.emplace_back(Move{4, backRank, 6, backRank});
                moves.back().overwrite_apply([](const Move& move, ChessBoard& board) {
                    board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank);
                    // Move the rook as well
                    board.move_piece(7, move.fromRank, 5, move.fromRank);
                });
                moves.back().overwrite_string_rep([](const Move& move, const ChessBoard& board) {
                    return "O-O";
                });
            }
            // Queenside castling
            if (file == 4 && rank == backRank &&
                board.has_piece(0, backRank) && board.at(0, backRank).get_type() == ROOK &&
                !board.has_piece(1, backRank) && !board.has_piece(2, backRank) && !board.has_piece(3, backRank)) {
                moves.emplace_back(Move{4, backRank, 2, backRank});
                moves.back().overwrite_apply([](const Move& move, ChessBoard& board) {
                    board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank);
                    // Move the rook as well
                    board.move_piece(0, move.fromRank, 3, move.fromRank);
                });
                moves.back().overwrite_string_rep([](const Move& move, const ChessBoard& board) {
                    return "O-O-O";
                });
            }
        }
        return moves;
    }),
    new MoveGenerator([](ChessBoard& board, int file, int rank) { // En passant move
        // FIME: Add logic to only allow en passant on the turn immediately following the opponent's pawn moving two squares forward from its starting position
        std::vector<Move> moves;
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == PAWN) {
            Color color = board.at(file, rank).get_color();
            int direction = (color == WHITE) ? 1 : -1;
            // Check for en passant capture to the left
            if (file > 0 && board.has_piece(file - 1, rank) && board.at(file - 1, rank).get_type() == PAWN && board.at(file - 1, rank).get_color() != color) {
                moves.emplace_back(Move{file, rank, file - 1, rank + direction});
                moves.back().overwrite_apply([direction](const Move& move, ChessBoard& board) {
                    board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank);
                    board.remove(move.toFile, move.toRank - direction);
                });
            }
            // Check for en passant capture to the right
            if (file < 7 && board.has_piece(file + 1, rank) && board.at(file + 1, rank).get_type() == PAWN && board.at(file + 1, rank).get_color() != color) {
                moves.emplace_back(Move{file, rank, file + 1, rank + direction});
                moves.back().overwrite_apply([direction](const Move& move, ChessBoard& board) {
                    board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank);
                    board.remove(move.toFile, move.toRank - direction);
                });
            }
        }
        return moves;
    }) // FIXME: Make sure en passant does not happen off the back of the board
});