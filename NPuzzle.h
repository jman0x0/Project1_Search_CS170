#pragma once

#ifndef NPUZZLE_H
#define NPUZZLE_H

#include <cassert>
#include <functional>
#include "Problem.h"

class NPuzzle : public Problem
{
public:
	using HeuristicFunction = std::function<int(const NPuzzle&, const NPuzzle&)>;
private:
	std::vector<std::vector<std::uint8_t>> board;
	HeuristicFunction heuristic;
public:
	template<typename T>
	NPuzzle(const T& slots, std::size_t dimensions, HeuristicFunction heuristic) {
		assert(slots.size() == dimensions * dimensions);

		board.resize(dimensions);
		for (std::size_t i{}; i != slots.size(); ++i) {
			board[i / dimensions].push_back(slots[i]);
		}
		this->heuristic = heuristic;
	}

	NPuzzle(std::vector<std::vector<std::uint8_t>> board, HeuristicFunction heuristic)
	: board(board),
	heuristic(heuristic) {

	}

	const std::vector<std::vector<std::uint8_t>>& getBoard() const {
		return board;
	}

	virtual bool matches(const Problem* problem) const override {
		return getHash() == problem->getHash();
	}

	virtual std::vector<std::unique_ptr<Problem>> getReachableNodes() const override {
		std::vector<std::unique_ptr<Problem>> reachable;

		auto [br, bc] { findBlank() };
		for (auto [dr, dc] : { std::pair{-1, 0}, std::pair{1, 0}, std::pair{0, -1}, std::pair{0, 1} }) {
			if (br + dr < board.size() && bc + dc < board[br + dr].size()) {
				auto newBoard{ board };
				std::swap(newBoard[br][bc], newBoard[br + dr][bc + dc]);
				reachable.push_back(std::make_unique<NPuzzle>(std::move(newBoard), heuristic));
			}
		}
		return reachable;
	}

	virtual std::bitset<150> getHash() const override {
		std::bitset<150> hash;

		std::size_t i{};
		for (auto& row : board) {
			for (auto& col : row) {
				for (std::size_t s{}; s != 6; ++s) {
					hash[i++ % hash.size()] = (col >> s) & 1;
				}
			}
		}
		return hash;
	}



	virtual std::string toString() const override {
		std::string output;
		output.reserve(board.size() * board.size() * 3);

		for (auto& row : board) {
			for (auto& col : row) {
				output += std::to_string(col);
				output += ' ';
			}
			output += '\n';
		}

		return output;
	}

	virtual int computeHeuristic(const Problem* goal) const override {
		auto* other{ dynamic_cast<const NPuzzle*>(goal) };
		
		if (other) {
			return heuristic(*this, *other);
		}
	}
private:
	std::pair<std::size_t, std::size_t> findBlank() const {
		for (std::size_t i{}; i != board.size(); ++i) {
			for (std::size_t j{}; j != board[i].size(); ++j) {
				if (board[i][j] == 0) {
					return { i,j };
				}
			}
		}
		assert(false && "BAD BOARD");
	}
};

#endif
