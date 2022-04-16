#include <iostream>
#include "SDL.h"
#include "stone.h"

// Methodes of Class Stone:
Stone::Stone(int x, int y){
	_kordinate.x = x;
	_kordinate.y = y;
}

bool Stone::stoneHit(SDL_Point &p){
	if((p.x == _kordinate.x) && (p.y == _kordinate.y)) return true;
	return false;
}

bool Stone::stoneHit(int x, int y){
	if((x == _kordinate.x) && (y == _kordinate.y)) return true;
	return false;
}

void Stone::setPosition(int x, int y){
	_kordinate.x = x;
	_kordinate.y = y;
}