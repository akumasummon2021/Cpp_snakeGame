#ifndef ASTART_H
#define ASTART_H

#include <memory>
#include <vector>
#include "stone.h"
#include "snake.h"

class Node {
	public: 
		Node(int kx, int ky, std::shared_ptr<Node> fatherPtr);
		void setFather(std::shared_ptr<Node> fatherPtr); 
		std::shared_ptr<Node> getFather(); 
		
		int getX();
		int getY();
	private:
		std::shared_ptr<Node> father;		
		int x;
		int y;
};

class Astar {
	public: 
		Astar(int grid_width, int grid_height);
		void placeStoneOnMap(std::vector<Stone> const &stones);
		void setEnemyDirektion(std::vector<Snake> &snake);
	private:
		std::vector<std::vector<int>> GameMap;
		std::vector<std::vector<int>> GameMap_init;
		int grid_width;
		int grid_height;

};

#endif