#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <utility>
#include "NPuzzle.h"
#include "AStar.h"

template<typename T>
void forceInput(std::istream& in, T& value) {
	while (!(in >> value)) {
		std::cout << "Bad input, please re-enter: ";
		in.clear();
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

std::unique_ptr<NPuzzle> buildPuzzle(std::size_t dimensions, NPuzzle::HeuristicFunction fn) {
	std::vector<bool> used;
	used.resize(dimensions*dimensions, false);
	std::vector<std::size_t> board;
	
	std::cout << "\nBuild your puzzle entering 0 to " << used.size() - 1 << ", where zero is the blank space.\n";

	for (std::size_t row{ 0 }; row < dimensions; ++row) {
		std::cout << "Enter ROW " << row << ", with whitespace between numbers: ";

		for (std::size_t col{ 0 }; col < dimensions; ++col) {
			std::size_t piece;
			do {
				forceInput(std::cin, piece);
			} while ((piece >= used.size() || used[piece]) && (std::cout << "Invalid piece " << piece << ", please re - enter\n"));
			used[piece] = true;
			board.push_back(piece);
		}
	}

	return std::make_unique<NPuzzle>( board, dimensions, fn );
}

std::unique_ptr<NPuzzle> defaultPuzzle(std::size_t dimensions, NPuzzle::HeuristicFunction fn) {
	if (dimensions == 3) {
		return std::make_unique<NPuzzle>(std::vector<std::uint8_t>{{1, 0, 3, 4, 2, 6, 7, 5, 8}}, 3, fn);
		//return std::make_unique<NPuzzle>(std::vector<std::uint8_t>{{1, 2, 3, 4, 5, 6, 7, 8, 0}}, 3, fn);
	}
	else if (dimensions == 4) {
		return std::make_unique<NPuzzle>(std::vector<std::uint8_t>{{1, 0, 3, 4, 2, 6, 7, 5, 8}}, 3, fn);

	}
	else {
		return std::make_unique<NPuzzle>(std::vector<std::uint8_t>{{1, 0, 3, 4, 2, 6, 7, 5, 8}}, 3, fn);

	}
}

std::unique_ptr<NPuzzle> randomPuzzle(std::size_t dimensions, NPuzzle::HeuristicFunction fn) {
	std::vector<std::size_t> pieces;
	pieces.resize(dimensions * dimensions);

	std::random_device rd{};
	std::mt19937 mt{ rd() };
	for (std::size_t i{}; i < pieces.size(); ++i) {
		pieces[i] = i;
	}
	std::shuffle(std::begin(pieces), std::end(pieces), mt);
	return std::make_unique<NPuzzle>( pieces, dimensions, fn );
}

std::unique_ptr<NPuzzle> goalState(std::size_t dimensions, NPuzzle::HeuristicFunction fn) {
	std::vector<std::size_t> pieces;
	pieces.resize(dimensions * dimensions);
	for (std::size_t i{}; i < pieces.size(); ++i) {
		pieces[i] = (i + 1) % pieces.size();
	}
	return std::make_unique<NPuzzle>( pieces, dimensions, fn );
}
int main() {
	char response;
	do {
		std::cout << "Welcome to the N-Puzzle solver!\n";
		std::cout << "Author: Joshua Moreno (862269121)\n";

		std::size_t dimensions;
		std::cout << "Would you like a 3x3, 4x4, or 5x5 puzzle?\n";
		std::cout << "Please enter the size for the puzzles length and width: ";
		do {
			forceInput(std::cin, dimensions);
		} while ((dimensions < 3 || dimensions > 5) && (std::cout << "Invalid dimension, please re-enter: "));

		char choice;
		std::cout << "Enter \'1\' for a default puzzle, \'2\' to build your own, and \'3\' for a random layout: ";
		do {
			std::cin >> choice;
		} while (choice != '1' && choice != '2' && choice != '3' && (std::cout << "Invalid choice, re-enter: "));

		char algo;
		std::cout << "Enter your choice of algorithm:\n";
		std::cout << "1. Uniform Cost Search\n";
		std::cout << "2. A* w/ Misplaced Tile Heuristic\n";
		std::cout << "3. A* w/ Euclidean Distance Heuristic\n";
		do {
			std::cin >> algo;
		} while (algo != '1' && algo != '2' && algo != '3' && (std::cout << "Invalid choice, re-enter: "));

		auto heuristic{ [&]() -> NPuzzle::HeuristicFunction {
			if (algo == '1') {
				return [](const NPuzzle& lp, const NPuzzle& rp) {
					return 0;
				};
			}
			else if (algo == '2') {
				return [](const NPuzzle& lp, const NPuzzle& rp) -> int {
					int h{};

					for (std::size_t i{}; i < lp.getBoard().size(); ++i) {
						for (std::size_t j{}; j < lp.getBoard()[i].size(); ++j) {
							if (lp.getBoard()[i][j] != rp.getBoard()[i][j]) {
								++h;
							}
						}
					}
					return h;
				};
			}
			else {
				return [](const NPuzzle& lp, const NPuzzle& rp) -> int {
					double h{};

					std::unordered_map<std::size_t, std::pair<int, int>> map;
					for (int i{}; i < rp.getBoard().size(); ++i) {
						for (int j{}; j < rp.getBoard()[i].size(); ++j) {
							map[rp.getBoard()[i][j]] = std::pair{ i, j };
						}
					}
					for (int i{}; i < lp.getBoard().size(); ++i) {
						for (int j{}; j < lp.getBoard()[i].size(); ++j) {
							const auto [x, y] { map[lp.getBoard()[i][j]] };
							const auto dist{ std::sqrt((i - x) * (i - x) + (j - y) * (j - y)) };
							h += dist;
						}
					}
					
					return static_cast<int>(h);
				};
			}
		} () };

		auto puzzle{ [&]() {
			if (choice == '1') {
				return defaultPuzzle(dimensions, heuristic);
			}
			else if (choice == '2') {
				return buildPuzzle(dimensions, heuristic);
			}
			else {
				return randomPuzzle(dimensions, heuristic);
			}
		}() };

		std::cout << "The NPuzzle:\n" << puzzle->toString() << "will be processed.\n";
		AStar astar;
	
		auto trace{ astar.process(std::move(puzzle), goalState(dimensions, heuristic)) };

		if (trace.solution.empty()) {
			std::cout << "There is no solution for this puzzle, please try another!\n";
		}
		else {
			for (auto& node : trace.solution) {
				std::cout << "Expanding state\n";
				std::cout << node.board << '\n';
				std::cout << "The best state to expand with g(n) = " << node.g << " and h(n) = " << node.h << " is...\n";
			}
			std::cout << "\nTo solve this problem the search algorithm expanded a total of " << trace.explored << " nodes.";
			std::cout << "\nThe maxmimum number of nodes in the queue at any one time: " << trace.max;
			std::cout << "\nThe depth of the goal node was " << trace.solution.size() - 1 << '\n';
		}
		std::cout << "Please enter \'Y\' or \'y\'to repeat the program: ";
	} while ((std::cin >> response) && toupper(response) == 'Y');

}