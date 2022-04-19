#include <iostream>
#include "Astar.h"
#include "snake.h"
#include <cmath>

Node::Node(int kx, int ky, std::shared_ptr<Node> fatherPtr) {
	x = kx; 
	y = ky; 
	father = fatherPtr;
}

void Node::setFather(std::shared_ptr<Node> fatherPtr) {father = fatherPtr;}
std::shared_ptr<Node> Node::getFather() {return father;}
int Node::getX() {return x;}
int Node::getY() {return y;}

float Node::calcDistance(Node &node){
	int sum1 = (x-node.getX())*(x-node.getX());
	int sum2 = (y-node.getY())*(y-node.getY());
	return sqrt(sum1 + sum2);
}

float Node::calcDistance(SDL_Point &point){
	int sum1 = (x-point.x)*(x-point.x);
	int sum2 = (y-point.y)*(y-point.y);
	return sqrt(sum1 + sum2);
}

float Node::calcDistance(int targetX, int targetY){
	int sum1 = (x-targetX)*(x-targetX);
	int sum2 = (y-targetY)*(y-targetY);
	return sqrt(sum1 + sum2);
}

bool Node::operator==(Node &a) const{
	return ((x == a.getX()) && (y == a.getY()));
}

bool Node::operator>(Node &a) const{
	return (gValue + hValue)>(a.gValue + a.hValue);
}

bool Node::operator<(Node &a) const{
	return (gValue + hValue)<(a.gValue + a.hValue);
}






Astar::Astar(int grid_width, int grid_height): grid_width{grid_width}, grid_height{grid_height}{
	// initialize
	std::vector<std::vector<int>> tmp(grid_height, std::vector<int>(grid_width, 0));
	
	// in Map, 0 = road, 1 = stone, 2 = enemies, -1 = actuell enemy path visited
	GameMap = tmp;
	GameMap_init = tmp;
}

void Astar::placeStoneOnMap(std::vector<Stone> const &stones){
	
	for(int i=0;i<stones.size();++i){
		GameMap[stones[i]._kordinate.y][stones[i]._kordinate.x] = 1;	
		GameMap_init[stones[i]._kordinate.y][stones[i]._kordinate.x] = 1;
	}

}

// updateEnemiesDirection without A*
/*
void Astar::updateEnemiesDirection(std::vector<Snake> &enemies){
	for(int i=0;i<enemies.size();++i){
		int hx = static_cast<int>(enemies[i].head_x);
		int hy = static_cast<int>(enemies[i].head_y);
		
		if((hx-enemies[i].goal.x > 0) && (enemies[i].direction != Snake::Direction::kRight) && (hy != enemies[i].old_y)){
			//std::cout<<"x diff > 0: "<<(head_x-foods[index].x)<<" Direction: "<<static_cast<std::underlying_type<Direction>::type>(direction)<<std::endl;
			enemies[i].old_x = hx;
			enemies[i].direction = Snake::Direction::kLeft;
		}
		else if((hy-enemies[i].goal.y > 0) && (enemies[i].direction != Snake::Direction::kDown) && (hx != enemies[i].old_x)){
			//std::cout<<"y diff > 0: "<<(head_y-foods[index].y)<<" Direction: "<<static_cast<std::underlying_type<Direction>::type>(direction)<<std::endl;
			enemies[i].old_y = hy;		
			enemies[i].direction = Snake::Direction::kUp;
		}
		else if((hx-enemies[i].goal.x < 0) && (enemies[i].direction != Snake::Direction::kLeft) && (hy != enemies[i].old_y)){
			//std::cout<<"x diff < 0: "<<(head_x-foods[index].x)<<" Direction: "<<static_cast<std::underlying_type<Direction>::type>(direction)<<std::endl;
			enemies[i].old_x = hx;
			enemies[i].direction = Snake::Direction::kRight;
		}
		else if((hy-enemies[i].goal.y < 0) && (enemies[i].direction != Snake::Direction::kUp) && (hx != enemies[i].old_x)){
			//std::cout<<"y diff < 0: "<<(head_y-foods[index].y)<<" Direction: "<<static_cast<std::underlying_type<Direction>::type>(direction)<<std::endl;
			enemies[i].old_y = hy;
			enemies[i].direction = Snake::Direction::kDown;
		}		
	}
}*/

// updateEnemiesDirection with A*
void Astar::updateEnemiesDirection(std::vector<Snake> &enemies){
	for(int i=0;i<enemies.size();++i){
		int hx = static_cast<int>(enemies[i].head_x);
		int hy = static_cast<int>(enemies[i].head_y);
		/* // for debug
		if(hx-enemies[i].goal.x > 0){
			enemies[i].direction = Snake::Direction::kLeft;
		}
		else if(hy-enemies[i].goal.y > 0){	
			enemies[i].direction = Snake::Direction::kUp;
		}
		else if(hx-enemies[i].goal.x < 0){;
			enemies[i].direction = Snake::Direction::kRight;
		}
		else if(hy-enemies[i].goal.y < 0){
			enemies[i].direction = Snake::Direction::kDown;
		}*/
		std::cout<<"head(x,y): ("<<hx<<", "<<hy<<")"<<std::endl;
		std::cout<<"enemies(x,y): ("<<enemies[i].path.back().x<<", "<<enemies[i].path.back().y<<")"<<std::endl;
		if(hx-enemies[i].path.back().x > 0){
			enemies[i].direction = Snake::Direction::kLeft;
		}
		else if(hy-enemies[i].path.back().y > 0){	
			enemies[i].direction = Snake::Direction::kUp;
		}
		else if(hx-enemies[i].path.back().x < 0){;
			enemies[i].direction = Snake::Direction::kRight;
		}
		else if(hy-enemies[i].path.back().y < 0){
			enemies[i].direction = Snake::Direction::kDown;
		}
	}
}

void Astar::placeEnemiesOnMap(std::vector<Snake> &enemies){
	for(int i=0;i<enemies.size();++i){
		int sx = static_cast<int>(enemies[i].head_x);
		int sy = static_cast<int>(enemies[i].head_y);
		GameMap[sy][sx] = 2;
		for(int j=0;j<enemies[i].body.size();++j){
			sx = static_cast<int>(enemies[i].body[j].x);
			sy = static_cast<int>(enemies[i].body[j].y);
			GameMap[sy][sx] = 2;
		}
	}
}


//only one enemy:
void Astar::AstarAlgorithmen(std::vector<Snake> &enemies){
	std::cout<<"[AstarAlgorithmen]: begin"<<std::endl;
	for(int i=0;i<enemies.size();++i){
		// reset Map
		if(!AstarOpenSet.empty()) {AstarOpenSet.clear();}
		if(!AstarCloseSet.empty()) {AstarCloseSet.clear();}
		if(!path.empty()) {path.clear();}
		GameMap = GameMap_init;	
		placeEnemiesOnMap(enemies);	

		// Astar:
		// at first add head-Node into openset
		Node head(static_cast<int>(enemies[i].head_x), static_cast<int>(enemies[i].head_y), nullptr);
		head.gValue = head.calcDistance(enemies[i].goal);		
		AstarOpenSet.push_back(head);
				
		// find neigbours untile find the goal
		std::cout<<"Head(x,y): ("<<head.getX()<<", "<<head.getY()<<")"<<std::endl;
		std::cout<<"Goal(x,y): ("<<enemies[i].goal.x<<", "<<enemies[i].goal.y<<")"<<std::endl;
		bool bl_reachGoal = findPath(head, enemies[i].goal);
		// std::cout<<"to find the path"<<std::endl;	
		while(!bl_reachGoal){
			sortOpenSet();
			bl_reachGoal = findPath(head, enemies[i].goal);
		}
		
		while(path.back().getFather()!=nullptr){
			Node node =  path.back();
			path.push_back(*(node.getFather()));
		}	
		
		if((enemies[i].path.size()==0) || (enemies[i].path.size() != path.size()) || (enemies[i].path[path.size()-1].x != path[path.size()-2].getX()) && (enemies[i].path[path.size()-1].y != path[path.size()-2].getY())){
			enemies[i].path.clear();
			for(int j=0;j<path.size()-1;++j){			
				enemies[i].path.emplace_back(SDL_Point {path[j].getX(), path[j].getY()});
			}
		}		
	}

}

// use a* to find the path
bool Astar::findPath(Node &start, SDL_Point goal){
	// closeSet push the last element of the openset and then to find the neigbours of the last element
	AstarCloseSet.emplace_back(std::move(AstarOpenSet.back()));
	AstarOpenSet.pop_back();
	// use bfs to find the neigbours
	int dirt[5] = {0,1,0,-1,0};
	
	int tmpX = AstarCloseSet.back().getX();
	int tmpY = AstarCloseSet.back().getY();
	
	for(int i=0;i<4;++i){
		if((tmpY+dirt[i]<grid_height)&&(tmpY+dirt[i]>=0)&&(tmpX+dirt[i+1]<grid_width)&&(tmpX+dirt[i+1]>=0)){
			if(GameMap[tmpY+dirt[i]][tmpX+dirt[i+1]] == 0){
				GameMap[tmpY+dirt[i]][tmpX+dirt[i+1]] = -1;
				Node temp(tmpX+dirt[i+1], tmpY+dirt[i], std::make_shared<Node>(AstarCloseSet.back()));
				if ((temp.getX() == goal.x) && (temp.getY() == goal.y)){
					path.emplace_back(std::move(temp));
					return true;
				}
				temp.gValue = temp.calcDistance(goal);
				temp.hValue = temp.calcDistance(start);
				AstarOpenSet.emplace_back(std::move(temp));
							
			}
		}
	}
	//std::cout<<"[AstarAlgorithmen]-[findPath]: end"<<std::endl;
	return false;
}

void Astar::sortOpenSet(){
	sort(AstarOpenSet.begin(), AstarOpenSet.end(), [](Node &a, Node &b)->bool{
        	return a > b;	
        });
}

void Astar::Debug(){
	for(int i=0;i<GameMap.size();++i){
		for(int j=0;j<GameMap[i].size();++j){
			std::cout<<GameMap[i][j]<<"  ";
		}
		std::cout<<std::endl;
	}
}

void Astar::Debug2(std::vector<Node> &node){
	for(int i=0;i<node.size();++i){
		std::cout<<"Element "<<i<<" (x,y): ("<<node[i].getX()<<", "<<node[i].getY()<<")"<<std::endl;
	}
}

void Astar::Debug3(std::vector<SDL_Point> path){
	for(int i=0;i<path.size();++i){
		std::cout<<"Element "<<i<<" (x,y): ("<<path[i].x<<", "<<path[i].y<<")"<<std::endl;
	}
}
