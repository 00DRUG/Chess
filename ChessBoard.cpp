#include "ChessBoard.hpp"
#include <iostream>
#include <SFML/Window.hpp>

ChessBoard::ChessBoard() {
    initBoard();
    pieceSelected = false;
    currentTurn = Piece::Color::White;

}

ChessBoard::~ChessBoard() {
    for (auto& row : board) {
        for (auto& piece : row) {
            delete piece;
        }
    }
}

void ChessBoard::initBoard() {
    // Очистка 
    for (auto& row : board) {
        for (auto& piece : row) {
            delete piece;
            piece = nullptr;
        }
    }

    board.clear();
    board.resize(8, std::vector<Piece*>(8, nullptr));

    for (int i = 0; i < 8; ++i) {
        board[1][i] = new Pawn(Piece::Color::Black);
        board[6][i] = new Pawn(Piece::Color::White);
    }

    board[0][0] = new Rook(Piece::Color::Black);
    board[0][7] = new Rook(Piece::Color::Black);
    board[7][0] = new Rook(Piece::Color::White);
    board[7][7] = new Rook(Piece::Color::White);

    board[0][1] = new Knight(Piece::Color::Black);
    board[0][6] = new Knight(Piece::Color::Black);
    board[7][1] = new Knight(Piece::Color::White);
    board[7][6] = new Knight(Piece::Color::White);

    board[0][2] = new Bishop(Piece::Color::Black);
    board[0][5] = new Bishop(Piece::Color::Black);
    board[7][2] = new Bishop(Piece::Color::White);
    board[7][5] = new Bishop(Piece::Color::White);

    board[0][3] = new Queen(Piece::Color::Black);
    board[7][3] = new Queen(Piece::Color::White);

    board[0][4] = new King(Piece::Color::Black);
    board[7][4] = new King(Piece::Color::White);
}

void ChessBoard::draw(sf::RenderWindow& window) {
    drawBoard(window);
    drawPieces(window);
    if (pieceSelected) {
        drawHints(window);
    }
}

void ChessBoard::handleEvent(const sf::Event& event) {
    static Piece::Color currentPlayer = Piece::Color::White;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        int x = event.mouseButton.x / 100;
        int y = event.mouseButton.y / 100;
        if (pieceSelected) {
            auto validMoves = getValidMoves(selectedPiece.x, selectedPiece.y);
            validMoves.erase(std::remove_if(validMoves.begin(), validMoves.end(), [this](sf::Vector2i move) {
                return !willMovePreventCheck(selectedPiece.x, selectedPiece.y, move.x, move.y, board[selectedPiece.y][selectedPiece.x]->getColor());
                }), validMoves.end());
            sf::Vector2i target(x, y);
            if (std::find(validMoves.begin(), validMoves.end(), target) != validMoves.end()) {
                if (dynamic_cast<King*>(board[selectedPiece.y][selectedPiece.x])) {
                    if (target.x == selectedPiece.x - 2) { 
                        std::swap(board[selectedPiece.y][selectedPiece.x], board[target.y][target.x]);
                        std::swap(board[target.y][0], board[target.y][target.x + 1]);
                        dynamic_cast<King*>(board[target.y][target.x])->hasMoved = true;
                        dynamic_cast<Rook*>(board[target.y][target.x + 1])->hasMoved = true;
                    }
                    else if (target.x == selectedPiece.x + 2) {
                        std::swap(board[selectedPiece.y][selectedPiece.x], board[target.y][target.x]);
                        std::swap(board[target.y][7], board[target.y][target.x - 1]);
                        dynamic_cast<King*>(board[target.y][target.x])->hasMoved = true;
                        dynamic_cast<Rook*>(board[target.y][target.x - 1])->hasMoved = true;
                    }
                    else { 
                        if (board[target.y][target.x] != nullptr) {
                            delete board[target.y][target.x]; 
                        }
                        board[target.y][target.x] = board[selectedPiece.y][selectedPiece.x];
                        board[selectedPiece.y][selectedPiece.x] = nullptr;
                        dynamic_cast<King*>(board[target.y][target.x])->hasMoved = true;
                    }
                }
                else {
                    if (board[y][x] != nullptr) {
                        delete board[y][x];
                        board[y][x] = nullptr;
                    }
                    std::swap(board[selectedPiece.y][selectedPiece.x], board[y][x]);
                    promotePawnIfNecessary(y, x);
                }

                pieceSelected = false;
                moveHints.clear();
                captureHints.clear();

                if (isCheckmate(Piece::Color::White)) {
                    handleCheckmate(Piece::Color::Black);
                }
                else if (isCheckmate(Piece::Color::Black)) {
                    handleCheckmate(Piece::Color::White);
                }

                currentPlayer = (currentPlayer == Piece::Color::White) ? Piece::Color::Black : Piece::Color::White;
            }
            else {
                pieceSelected = false;
                moveHints.clear();
                captureHints.clear();
            }
        }
        else {
            if (board[y][x] != nullptr && board[y][x]->getColor() == currentPlayer) {
                selectedPiece = sf::Vector2i(x, y);
                pieceSelected = true;
                auto validMoves = getValidMoves(x, y);
                highlightValidMoves(validMoves);
            }
        }
    }
}





void ChessBoard::promotePawnIfNecessary(int y, int x) {
    if (dynamic_cast<Pawn*>(board[y][x]) && ((board[y][x]->getColor() == Piece::Color::White && y == 0) ||
        (board[y][x]->getColor() == Piece::Color::Black && y == 7))) {
        Piece* promotedPiece = choosePromotion(board[y][x]->getColor());
        delete board[y][x];
        board[y][x] = promotedPiece;
    }
}

Piece* ChessBoard::choosePromotion(Piece::Color color) {
    return showPromotionDialog(color);
}

void ChessBoard::drawBoard(sf::RenderWindow& window) {
    sf::RectangleShape square(sf::Vector2f(100, 100));
    bool white = true;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((i + j) % 2 == 0)
                square.setFillColor(sf::Color::White);
            else
                square.setFillColor(sf::Color::Black);

            square.setPosition(i * 100, j * 100);
            window.draw(square);
        }
    }
} 

void ChessBoard::drawPieces(sf::RenderWindow& window) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (board[i][j] != nullptr) {
                board[i][j]->draw(window, j, i);
            }
        }
    }
}

void ChessBoard::drawHints(sf::RenderWindow& window) {
    for (const auto& hint : moveHints) {
        window.draw(hint);
    }
    for (const auto& hint : captureHints) {
        window.draw(hint);
    }
}


std::vector<sf::Vector2i> ChessBoard::getValidMoves(int x, int y) {
    std::vector<sf::Vector2i> validMoves;
    if (board[y][x] != nullptr) {
        auto potentialMoves = board[y][x]->getValidMoves(board, x, y);
        for (const auto& move : potentialMoves) {
            if (!wouldBeInCheck(board[y][x], x, y, move.x, move.y)) {
                validMoves.push_back(move);
            }
        }
    }
    return validMoves;
}

sf::Vector2i ChessBoard::findKing(Piece::Color color) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (board[y][x] != nullptr && board[y][x]->getColor() == color && dynamic_cast<King*>(board[y][x]) != nullptr) {
                return sf::Vector2i(x, y);
            }
        }
    }
    return sf::Vector2i(-1, -1); // FULL ERROR
}

void ChessBoard::highlightValidMoves(const std::vector<sf::Vector2i>& moves) {
    moveHints.clear();
    captureHints.clear();
    for (const auto& move : moves) {
        sf::CircleShape hint(15); 
        hint.setOrigin(15, 15); 
        hint.setPosition(move.x * 100 + 50, move.y * 100 + 50);
        if (board[move.y][move.x] == nullptr) {
            hint.setFillColor(sf::Color::Green);
            moveHints.push_back(hint);
        }
        else {
            hint.setFillColor(sf::Color::Red);
            captureHints.push_back(hint);
        }
    }
}


bool ChessBoard::isInCheck(Piece::Color color) {
    sf::Vector2i kingPos;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (board[y][x] != nullptr && board[y][x]->getColor() == color && dynamic_cast<King*>(board[y][x]) != nullptr) {
                kingPos = sf::Vector2i(x, y);
                break;
            }
        }
    }

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (board[y][x] != nullptr && board[y][x]->getColor() != color) {
                auto moves = board[y][x]->getValidMoves(board, x, y);
                if (std::find(moves.begin(), moves.end(), kingPos) != moves.end()) {
                    return true;
                }
            }
        }
    }

    return false;
}


Piece* ChessBoard::showPromotionDialog(Piece::Color color) {
    sf::RenderWindow promotionWindow(sf::VideoMode(500, 200), "Choose Promotion");
    sf::Texture queenTexture, rookTexture, bishopTexture, knightTexture;
    queenTexture.loadFromFile((color == Piece::Color::White) ? "figures/white-queen.png" : "figures/black-queen.png");
    rookTexture.loadFromFile((color == Piece::Color::White) ? "figures/white-rook.png" : "figures/black-rook.png");
    bishopTexture.loadFromFile((color == Piece::Color::White) ? "figures/white-bishop.png" : "figures/black-bishop.png");
    knightTexture.loadFromFile((color == Piece::Color::White) ? "figures/white-knight.png" : "figures/black-knight.png");

    sf::Sprite queenSprite(queenTexture), rookSprite(rookTexture), bishopSprite(bishopTexture), knightSprite(knightTexture);
    queenSprite.setPosition(50, 50);
    rookSprite.setPosition(150, 50);
    bishopSprite.setPosition(250, 50);
    knightSprite.setPosition(350, 50);

    queenSprite.setOrigin(queenSprite.getLocalBounds().width / 2, queenSprite.getLocalBounds().height / 2);
    rookSprite.setOrigin(rookSprite.getLocalBounds().width / 2, rookSprite.getLocalBounds().height / 2);
    bishopSprite.setOrigin(bishopSprite.getLocalBounds().width / 2, bishopSprite.getLocalBounds().height / 2);
    knightSprite.setOrigin(knightSprite.getLocalBounds().width / 2, knightSprite.getLocalBounds().height / 2);

    while (promotionWindow.isOpen()) {
        sf::Event event;
        while (promotionWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                promotionWindow.close();
                return new Queen(color);
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i clickPos = sf::Mouse::getPosition(promotionWindow);
                if (queenSprite.getGlobalBounds().contains(clickPos.x, clickPos.y)) {
                    promotionWindow.close();
                    return new Queen(color);
                }
                else if (rookSprite.getGlobalBounds().contains(clickPos.x, clickPos.y)) {
                    promotionWindow.close();
                    return new Rook(color);
                }
                else if (bishopSprite.getGlobalBounds().contains(clickPos.x, clickPos.y)) {
                    promotionWindow.close();
                    return new Bishop(color);
                }
                else if (knightSprite.getGlobalBounds().contains(clickPos.x, clickPos.y)) {
                    promotionWindow.close();
                    return new Knight(color);
                }
            }
        }
        promotionWindow.clear(sf::Color::White);
        promotionWindow.draw(queenSprite);
        promotionWindow.draw(rookSprite);
        promotionWindow.draw(bishopSprite);
        promotionWindow.draw(knightSprite);
        promotionWindow.display();
    }
    return new Queen(color);
}

bool ChessBoard::wouldBeInCheck(Piece* movingPiece, int fromX, int fromY, int toX, int toY) {
    Piece* capturedPiece = board[toY][toX];
    std::swap(board[fromY][fromX], board[toY][toX]);
    board[fromY][fromX] = nullptr;

    bool inCheck = isInCheck(movingPiece->getColor());

    std::swap(board[fromY][fromX], board[toY][toX]);
    board[toY][toX] = capturedPiece;

    return inCheck;
}
bool ChessBoard::willMovePreventCheck(int startX, int startY, int endX, int endY, Piece::Color color) {
    Piece* originalDestPiece = board[endY][endX];
    board[endY][endX] = board[startY][startX];
    board[startY][startX] = nullptr;
    bool inCheck = isInCheck(color);
    board[startY][startX] = board[endY][endX];
    board[endY][endX] = originalDestPiece;
    return !inCheck;
}

bool ChessBoard::isCheckmate(Piece::Color color) {
    if (!isInCheck(color)) {
        return false;
    }
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (board[y][x] != nullptr && board[y][x]->getColor() == color) {
                auto validMoves = getValidMoves(x, y);
                if (!validMoves.empty()) {
                    return false;
                }
            }
        }
    }
    return true;
}
void ChessBoard::handleCheckmate(Piece::Color winningColor) {
    std::string winner = (winningColor == Piece::Color::White) ? "White" : "Black";

    sf::RenderWindow alertWindow(sf::VideoMode(300, 150), "Checkmate");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font.\n";
    }

    sf::Text text("Checkmate! " + winner + " wins!", font, 20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(20, 20);

    sf::RectangleShape button(sf::Vector2f(100, 50));
    button.setFillColor(sf::Color::Blue);
    button.setPosition(100, 80);

    sf::Text buttonText("Restart", font, 20);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(115, 90);

    while (alertWindow.isOpen()) {
        sf::Event event;
        while (alertWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                alertWindow.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                if (button.getGlobalBounds().contains(mouseX, mouseY)) {
                    alertWindow.close();
                    initBoard();
                }
            }
        }
        alertWindow.clear(sf::Color::White);
        alertWindow.draw(text);
        alertWindow.draw(button);
        alertWindow.draw(buttonText);
        alertWindow.display();
    }
}

