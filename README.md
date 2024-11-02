**Problems**

For this project I worked on it by myself. Thus I encountered many problems.

First, I had difficulty designing the project because I had to design everything by myself. In particular, designing a generic, abstract, and extensible problem state class for the AStar search algorithm was difficult.

Nonetheless, the solution was developed through the use of virtual methods and inheritance.

Second, I had difficulty tracing the solution path, once a solution was found. The solution was to keep track of all explored problem state nodes in a list and have each problem state node have a previous pointer.

Third, I had difficulty managing memory and abstract class instances. Additionally, creating and managing the next reachable nodes was a struggle. 

The interface portion was not so difficult.

**Design**
My design consists of classes ProblemTrace, Problem, AStar, and NPuzzle. 

ProblemTrace is more of a node esque class that keep tracks of the current problem state and has methods pertaining to the heuristic cost, uniform cost, and the previous state instances.

Problem is an abstract class to represent any arbtirary problem state. In particular, it a match method, getReachableNodes method, getHash method, toString method, and computeHeuristic method. Respectively, matching method is used for comparing a state to a goal, getHash is to compute a unique hash such that exploration can be tracked, toString converts a state to a readable means, computeHeuristic estimates the cost from a base node to a goal node.

NPuzzle inherits from Problem class and keeps track of the Puzzle board through a 2D vector member variable named board. It also overrides all the methods from the roblem class.

**Optimization**

I did attempt to optimize this problem through the use of a priority queue data type which has O(n log n) insertion and removal. Likewise, I used an unordered map and bitset to track all explored nodes in a more memory efficient and time efficient manner.

Other optimizations could be deleting traces for dead nodes, but I didn't have to implement this. This wuld help with memory storage.

**Findings**

I found that the Euclidean distance heuristic performed the best out of the three. From the lecture we know that the Manhattan Distance heuristic is an admissible, Euclidean distance is a smaller estimate so it is also admissible. However, compared to the Misplaced Tile Heuristic it is a better estimate since it doesn't just merely count misplaced tiles as a single point. The uniform cost search heuristic/algorithm was the worst, which makes sense since it has no means of determining truly how close a node is to the goal state.
