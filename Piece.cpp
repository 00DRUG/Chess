#include "Piece.hpp"
#include <iostream>
#include <filesystem>
#include <algorithm>

// Piece implementation
Piece::Piece(Color color) : color(color) {}

Piece::Color Piece::getColor() const {
    return color;
}

// Helper function to set the sprite's scale and origin for centering
void setSpriteProperties(sf::Sprite& sprite) {
    sprite.setScale(0.75f, 0.75f); // Adjust the scale if needed
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
}

bool Piece::isValidMove(const std::vector<std::vector<Piece*>>& board, int newX, int newY) {
    return newX >= 0 && newX < 8 && newY >= 0 && newY < 8 && board[newY][newX] == nullptr;
}

bool Piece::isValidCapture(const std::vector<std::vector<Piece*>>& board, int newX, int newY) {
    return newX >= 0 && newX < 8 && newY >= 0 && newY < 8 &&
        board[newY][newX] != nullptr && board[newY][newX]->getColor() != color;
}

// Pawn implementation
Pawn::Pawn(Color color) : Piece(color) {
    std::string relativePath = (color == Color::White) ? "figures/white-pawn.png" : "figures/black-pawn.png";

    if (!texture.loadFromFile(relativePath)) {
        std::cerr << "Failed to load image \"" << relativePath << "\". Please check the file path.\n";
    }
    else {
        std::cerr << "Successfully loaded image \"" << relativePath << "\".\n";
    }
    sprite.setTexture(texture);
    setSpriteProperties(sprite);
}

void Pawn::draw(sf::RenderWindow& window, int x, int y) {
    sprite.setPosition(static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50)); // Center the piece
    window.draw(sprite);
}

std::vector<sf::Vector2i> Pawn::getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) {
    std::vector<sf::Vector2i> moves;
    int direction = (color == Color::White) ? -1 : 1;
    int startRow = (color == Color::White) ? 6 : 1;

    if (isValidMove(board, x, y + direction)) {
        moves.push_back(sf::Vector2i(x, y + direction));
        if (y == startRow && isValidMove(board, x, y + 2 * direction)) {
            moves.push_back(sf::Vector2i(x, y + 2 * direction));
        }
    }

    // Captures
    if (isValidCapture(board, x - 1, y + direction)) {
        moves.push_back(sf::Vector2i(x - 1, y + direction));
    }
    if (isValidCapture(board, x + 1, y + direction)) {
        moves.push_back(sf::Vector2i(x + 1, y + direction));
    }

    return moves;
}

// Rook implementation
Rook::Rook(Color color) : Piece(color) {
    std::string relativePath = (color == Color::White) ? "figures/white-rook.png" : "figures/black-rook.png";

    if (!texture.loadFromFile(relativePath)) {
        std::cerr << "Failed to load image \"" << relativePath << "\". Please check the file path.\n";
    }
    else {
        std::cerr << "Successfully loaded image \"" << relativePath << "\".\n";
    }
    sprite.setTexture(texture);
    setSpriteProperties(sprite);
}

void Rook::draw(sf::RenderWindow& window, int x, int y) {
    sprite.setPosition(static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50)); // Center the piece
    window.draw(sprite);
}

std::vector<sf::Vector2i> Rook::getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) {
    std::vector<sf::Vector2i> moves;

    // Horizontal and vertical moves
    for (int i = x + 1; i < 8; ++i) {
        if (isValidMove(board, i, y)) {
            moves.push_back(sf::Vector2i(i, y));
        }
        else {
            if (isValidCapture(board, i, y)) {
                moves.push_back(sf::Vector2i(i, y));
            }
            break;
        }
    }
    for (int i = x - 1; i >= 0; --i) {
        if (isValidMove(board, i, y)) {
            moves.push_back(sf::Vector2i(i, y));
        }
        else {
            if (isValidCapture(board, i, y)) {
                moves.push_back(sf::Vector2i(i, y));
            }
            break;
        }
    }
    for (int i = y + 1; i < 8; ++i) {
        if (isValidMove(board, x, i)) {
            moves.push_back(sf::Vector2i(x, i));
        }
        else {
            if (isValidCapture(board, x, i)) {
                moves.push_back(sf::Vector2i(x, i));
            }
            break;
        }
    }
    for (int i = y - 1; i >= 0; --i) {
        if (isValidMove(board, x, i)) {
            moves.push_back(sf::Vector2i(x, i));
        }
        else {
            if (isValidCapture(board, x, i)) {
                moves.push_back(sf::Vector2i(x, i));
            }
            break;
        }
    }
    return moves;
}

// Knight implementation
Knight::Knight(Color color) : Piece(color) {
    std::string relativePath = (color == Color::White) ? "figures/white-knight.png" : "figures/black-knight.png";

    if (!texture.loadFromFile(relativePath)) {
        std::cerr << "Failed to load image \"" << relativePath << "\". Please check the file path.\n";
    }
    else {
        std::cerr << "Successfully loaded image \"" << relativePath << "\".\n";
    }
    sprite.setTexture(texture);
    setSpriteProperties(sprite);
}

void Knight::draw(sf::RenderWindow& window, int x, int y) {
    sprite.setPosition(static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50)); // Center the piece
    window.draw(sprite);
}

std::vector<sf::Vector2i> Knight::getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) {
    std::vector<sf::Vector2i> moves;
    std::vector<sf::Vector2i> offsets = {
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}
    };

    for (const auto& offset : offsets) {
        int newX = x + offset.x;
        int newY = y + offset.y;
        if (isValidMove(board, newX, newY) || isValidCapture(board, newX, newY)) {
            moves.push_back(sf::Vector2i(newX, newY));
        }
    }
    return moves;
}

// Bishop implementation
Bishop::Bishop(Color color) : Piece(color) {
    std::string relativePath = (color == Color::White) ? "figures/white-bishop.png" : "figures/black-bishop.png";

    if (!texture.loadFromFile(relativePath)) {
        std::cerr << "Failed to load image \"" << relativePath << "\". Please check the file path.\n";
    }
    else {
        std::cerr << "Successfully loaded image \"" << relativePath << "\".\n";
    }
    sprite.setTexture(texture);
    setSpriteProperties(sprite);
}

void Bishop::draw(sf::RenderWindow& window, int x, int y) {
    sprite.setPosition(static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50)); // Center the piece
    window.draw(sprite);
}

std::vector<sf::Vector2i> Bishop::getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) {
    std::vector<sf::Vector2i> moves;

    // Diagonal moves
    for (int i = 1; x + i < 8 && y + i < 8; ++i) {
        if (isValidMove(board, x + i, y + i)) {
            moves.push_back(sf::Vector2i(x + i, y + i));
        }
        else {
            if (isValidCapture(board, x + i, y + i)) {
                moves.push_back(sf::Vector2i(x + i, y + i));
            }
            break;
        }
    }
    for (int i = 1; x - i >= 0 && y + i < 8; ++i) {
        if (isValidMove(board, x - i, y + i)) {
            moves.push_back(sf::Vector2i(x - i, y + i));
        }
        else {
            if (isValidCapture(board, x - i, y + i)) {
                moves.push_back(sf::Vector2i(x - i, y + i));
            }
            break;
        }
    }
    for (int i = 1; x + i < 8 && y - i >= 0; ++i) {
        if (isValidMove(board, x + i, y - i)) {
            moves.push_back(sf::Vector2i(x + i, y - i));
        }
        else {
            if (isValidCapture(board, x + i, y - i)) {
                moves.push_back(sf::Vector2i(x + i, y - i));
            }
            break;
        }
    }
    for (int i = 1; x - i >= 0 && y - i >= 0; ++i) {
        if (isValidMove(board, x - i, y - i)) {
            moves.push_back(sf::Vector2i(x - i, y - i));
        }
        else {
            if (isValidCapture(board, x - i, y - i)) {
                moves.push_back(sf::Vector2i(x - i, y - i));
            }
            break;
        }
    }
    return moves;
}

// Queen implementation
Queen::Queen(Color color) : Piece(color) {
    std::string relativePath = (color == Color::White) ? "figures/white-queen.png" : "figures/black-queen.png";

    if (!texture.loadFromFile(relativePath)) {
        std::cerr << "Failed to load image \"" << relativePath << "\". Please check the file path.\n";
    }
    else {
        std::cerr << "Successfully loaded image \"" << relativePath << "\".\n";
    }
    sprite.setTexture(texture);
    setSpriteProperties(sprite);
}

void Queen::draw(sf::RenderWindow& window, int x, int y) {
    sprite.setPosition(static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50)); // Center the piece
    window.draw(sprite);
}

std::vector<sf::Vector2i> Queen::getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) {
    std::vector<sf::Vector2i> moves;

    // Horizontal and vertical moves (like Rook)
    for (int i = x + 1; i < 8; ++i) {
        if (isValidMove(board, i, y)) {
            moves.push_back(sf::Vector2i(i, y));
        }
        else {
            if (isValidCapture(board, i, y)) {
                moves.push_back(sf::Vector2i(i, y));
            }
            break;
        }
    }
    for (int i = x - 1; i >= 0; --i) {
        if (isValidMove(board, i, y)) {
            moves.push_back(sf::Vector2i(i, y));
        }
        else {
            if (isValidCapture(board, i, y)) {
                moves.push_back(sf::Vector2i(i, y));
            }
            break;
        }
    }
    for (int i = y + 1; i < 8; ++i) {
        if (isValidMove(board, x, i)) {
            moves.push_back(sf::Vector2i(x, i));
        }
        else {
            if (isValidCapture(board, x, i)) {
                moves.push_back(sf::Vector2i(x, i));
            }
            break;
        }
    }
    for (int i = y - 1; i >= 0; --i) {
        if (isValidMove(board, x, i)) {
            moves.push_back(sf::Vector2i(x, i));
        }
        else {
            if (isValidCapture(board, x, i)) {
                moves.push_back(sf::Vector2i(x, i));
            }
            break;
        }
    }

    // Diagonal moves (like Bishop)
    for (int i = 1; x + i < 8 && y + i < 8; ++i) {
        if (isValidMove(board, x + i, y + i)) {
            moves.push_back(sf::Vector2i(x + i, y + i));
        }
        else {
            if (isValidCapture(board, x + i, y + i)) {
                moves.push_back(sf::Vector2i(x + i, y + i));
            }
            break;
        }
    }
    for (int i = 1; x - i >= 0 && y + i < 8; ++i) {
        if (isValidMove(board, x - i, y + i)) {
            moves.push_back(sf::Vector2i(x - i, y + i));
        }
        else {
            if (isValidCapture(board, x - i, y + i)) {
                moves.push_back(sf::Vector2i(x - i, y + i));
            }
            break;
        }
    }
    for (int i = 1; x + i < 8 && y - i >= 0; ++i) {
        if (isValidMove(board, x + i, y - i)) {
            moves.push_back(sf::Vector2i(x + i, y - i));
        }
        else {
            if (isValidCapture(board, x + i, y - i)) {
                moves.push_back(sf::Vector2i(x + i, y - i));
            }
            break;
        }
    }
    for (int i = 1; x - i >= 0 && y - i >= 0; ++i) {
        if (isValidMove(board, x - i, y - i)) {
            moves.push_back(sf::Vector2i(x - i, y - i));
        }
        else {
            if (isValidCapture(board, x - i, y - i)) {
                moves.push_back(sf::Vector2i(x - i, y - i));
            }
            break;
        }
    }

    return moves;
}

// King implementation
King::King(Color color) : Piece(color), hasMoved(false) {
    std::string relativePath = (color == Color::White) ? "figures/white-king.png" : "figures/black-king.png";

    if (!texture.loadFromFile(relativePath)) {
        std::cerr << "Failed to load image \"" << relativePath << "\". Please check the file path.\n";
    }
    else {
        std::cerr << "Successfully loaded image \"" << relativePath << "\".\n";
    }
    sprite.setTexture(texture);
    setSpriteProperties(sprite);
}

void King::draw(sf::RenderWindow& window, int x, int y) {
    sprite.setPosition(static_cast<float>(x * 100 + 50), static_cast<float>(y * 100 + 50));
    window.draw(sprite);
}

std::vector<sf::Vector2i> King::getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) {
    std::vector<sf::Vector2i> moves;
    std::vector<sf::Vector2i> offsets = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    for (const auto& offset : offsets) {
        int newX = x + offset.x;
        int newY = y + offset.y;
        if (isValidMove(board, newX, newY) || isValidCapture(board, newX, newY)) {
            moves.push_back(sf::Vector2i(newX, newY));
        }
    }

    // Castling
    if (!hasMoved) {
        if (canCastle(board, x, y, 0, y)) { // Queen-side castling
            moves.push_back(sf::Vector2i(x - 2, y));
        }
        if (canCastle(board, x, y, 7, y)) { // King-side castling
            moves.push_back(sf::Vector2i(x + 2, y));
        }
    }

    return moves;
}

bool King::canCastle(const std::vector<std::vector<Piece*>>& board, int x, int y, int rookX, int rookY) {
    if (rookX == 0 && dynamic_cast<Rook*>(board[rookY][rookX]) && !dynamic_cast<Rook*>(board[rookY][rookX])->hasMoved) {
        for (int i = x - 1; i > 0; --i) {
            if (board[y][i] != nullptr) return false;
        }
        return true;
    }
    if (rookX == 7 && dynamic_cast<Rook*>(board[rookY][rookX]) && !dynamic_cast<Rook*>(board[rookY][rookX])->hasMoved) {
        for (int i = x + 1; i < 7; ++i) {
            if (board[y][i] != nullptr) return false;
        }
        return true;
    }
    return false;
}
