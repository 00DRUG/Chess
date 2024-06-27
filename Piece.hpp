#ifndef PIECE_HPP
#define PIECE_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Piece {
public:
    enum class Color { White, Black };

    Piece(Color color);
    virtual ~Piece() {}
    virtual void draw(sf::RenderWindow& window, int x, int y) = 0;
    virtual std::vector<sf::Vector2i> getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) = 0;
    Color getColor() const;

protected:
    Color color;
    sf::Texture texture;
    sf::Sprite sprite;
    bool isValidMove(const std::vector<std::vector<Piece*>>& board, int newX, int newY);
    bool isValidCapture(const std::vector<std::vector<Piece*>>& board, int newX, int newY);
};

class Pawn : public Piece {
public:
    Pawn(Color color);
    void draw(sf::RenderWindow& window, int x, int y) override;
    std::vector<sf::Vector2i> getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) override;
};

class Rook : public Piece {
public:
    Rook(Color color);
    void draw(sf::RenderWindow& window, int x, int y) override;
    std::vector<sf::Vector2i> getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) override;
    bool hasMoved;
};

class Knight : public Piece {
public:
    Knight(Color color);
    void draw(sf::RenderWindow& window, int x, int y) override;
    std::vector<sf::Vector2i> getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) override;
};

class Bishop : public Piece {
public:
    Bishop(Color color);
    void draw(sf::RenderWindow& window, int x, int y) override;
    std::vector<sf::Vector2i> getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) override;
};

class Queen : public Piece {
public:
    Queen(Color color);
    void draw(sf::RenderWindow& window, int x, int y) override;
    std::vector<sf::Vector2i> getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) override;
};

class King : public Piece {
public:
    King(Color color);
    void draw(sf::RenderWindow& window, int x, int y) override;
    std::vector<sf::Vector2i> getValidMoves(const std::vector<std::vector<Piece*>>& board, int x, int y) override;

private:
    bool hasMoved;
    bool canCastle(const std::vector<std::vector<Piece*>>& board, int x, int y, int rookX, int rookY);
    friend class ChessBoard;
};
#endif // PIECE_HPP
