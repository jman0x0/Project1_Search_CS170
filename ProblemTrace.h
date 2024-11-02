#pragma once

#ifndef PROBLEM_TRACE_H
#define PROBLEM_TRACE_H

#include <memory>
#include "Problem.h"

class ProblemTrace
{
private:
	const ProblemTrace* previous;
	mutable std::unique_ptr<Problem> problem;
	int cost;
	int heuristic;
public:
	ProblemTrace(const ProblemTrace* previous, std::unique_ptr<Problem> problem, int cost, int heuristic)
		: previous(previous),
		problem(std::move(problem)),
		cost(cost),
		heuristic(heuristic) {

	}

	ProblemTrace(ProblemTrace&&) = default;
	ProblemTrace& operator=(ProblemTrace&&) = default;

	const ProblemTrace* getPrevious() const {
		return previous;
	}

	std::unique_ptr<Problem> getProblemStateInstance() const {
		return std::move(problem);
	}

	const Problem* getProblemState() const {
		return problem.get();
	}

	int getCost() const {
		return cost;
	}

	int getHeuristicCost() const {
		return heuristic;
	}

	int getTotalCost() const {
		return cost + heuristic;
	}
};

struct ProblemTraceComparator {
	bool operator()(const ProblemTrace& lhs, const ProblemTrace& rhs) const {
		return lhs.getTotalCost() > rhs.getTotalCost();
	}
};


#endif

