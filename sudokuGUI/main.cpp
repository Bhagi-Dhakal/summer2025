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




int main() {


    sf::RenderWindow window(sf::VideoMode(1000, 600), "Sudoku", sf::Style::Close);

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }
    }



    return 1;
}