#include <iostream>
#include "Astar.h"

Node::Node(int kx, int ky, std::shared_ptr<Node> fatherPtr) {
	x = kx; 
	y = ky; 
	father = fatherPtr;
}

void Node::setFather(std::shared_ptr<Node> fatherPtr) {father = fatherPtr;}
std::shared_ptr<Node> Node::getFather() {return father;}
int Node::getX() {return x;}
int Node::getY() {return y;}

Astar::Astar(int grid_width, int grid_height): grid_width{grid_width}, grid_height{grid_height}{
	// initialize
	std::vector<std::vector<int>> tmp(grid_height, std::vector<int>(grid_width, 0));
	GameMap = tmp;
	GameMap_init = tmp;
}

void Astar::placeStoneOnMap(std::vector<Stone> const &stones){
	
	for(int i=0;i<stones.size();++i){
		GameMap[stones[i]._kordinate.y][stones[i]._kordinate.x] = 1;	
		GameMap_init[stones[i]._kordinate.y][stones[i]._kordinate.x] = 1;
	}

}

void Astar::setEnemyDirektion(std::vector<Snake> &snake){
	
}