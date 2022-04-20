#include <iostream>
#include "food.h"

Food::Food(int x, int y, FoodType ft){
	//SDL_Point tmp(x, y);
	//point = std::move(tmp);
	point.x = x;
	point.y = y;
	type = ft;
}

Food::FoodType Food::getType() const { return type;}
int Food::getX() const {return point.x;}
int Food::getY() const {return point.y;}