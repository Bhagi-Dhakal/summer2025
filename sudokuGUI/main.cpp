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
bool GameScreen = false;
bool LevelScreen = false;

int level = 0;

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

int main() {


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



    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }

            if (PLAY.isClicked(event, window)) {
                PlayScreen = false;
                LevelScreen = true;
            }

            if (EASY.isClicked(event, window)) {
                level = 0;

                LevelScreen = false;
                GameScreen = true;

            }

            if (MEDIUM.isClicked(event, window)) {

                level = 1;
                LevelScreen = false;
                GameScreen = true;


            }
            if (HARD.isClicked(event, window)) {
                level = 2;
                LevelScreen = false;
                GameScreen = true;


            }

            if (EXPERT.isClicked(event, window)) {
                level = 3;
                LevelScreen = false;
                GameScreen = true;


            }
        }

        if (PlayScreen) {
            drawPlayScreen(window, font);

            PLAY.render(window);
            PLAY.update(window);

            // window.update();

        }

        if (LevelScreen) {
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



        window.display();
    }


    return 1;
}