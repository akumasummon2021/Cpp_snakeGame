#ifndef Food_H
#define Food_H

#include "SDL.h"

class Food {
	public:
		enum class FoodType {FTnoFood = 0, FTnormal, FTcutEnemies};
		Food(int x, int y, FoodType ft);
		FoodType getType() const;
		int getX() const;
		int getY() const;
	private:
		SDL_Point point;
		FoodType type;
};


#endif