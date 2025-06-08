/*
    Will create a sodoku, puzzle GUI using SFML.
    Will add the ablity to solve the puzzle, added numbers generate
    new puzzles. Plan is to use back tracking algo to solve the puzzles.

    File Name: Main.cpp
    Compile: Use makefile.....
*/

/* Includes */
#include "../tools/Button.h"
#include <SFML/Graphics.hpp>
#include <iostream>

/* Global Vars*/
int SCREENW = 800;
int SCREENH = 750;

bool PlayScreen = true;
bool LevelScreen = false;
bool GameScreen = false;

int level = 0;
std::vector<int> selectedCell{ -1,-1 };
int cellSize = 76;

/* Screens */
void drawPlayScreen(sf::RenderWindow& window, sf::Font font) {
    window.clear(sf::Color(33, 52, 72));

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(75);
    text.setString("SODOKU");
    text.setPosition(250, 50);

    window.draw(text);
}

void drawLevelScreen(sf::RenderWindow& window, sf::Font font) {
    window.clear(sf::Color(33, 52, 72));

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(30);
    text.setString("Select a difficulty level: ");
    text.setPosition(240, 250);

    window.draw(text);
}

std::vector<int> getRowCol(int x, int y) {
    const int cellSize = 76;
    const int widthOffset = 58;
    const int heightOffset = 8;

    int row = (y - heightOffset) / cellSize;
    int col = (x - widthOffset) / cellSize;

    return std::vector<int> {row, col};
}


// Lots of over lap here for this function... I can improve this but later! 
void highlightSingle(sf::RenderWindow& window, sf::Color color) {

    sf::RectangleShape highlight(sf::Vector2f(cellSize - 10, cellSize - 10));
    highlight.setFillColor(sf::Color::Transparent);
    highlight.setOutlineColor(color);
    highlight.setOutlineThickness(3);
    highlight.setPosition(58 + 5 + selectedCell[1] * cellSize, 8 + 5 + selectedCell[0] * cellSize);

    window.draw(highlight);

}

void highlightSingleRC(sf::RenderWindow& window, sf::Color color, int row, int col) {

    sf::RectangleShape highlight(sf::Vector2f(cellSize - 10, cellSize - 10));
    highlight.setFillColor(sf::Color::Transparent);
    highlight.setOutlineColor(color);
    highlight.setOutlineThickness(3);
    highlight.setPosition(58 + 5 + col * cellSize, 8 + 5 + row * cellSize);

    window.draw(highlight);

}

void highlightRow(sf::RenderWindow& window, sf::Color color, std::vector<std::vector<int>> puzzle) {

    for (int i = 0; i < 9; ++i) {
        if (i == selectedCell[1]) continue;// skip the selected(for dif color)
        if (puzzle[selectedCell[0]][i]) {
            sf::RectangleShape highlight(sf::Vector2f(cellSize - 10, cellSize - 10));
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineColor(color);
            highlight.setOutlineThickness(3);
            highlight.setPosition(58 + 5 + i * cellSize, 8 + 5 + selectedCell[0] * cellSize);

            window.draw(highlight);
        }
    }
}

void highlightCol(sf::RenderWindow& window, sf::Color color, std::vector<std::vector<int>> puzzle) {

    for (int i = 0; i < 9; ++i) {
        if (i == selectedCell[0]) continue;// skip the selected(for dif color)
        if (puzzle[i][selectedCell[1]]) {
            sf::RectangleShape highlight(sf::Vector2f(cellSize - 10, cellSize - 10));
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineColor(color);
            highlight.setOutlineThickness(3);
            highlight.setPosition(58 + 5 + selectedCell[1] * cellSize, 8 + 5 + i * cellSize);

            window.draw(highlight);
        }
    }
}


void drawNumbers(std::vector<std::vector<int>> puzzle, sf::Font font, sf::RenderWindow& window) {
    // Lets make the grid now
    const int cellSize = 76;
    const int widthOffset = 58;
    const int heightOffset = 8;

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(35);

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int num = puzzle[i][j];

            if (num == 0) continue;

            text.setString(std::to_string(num));
            // sets the test origin to center.
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);

            // Compute cell center 
            float x = widthOffset + cellSize * j + cellSize / 2.0f;
            float y = heightOffset + cellSize * i + cellSize / 2.0f;

            text.setPosition(x, y);
            window.draw(text);
        }
    }
}

void drawGameScreen(sf::RenderWindow& window, sf::Font font, std::vector<std::vector<int>> puzzle) {
    window.clear(sf::Color(33, 52, 72));

    // Lets make the grid now
    int heightOffset = 8;
    int widthOffset = 58;

    // verticle lines 
    for (int i = 0; i < 10; ++i) {
        if (i % 3 != 0) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(widthOffset + 76 * i, heightOffset)),
                sf::Vertex(sf::Vector2f(widthOffset + 76 * i, 700 - heightOffset))
            };
            line[0].color = sf::Color(0, 0, 0);
            line[1].color = sf::Color(0, 0, 0);
            window.draw(line, 2, sf::Lines);
        }
        else {
            sf::RectangleShape line(sf::Vector2f(3, 684));
            line.setPosition(widthOffset - 3 + i * 76, heightOffset);
            line.setFillColor(sf::Color(0, 0, 0));

            window.draw(line);
        }
    }

    // Horizontal lines 
    for (int i = 0; i < 10; ++i) {
        if (i % 3 != 0) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(widthOffset, heightOffset + 76 * i)),
                sf::Vertex(sf::Vector2f(800 - widthOffset , heightOffset + 76 * i))
            };
            line[0].color = sf::Color(0, 0, 0);
            line[1].color = sf::Color(0, 0, 0);
            window.draw(line, 2, sf::Lines);
        }
        else {
            sf::RectangleShape line(sf::Vector2f(684 + 3, 3));
            line.setPosition(widthOffset - 3, heightOffset - 3 + i * 76);
            line.setFillColor(sf::Color(0, 0, 0));

            window.draw(line);
        }
    }

    if (selectedCell[0] != -1 && selectedCell[1] != -1) {
        if (!puzzle[selectedCell[0]][selectedCell[1]]) {
            // selecting empty space
            highlightSingle(window, sf::Color(255, 50, 50));
            highlightRow(window, sf::Color(100, 30, 30), puzzle);
            highlightCol(window, sf::Color(100, 30, 30), puzzle);
        }
        else {
            // selecting non empty space! 
            int num = puzzle[selectedCell[0]][selectedCell[1]];
            for (int i = 0; i < 9; ++i) {
                for (int j = 0; j < 9; ++j) {
                    if (puzzle[i][j] == num) {
                        highlightSingleRC(window, sf::Color(0, 255, 0), i, j);
                    }
                }
            }

        }
    }

    drawNumbers(puzzle, font, window);
}


int main() {

    std::vector<std::vector<int>> puzzle = {
        {0, 0, 0, 4, 0, 0, 0, 9, 2},
        {0, 0, 9, 7, 0, 3, 0, 0, 0},
        {0, 6, 0, 0, 0, 0, 0, 0, 4},
        {0, 1, 3, 0, 0, 0, 0, 0, 0},
        {2, 0, 7, 8, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 2, 0, 5, 0},
        {8, 0, 0, 0, 5, 0, 0, 4, 0},
        {7, 0, 0, 0, 9, 0, 1, 0, 8},
        {0, 0, 0, 0, 0, 4, 0, 0, 0} };

    sf::RenderWindow window(sf::VideoMode(SCREENW, SCREENH), "Sudoku", sf::Style::Close);


    sf::Font font;
    if (!font.loadFromFile("../tools/Quicksand-Medium.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // Buttons 
    Button PLAY({ 200, 50 }, { 300 , 475 }, "PLAY", font);

    // Level buttions
    Button EASY({ 300, 50 }, { 250 , 300 }, "EASY", font);
    EASY.setColors(sf::Color(122, 245, 122), sf::Color(120, 255, 120), sf::Color(70, 210, 70));

    Button MEDIUM({ 300, 50 }, { 250 , 360 }, "MEDIUM", font);
    MEDIUM.setColors(sf::Color(245, 224, 20), sf::Color(255, 220, 60), sf::Color(200, 150, 0));

    Button HARD({ 300, 50 }, { 250 , 420 }, "HARD", font);
    HARD.setColors(sf::Color(235, 122, 20), sf::Color(255, 130, 30), sf::Color(180, 70, 0));

    Button EXPERT({ 300, 50 }, { 250 , 480 }, "EXPERT", font);
    EXPERT.setColors(sf::Color(214, 20, 20), sf::Color(230, 0, 0), sf::Color(150, 0, 0));

    Button SOLVE({ 150, 30 }, { 600 , 715 }, "SOLVE", font);
    SOLVE.setColors(sf::Color(214, 20, 20), sf::Color(230, 0, 0), sf::Color(150, 0, 0));


    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

            if (PlayScreen && PLAY.isClicked(event, window)) {
                PlayScreen = false;
                LevelScreen = true;
                continue;
            }

            if (LevelScreen && EASY.isClicked(event, window)) {
                level = 0;

                LevelScreen = false;
                GameScreen = true;
                continue;
            }

            if (LevelScreen && MEDIUM.isClicked(event, window)) {

                level = 1;
                LevelScreen = false;
                GameScreen = true;
                continue;
            }
            if (LevelScreen && HARD.isClicked(event, window)) {
                level = 2;
                LevelScreen = false;
                GameScreen = true;
                continue;
            }

            if (LevelScreen && EXPERT.isClicked(event, window)) {
                level = 3;
                LevelScreen = false;
                GameScreen = true;
                continue;
            }

            if (GameScreen && SOLVE.isClicked(event, window)) {
                std::cout << "Solve Clicked";
            }


            sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
            if (GameScreen &&
                (mousePosWindow.x > 58 && mousePosWindow.x < 742) &&
                (mousePosWindow.y > 8 && mousePosWindow.y < 692)) {

                if (event.type == sf::Event::MouseButtonPressed) {
                    selectedCell = getRowCol(mousePosWindow.x, mousePosWindow.y);
                }
            }

            // Not a fan of all this nesting... 
            if (selectedCell[0] != -1 && selectedCell[1] != -1) {
                if (GameScreen && event.type == sf::Event::KeyPressed) {
                    if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
                        int value = event.key.code - sf::Keyboard::Num0;
                        //std::cout << "You pressed: " << value << std::endl;
                        if (puzzle[selectedCell[0]][selectedCell[1]] == 0) {
                            puzzle[selectedCell[0]][selectedCell[1]] = value;
                        }
                    }
                }
            }

        }

        if (PlayScreen) {
            drawPlayScreen(window, font);

            PLAY.render(window);
            PLAY.update(window);

            // window.update();

        }
        else if (LevelScreen) {
            drawLevelScreen(window, font);

            EASY.render(window);
            MEDIUM.render(window);
            HARD.render(window);
            EXPERT.render(window);

            EASY.update(window);
            MEDIUM.update(window);
            HARD.update(window);
            EXPERT.update(window);

        }
        else if (GameScreen) {
            drawGameScreen(window, font, puzzle);

            SOLVE.render(window);
            SOLVE.update(window);

        }



        window.display();
    }


    return 1;
}