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

