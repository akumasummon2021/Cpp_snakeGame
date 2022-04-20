#ifndef ASTART_H
#define ASTART_H

#include <memory>
#include <vector>
#include "stone.h"
#include "SDL.h"
#include "snake.h"
#include <algorithm>

class Node {
	public: 
		Node(int kx, int ky, std::shared_ptr<Node> fatherPtr);
		void setFather(std::shared_ptr<Node> fatherPtr); 
		std::shared_ptr<Node> getFather(); 
		
		int getX();
		int getY();
		
		float gValue = 0;
		float hValue = 0;
		float calcDistance(Node &node);
		float calcDistance(int targetX, int targetY);
		float calcDistance(SDL_Point &point);		
		bool operator==(Node &a) const;
		bool operator>(Node &a) const;
		bool operator<(Node &a) const;
	private:
		std::shared_ptr<Node> father;	
		int x;
		int y;
};

class Astar {
	public: 
		//enum class MapSymbol { msPath = -1, msRoad, msStone, msEnemies};
		Astar(int grid_width, int grid_height);
		void placeStoneOnMap(std::vector<Stone> const &stones);
		void placeEnemiesOnMap(std::vector<Snake> &snakes);
		// to change all enemies direction
		void AstarAlgorithmen(std::vector<Snake> &enemies);
		void updateEnemiesDirection(std::vector<Snake> &snakes);
		bool findPath(Node &start, SDL_Point goal);
		void sortOpenSet();	
		void Debug();
		void Debug2(std::vector<Node> &node);
		void Debug3(std::vector<SDL_Point> path);
		void findNoPathToDo(Snake &enemy, Node &headNode);	 
	private:
		std::vector<std::vector<int>> GameMap;
		std::vector<std::vector<int>> AstartTemp;
		std::vector<std::vector<int>> GameMap_init;
		int grid_width;
		int grid_height;
		std::vector<Node> AstarOpenSet;
		std::vector<Node> AstarCloseSet;
		std::vector<Node> path;
};

#endif