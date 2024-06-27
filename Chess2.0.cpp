#include <SFML/Graphics.hpp>
#include "ChessBoard.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Game");

    ChessBoard board;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            board.handleEvent(event);
        }

        window.clear();
        board.draw(window);
        window.display();
    }

    return 0;
}

#ifdef _WIN32
#include <Windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main();
}
#endif
