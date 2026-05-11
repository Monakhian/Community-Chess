#include "GamePieces.h"
#include "Rules.h"

#include <vector>
#include <memory>

RuleSet basicRules (std::vector<std::shared_ptr<Rule>>{
    std::make_shared<MoveGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Pawn move
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == PAWN) {
            if (board.at(file, rank).get_color() == WHITE && rank < 7) {
                if (!board.has_piece(file, rank + 1)) {
                    moveGrid.at(file, rank + 1).emplace_back(Move{file, rank, file, rank + 1});
                }
            } else if (board.at(file, rank).get_color() == BLACK && rank > 0) {
                if (!board.has_piece(file, rank - 1)) {
                    moveGrid.at(file, rank).emplace_back(Move{file, rank, file, rank - 1});
                }
            }
        }
    }),
    std::make_shared<AttackGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Pawn capture
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == PAWN) {
            Color color = board.at(file, rank).get_color();
            int direction = (color == WHITE) ? 1 : -1;
            // Capture to the left
            if (file > 0 && board.has_piece(file - 1, rank + direction) && board.at(file - 1, rank + direction).get_color() != color) {
                moveGrid.at(file - 1, rank + direction).emplace_back(Move{file, rank, file - 1, rank + direction});
            }
            // Capture to the right
            if (file < 7 && board.has_piece(file + 1, rank + direction) && board.at(file + 1, rank + direction).get_color() != color) {
                moveGrid.at(file, rank).emplace_back(Move{file, rank, file + 1, rank + direction});
            }
        }
    }),
    std::make_shared<MoveGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Knight move
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == KNIGHT) {
            std::vector<std::pair<int, int>> knightMoves = {
                {1, 2}, {2, 1}, {2, -1}, {1, -2},
                {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
            };
            for (const auto& [df, dr] : knightMoves) {
                int newFile = file + df;
                int newRank = rank + dr;
                if (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (!board.has_piece(newFile, newRank)) {
                        moveGrid.at(newFile, newRank).emplace_back(Move{file, rank, newFile, newRank});
                    }
                }
            }
        }
    }),
    std::make_shared<AttackGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Knight attack
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == KNIGHT) {
            std::vector<std::pair<int, int>> knightMoves = {
                {1, 2}, {2, 1}, {2, -1}, {1, -2},
                {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
            };
            for (const auto& [df, dr] : knightMoves) {
                int newFile = file + df;
                int newRank = rank + dr;
                if (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (board.has_piece(newFile, newRank) && board.at(newFile, newRank).get_color() != board.at(file, rank).get_color()) {
                        moveGrid.at(newFile, newRank).emplace_back(Move{file, rank, newFile, newRank});
                    }
                }
            }
        }
    }),
    std::make_shared<MoveGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Bishop move
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == BISHOP) {
            std::vector<std::pair<int, int>> directions = {
                {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
            };
            for (const auto& [df, dr] : directions) {
                int newFile = file + df;
                int newRank = rank + dr;
                while (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (!board.has_piece(newFile, newRank)) {
                        moveGrid.at(newFile, newRank).emplace_back(Move{file, rank, newFile, newRank});
                    } else {
                        break;
                    }
                    newFile += df;
                    newRank += dr;
                }
            }
        }
    }),
    std::make_shared<AttackGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Bishop attack
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == BISHOP) {
            std::vector<std::pair<int, int>> directions = {
                {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
            };
            for (const auto& [df, dr] : directions) {
                int newFile = file + df;
                int newRank = rank + dr;
                while (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (board.has_piece(newFile, newRank)) {
                        if (board.at(newFile, newRank).get_color() != board.at(file, rank).get_color()) {
                            moveGrid.at(newFile, newRank).emplace_back(Move{file, rank, newFile, newRank});
                        }
                        break;
                    }
                    newFile += df;
                    newRank += dr;
                }
            }
        }
    }),
    std::make_shared<MoveGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Rook move
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == ROOK) {
            std::vector<std::pair<int, int>> directions = {
                {1, 0}, {-1, 0}, {0, 1}, {0, -1}
            };
            for (const auto& [df, dr] : directions) {
                int newFile = file + df;
                int newRank = rank + dr;
                while (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (!board.has_piece(newFile, newRank)) {
                        moveGrid.at(newFile, newRank).emplace_back(Move{file, rank, newFile, newRank});
                    } else {
                        break;
                    }
                    newFile += df;
                    newRank += dr;
                }
            }
        }
    }),
    std::make_shared<AttackGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Rook attack
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == ROOK) {
            std::vector<std::pair<int, int>> directions = {
                {1, 0}, {-1, 0}, {0, 1}, {0, -1}
            };
            for (const auto& [df, dr] : directions) {
                int newFile = file + df;
                int newRank = rank + dr;
                while (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (board.has_piece(newFile, newRank)) {
                        if (board.at(newFile, newRank).get_color() != board.at(file, rank).get_color()) {
                            moveGrid.at(newFile, newRank).emplace_back(Move{file, rank, newFile, newRank});
                        }
                        break;
                    }
                    newFile += df;
                    newRank += dr;
                }
            }
        }
    }),
    std::make_shared<MoveGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Queen move
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
                        moveGrid.at(newFile, newRank).emplace_back(Move{file, rank, newFile, newRank});
                    } else {
                        break;
                    }
                    newFile += df;
                    newRank += dr;
                }
            }
        }
    }),
    std::make_shared<AttackGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Queen attack
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == QUEEN) {
            std::vector<std::pair<int, int>> directions = {
                {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
            };
            for (const auto& [df, dr] : directions) {
                int newFile = file + df;
                int newRank = rank + dr;
                while (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (board.has_piece(newFile, newRank)) {
                        if (board.at(newFile, newRank).get_color() != board.at(file, rank).get_color()) {
                            moveGrid.at(newFile, newRank).emplace_back(Move{file, rank, newFile, newRank});
                        }
                        break;
                    }
                    newFile += df;
                    newRank += dr;
                }
            }
        }
    }),
    std::make_shared<MoveGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // King move
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == KING) {
            std::vector<std::pair<int, int>> kingMoves = {
                {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
            };
            for (const auto& [df, dr] : kingMoves) {
                int newFile = file + df;
                int newRank = rank + dr;
                if (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (!board.has_piece(newFile, newRank)) {
                        moveGrid.at(newFile, newRank).emplace_back(Move{file, rank, newFile, newRank});
                    }
                }
            }
        }
    }),
    std::make_shared<AttackGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // King attack
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == KING) {
            std::vector<std::pair<int, int>> kingMoves = {
                {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
            };
            for (const auto& [df, dr] : kingMoves) {
                int newFile = file + df;
                int newRank = rank + dr;
                if (newFile >= 0 && newFile < 8 && newRank >= 0 && newRank < 8) {
                    if (board.at(newFile, newRank).get_color() != board.at(file, rank).get_color()) {
                        moveGrid.at(newFile, newRank).emplace_back(Move{file, rank, newFile, newRank});
                    }
                }
            }
        }
    }),
    std::make_shared<MoveTransformer>([](ChessBoard& board, MoveGrid& moveGrid) { // Pawn promotion
        for (int file = 0; file < 8; file++) {
            for (int rank = 0; rank < 8; rank++) {
                for (auto& move : moveGrid.at(file, rank)) {
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
            }
        }
    }),
    std::make_shared<MoveGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // Castling move
        // FIXME: Add logic to detect when king/rooks have moved and to check for check conditions that would prevent castling
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == KING) {
            Color color = board.at(file, rank).get_color();
            int backRank = (color == WHITE) ? 0 : 7;
            // Kingside castling
            if (file == 4 && rank == backRank &&
                board.has_piece(7, backRank) && board.at(7, backRank).get_type() == ROOK &&
                !board.has_piece(5, backRank) && !board.has_piece(6, backRank)) {
                moveGrid.at(6, backRank).emplace_back(Move{4, backRank, 6, backRank});
                moveGrid.at(6, backRank).back().overwrite_apply([](const Move& move, ChessBoard& board) {
                    board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank);
                    // Move the rook as well
                    board.move_piece(7, move.fromRank, 5, move.fromRank);
                });
                moveGrid.at(6, backRank).back().overwrite_string_rep([](const Move& move, const ChessBoard& board) {
                    return "O-O";
                });
            }
            // Queenside castling
            if (file == 4 && rank == backRank &&
                board.has_piece(0, backRank) && board.at(0, backRank).get_type() == ROOK &&
                !board.has_piece(1, backRank) && !board.has_piece(2, backRank) && !board.has_piece(3, backRank)) {
                moveGrid.at(2, backRank).emplace_back(Move{4, backRank, 2, backRank});
                moveGrid.at(2, backRank).back().overwrite_apply([](const Move& move, ChessBoard& board) {
                    board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank);
                    // Move the rook as well
                    board.move_piece(0, move.fromRank, 3, move.fromRank);
                });
                moveGrid.at(2, backRank).back().overwrite_string_rep([](const Move& move, const ChessBoard& board) {
                    return "O-O-O";
                });
            }
        }
    }),
    std::make_shared<AttackGenerator>([](ChessBoard& board, MoveGrid& moveGrid, int file, int rank) { // En passant move
        // FIME: Add logic to only allow en passant on the turn immediately following the opponent's pawn moving two squares forward from its starting position
        if (board.has_piece(file, rank) && board.at(file, rank).get_type() == PAWN) {
            Color color = board.at(file, rank).get_color();
            if (rank == ((color == WHITE) ? 4 : 3)) { // En passant is only possible on the 5th rank for white and 4th rank for black
                int direction = (color == WHITE) ? 1 : -1;
                // Check for en passant capture to the left
                if (file > 0 && board.has_piece(file - 1, rank) && board.at(file - 1, rank).get_type() == PAWN && board.at(file - 1, rank).get_color() != color) {
                    moveGrid.at(file - 1, rank + direction).emplace_back(Move{file, rank, file - 1, rank + direction});
                    moveGrid.at(file - 1, rank + direction).back().overwrite_apply([direction](const Move& move, ChessBoard& board) {
                        board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank);
                        board.remove(move.toFile, move.toRank - direction);
                    });
                }
                // Check for en passant capture to the right
                if (file < 7 && board.has_piece(file + 1, rank) && board.at(file + 1, rank).get_type() == PAWN && board.at(file + 1, rank).get_color() != color) {
                    moveGrid.at(file + 1, rank + direction).emplace_back(Move{file, rank, file + 1, rank + direction});
                    moveGrid.at(file + 1, rank + direction).back().overwrite_apply([direction](const Move& move, ChessBoard& board) {
                        board.move_piece(move.fromFile, move.fromRank, move.toFile, move.toRank);
                        board.remove(move.toFile, move.toRank - direction);
                    });
                }
            }
        }
    }),
    std::make_shared<MoveRestrictor>([](ChessBoard& board, MoveGrid& moveGrid) { // Can't make moves that would put your own king in check
        int kingFile;
        int kingRank;
        bool foundKing = false;
        for (int file = 0; file < 8; file++) {
            for (int rank = 0; rank < 8; rank++) {
                if (board.has_piece(file, rank) && board.at(file, rank).get_type() == KING) {
                    kingFile = file;
                    kingRank = rank;
                    foundKing = true;
                    break;
                }
            }
            if (foundKing) { break; }
        }
        for (int moveGridFile = 0; moveGridFile < 8; moveGridFile++) {
            for (int moveGridRank = 0; moveGridRank < 8; moveGridRank++) {
                for (auto& move : moveGrid.at(moveGridFile, moveGridRank)) {
                    // Check to see if move would put the king in check
                    ChessBoard tempBoard = board;
                    move.execute(tempBoard);
                    Color kingColor = board.at(move.fromFile, move.fromRank).get_color();
                    bool kingInCheck = false;
                    // Iterate through the chess board and check each enemy piece to see if it can attack the king
                    for (int file = 0; file < 8; file++) {
                        for (int rank = 0; rank < 8; rank++) {
                            if (tempBoard.has_piece(file, rank) && tempBoard.at(file, rank).get_color() != kingColor) {
                                // Apply rules to see if this piece can attack the king's position on the temp board
                                MoveGrid tempMoveGrid;
                                // FIXME: iterate through total rules list instead of just basicRules
                                for (const auto& rule : basicRules.get_rules()) {
                                    if (auto attackGen = std::dynamic_pointer_cast<AttackGenerator>(rule)) {
                                        attackGen->get_attacks(tempBoard, tempMoveGrid, file, rank);
                                    }
                                    if (tempMoveGrid.at(kingFile, kingRank).size() > 0) {
                                        kingInCheck = true;
                                        break;
                                    }
                                }
                                if (kingInCheck) { break; }
                            }
                            if (kingInCheck) { break; }
                        }
                        if (kingInCheck) { break; }
                    }
                    if (kingInCheck) {
                        // moveGrid.at(moveGridFile, moveGridRank).erase(std::remove(moveGrid.at(moveGridFile, moveGridRank).begin(), moveGrid.at(moveGridFile, moveGridRank).end(), move), moveGrid.at(moveGridFile, moveGridRank).end());
                        // Above generated by Copilot, but below should be more efficient since it doesn't need to search through the vector twice
                        moveGrid.at(moveGridFile, moveGridRank).erase(std::find(moveGrid.at(moveGridFile, moveGridRank).begin(), moveGrid.at(moveGridFile, moveGridRank).end(), move));
                    }
                }
            }
        }
    })
});