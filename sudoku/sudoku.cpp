/*
================================================================================
Author: Bhagi Dhakal
Compile: g++ -o sudoku sudoku.cpp

g++ -std=c++20 sudoku.cpp -o sudoku
Description Sudoku game in the terminal, I will also make options to solve the
sudoku puzzle using algorithms like backtracking.

https://www.geeksforgeeks.org/top-50-cpp-project-ideas-for-beginners-advanced/
*/

#include <iostream>
#include <vector>
#include <cstdlib>

std::vector<std::vector<int>> allocate_2d_array(int rows, int cols)
{
	return std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));
}

void print_2d_array(const std::vector<std::vector<int>>& array)
{
	for (int i = 0; i < 9; i++)
	{
		if ((i) % 3 == 0)
		{
			std::cout << "\n";
		}
		for (int j = 0; j < 9; j++)
		{

			if (array[i][j] != 0)
			{

				std::cout << array[i][j] << "  ";
			}
			else
			{

				std::cout << ".  ";
			}

			if ((j + 1) % 3 == 0)
			{
				std::cout << "  ";
			}
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void printValidNumbers(std::vector<int> validList)
{
	for (int valid : validList)
	{
		std::cout << valid << " ";
	}
	std::cout << "\n"
		<< std::endl;
}

std::vector<int> getCommandInput()
{
	std::vector<int> command;
	int input;

	std::cout << "Please let me know what row, column and the number you want to try! \n";

	std::cout << "Row(1-9): ";
	std::cin >> input;
	while (input < 1 || input > 9)
	{
		std::cout << "Please give valid Row(1-9):";
		std::cin >> input;
	}
	command.push_back(input);

	std::cout << "Col(1-9): ";
	std::cin >> input;
	while (input < 1 || input > 9)
	{
		std::cout << "Please give valid Col(1-9): ";
		std::cin >> input;
	}
	command.push_back(input);

	std::cout << "Number(1-9): ";
	std::cin >> input;
	while (input < 1 || input > 9)
	{
		std::cout << "Please give valid Number(1-9): ";
		std::cin >> input;
	}

	command.push_back(input);

	return command;
}

void checkAnswer(std::vector<int> command, std::vector<std::vector<int>> puzzle)
{
	// first check if the board has number in the position or not
	if (puzzle[command[0] - 1][command[1] - 1] != 0)
	{
		std::cout << "You can not change number in this position, Row:" << command[0] << ", Col:" << command[1] << ".\n";
		print_2d_array(puzzle);
		return;
	}

	// also need to check answer before doing this
	puzzle[command[0] - 1][command[1] - 1] = command[2];
	print_2d_array(puzzle);

	return;
}

std::vector<int> validNumberList(std::vector<std::vector<int>> puzzle, int row, int col)
{
	std::vector<int> validList{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	// check row
	for (int i = 0; i < 9; i++)
	{
		if (puzzle[row][i] != 0)
		{
			// remove that number from our list.
			validList.erase(std::remove(validList.begin(), validList.end(), puzzle[row][i]), validList.end());
		}
	}

	// check col
	for (int i = 0; i < 9; i++)
	{
		if (puzzle[i][col] != 0)
		{
			// remove the number we find from our list.
			validList.erase(std::remove(validList.begin(), validList.end(), puzzle[i][col]), validList.end());
		}
	}

	// check box

	// this will return the box of the index blongs to, clever way
	int boxIndexRow = (row / 3) * 3;
	int boxIndexCol = (col / 3) * 3;

	std::cout << boxIndexRow << " c" << boxIndexCol << "\n";

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (puzzle[boxIndexRow + i][boxIndexCol + j] != 0)
			{
				validList.erase(std::remove(validList.begin(), validList.end(), puzzle[boxIndexRow + i][boxIndexCol + j]), validList.end());
			}
		}
	}

	return validList;
}

void solvePuzzle(std::vector<std::vector<int>> puzzle)
{

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			// std::cout << "Inside the solve puzzle \n";
			if (puzzle[i][j] == 0)
			{
				// std::cout << "Inside the IF  \n";
				//  get list of valid numbers
				std::vector<int> validNumbers = validNumberList(puzzle, i, j);
				print_2d_array(puzzle);
				// try out all the valid numbres.
				if (validNumbers.empty())
				{
					return;
				}
				else
				{
					printValidNumbers(validNumbers);
					for (int num : validNumbers)
					{
						puzzle[i][j] = num;
						solvePuzzle(puzzle);
					}
				}
			}
		}
	}
}

void get_command(std::vector<std::vector<int>> puzzle)
{
	char choice;
	std::cout << "What would you like to do? \n";

	std::cout << "Press C to create new game!\n";
	std::cout << "Press P to solve the puzzle!\n";

	std::cout << "Press S to get the Solution!\n";

	std::cout << "Press Q to quit the program! \n";

	std::cout << "Option: ";

	std::cin >> choice;
	std::cout << "\n";

	if (choice == 'c' || choice == 'C')
	{
		std::cout << "Created new puzzle!";
	}
	else if (choice == 'p' || choice == 'P')
	{
		std::cout << "Call add function!\n";
		std::vector<int> addCommand = getCommandInput();

		checkAnswer(addCommand, puzzle);
	}
	else if (choice == 'q' || choice == 'Q')
	{
		std::cout << "No Progress will be saved! Goodbye! \n";
		std::exit(0);
	}
	else if (choice == 's' || choice == 'S')
	{
		std::cout << "Call solve Function! \n";
		solvePuzzle(puzzle);
	}
	else
	{
		std::cout << "Invalid option. Try again.\n";
	}
}

int main()
{

	std::cout << "Sudoku Game \n";

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

	print_2d_array(puzzle);

	while (true)
	{
		get_command(puzzle);
	}

	return 0;
}
