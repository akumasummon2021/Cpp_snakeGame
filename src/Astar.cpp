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

void Astar::AstarAlgorithmen(std::vector<Snake> &enemies){
	// reset Map
	GameMap = GameMap_init;	
	placeEnemiesOnMap(enemies);	
	
	// find the path for all enemies
	for(int i=0;i<enemies.size();++i){
		std::cout<<"Enemy "<<i<<" in calc"<<std::endl;
		// reset openset/Map/Enemies
		if(!AstarOpenSet.empty()) {AstarOpenSet.clear();}
		GameMap = GameMap_init;	
		placeEnemiesOnMap(enemies);	
		
		// Astar:
		// at first add head into openset
		Node head(static_cast<int>(enemies[i].head_x), static_cast<int>(enemies[i].head_y), nullptr);
		head.gValue = head.calcDistance(enemies[i].goal);		
		AstarOpenSet.push_back(head);
		
		std::cout<<"head(x,y): "<<head.getX()<<", "<<head.getY()<<std::endl;
		std::cout<<"goal(x,y): "<<enemies[i].goal.x<<", "<<enemies[i].goal.y<<std::endl;
		std::cout<<"Astar begins"<<std::endl;
		// find neigbours untile find the goal
		
		bool bl_reachGoal = findPath(AstarOpenSet.front(), head, enemies[i].goal);
		
		std::cout<<"AstarOpenSet first(x,y): "<<AstarOpenSet[0].getX()<<", "<<AstarOpenSet[0].getY()<<" distance, g: "<<AstarOpenSet[0].gValue<<" distance, h: "<<AstarOpenSet[0].hValue<<std::endl;
		
		while(!bl_reachGoal){
			std::cout<<"loop begins, size of AstarOpenSet: "<<AstarOpenSet.size()<<std::endl;
			// neigbours of front() are already in openset, front() is then useless	

			//AstarOpenSet.pop_front();
			
			// sort the openset 
			sortOpenSet();
			Debug2(AstarOpenSet);
			
			std::cout<<"After sortOpenSet"<<std::endl;
			bl_reachGoal = findPath(AstarOpenSet[AstarOpenSet.size()-1], head, enemies[i].goal);
			std::cout<<"bl_reachGoal = "<<bl_reachGoal<<std::endl;
		}
			
		std::cout<<"find path begins"<<std::endl;
		// find the path from goal back to start
		do
		{
			Node tmp = path.back();
			path.emplace_back(*(tmp.getFather().get()));
		}while((path.back().getX()==head.getX())&&path.back().getY()==head.getY());
		
		// after path decision finished, updateEnemiesDirection();
		
		// update enemies location
		// enemies[i].update();
		std::cout<<"Debug begins"<<std::endl;
		Debug();
		
	}
	

}

bool Astar::findPath(Node node, Node &start, SDL_Point goal){
	int dirt[5] = {0,1,0,-1,0};
	AstarOpenSet.pop_back();
	std::cout<<"findPath begins"<<std::endl;
	Debug2(AstarOpenSet);
	
	for(int i=0;i<4;++i){
		if((node.getY()+dirt[i]<grid_height)&&(node.getY()+dirt[i]>=0)&&(node.getX()+dirt[i+1]<grid_width)&&(node.getY()+dirt[i+1]>=0)){
			if(GameMap[node.getY()+dirt[i]][node.getX()+dirt[i+1]] == 0){
				GameMap[node.getY()+dirt[i]][node.getX()+dirt[i+1]] = -1;
				std::shared_ptr<Node> nodePtr(&node);
				Node temp(node.getX()+dirt[i+1], node.getY()+dirt[i], nodePtr);
				if ((temp.getX() == goal.x) && (temp.getY() == goal.y)){
					path.push_back(temp);
					return true;
				}
				temp.gValue = temp.calcDistance(goal);
				temp.hValue = temp.calcDistance(start);
				AstarOpenSet.push_back(temp);
							
			}
		}
	}
	std::cout<<"loop ends"<<std::endl;
	Debug2(AstarOpenSet);	
	std::cout<<"findPath ends"<<std::endl;
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