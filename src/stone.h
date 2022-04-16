#ifndef STONE_H
#define STONE_H

#include <random>
#include <vector>
#include "SDL.h"
#include "snake.h"

class Stone{
	public:
		Stone(int x, int y);
		bool stoneHit(SDL_Point &p);
		bool stoneHit(int x, int y);
		void setPosition(int x, int y);	
		
		SDL_Point _kordinate;
};


#endif