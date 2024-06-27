#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Piece.hpp"

class ChessBoard {
public:
    ChessBoard();
    ~ChessBoard();
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);
    void initBoard();
    bool isInCheck(Piece::Color color);
    bool willMovePreventCheck(int startX, int startY, int endX, int endY, Piece::Color color);
    sf::Vector2i findKing(Piece::Color color);
private:
    Piece::Color currentTurn;
    std::vector<std::vector<Piece*>> board;
    bool pieceSelected;
    sf::Vector2i selectedPiece;
    std::vector<sf::CircleShape> moveHints;
    std::vector<sf::CircleShape> captureHints;

    void drawBoard(sf::RenderWindow& window);
    void drawPieces(sf::RenderWindow& window);
    void drawHints(sf::RenderWindow& window);
    std::vector<sf::Vector2i> getValidMoves(int x, int y);
    void highlightValidMoves(const std::vector<sf::Vector2i>& moves);
    void promotePawnIfNecessary(int y, int x);
    Piece* choosePromotion(Piece::Color color);

    bool wouldBeInCheck(Piece* movingPiece, int fromX, int fromY, int toX, int toY);
    bool isCheckmate(Piece::Color color);
    void handleCheckmate(Piece::Color winningColor);

    Piece* showPromotionDialog(Piece::Color color);
};

#endif // CHESSBOARD_HPP
