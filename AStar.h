#pragma once

#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include "Problem.h"
#include "ProblemTrace.h"

class AStar
{
public:
	struct Inner {
		std::string board;
		int g;
		int h;
	};
	struct Data {
		std::vector<Inner> solution;
		std::size_t max;
		std::size_t explored;
	};
public:
	Data process(std::unique_ptr<Problem> initial, std::unique_ptr<Problem> goal) {
		std::vector<ProblemTrace> trace;
		std::size_t maxQueueSize{ };
		std::priority_queue<ProblemTrace, std::vector<ProblemTrace>, ProblemTraceComparator> queue;
		std::unordered_map<std::bitset<150>, bool> explored;
		std::list<ProblemTrace> nodes;
		queue.emplace(nullptr, std::move(initial), 0, 0);
		std::cout << "Expanding state " << '\n';
		std::cout << queue.top().getProblemState()->toString() << '\n';
		bool first{ true };
		while (!queue.empty()) {
			maxQueueSize = std::max(maxQueueSize, queue.size());
			auto& top{ queue.top() };
			auto hash{ top.getProblemState()->getHash() };
			if (explored[hash]) {
				queue.pop();
				continue;
			}
			if (!first) {
				std::cout << "The best state to expand with g(n) = " << top.getCost() << " and h(n) = " << top.getHeuristicCost() << " is...\n";
				std::cout << top.getProblemState()->toString() << " Expanding this node...\n";
			}
			first = false;
			if (top.getProblemState()->matches(goal.get())) {
				std::vector<Inner> solution;

				for (auto p{ &top }; p != nullptr; p = p->getPrevious()) {
					solution.push_back({ p->getProblemState()->toString(), p->getCost(), p->getHeuristicCost() });
				}
				std::reverse(solution.begin(), solution.end());
				return { solution, maxQueueSize, explored.size() };
			}
			explored[hash] = true;

			auto reachable{ top.getProblemState()->getReachableNodes() };
			auto uniformCost{ top.getCost() + 1 };
			auto residual{ top.getProblemStateInstance() };
			auto prev{ top.getPrevious() };

			nodes.push_back(ProblemTrace(prev, std::move(residual), top.getCost(), top.getHeuristicCost()));
			queue.pop();
			std::size_t size{ queue.size() };
			for (auto& candidate : reachable) {
				auto heuristic{ candidate->computeHeuristic(goal.get()) };
				queue.emplace(&nodes.back(), std::move(candidate), uniformCost, heuristic);
			}
		}

		return { std::vector<Inner>{}, maxQueueSize, explored.size() };
	}
};

#endif
