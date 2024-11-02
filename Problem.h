#pragma once

#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <bitset>
#include <string>
#include <memory>

class Problem
{
private:
	
public:
	virtual bool matches(const Problem* state) const = 0;

	virtual std::vector<std::unique_ptr<Problem>> getReachableNodes() const = 0;

	virtual std::bitset<150> getHash() const = 0;

	virtual std::string toString() const = 0;

	virtual int computeHeuristic(const Problem* goal) const {
		return 0;
	}
};

#endif
