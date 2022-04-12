#include "snake.h"
#include <cmath>
#include <iostream>
#include <float.h>
#include <cmath>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

// find the direction for the enemies
void Snake::upDateDiretion(std::vector<SDL_Point> &foods){
	float distance_squart = FLT_MAX;
	int index;
	
	for(int i=0;i<foods.size();++i){
		int abs = (head_x-foods[i].x)*(head_x-foods[i].x) + (head_y-foods[i].y)*(head_y-foods[i].y);
		if(distance_squart > abs){
			index = i;
			distance_squart = abs;
		}		
	}
	
	std::cout<<"distance_squart: "<<distance_squart<<std::endl;
	
	if((head_x-foods[index].x != 0) && (abs(head_x-foods[index].x) - speed < speed) && ((direction == Direction::kRight) || (direction == Direction::kLeft))){
		head_x = (direction == Direction::kRight)?(foods[index].x - speed):(foods[index].x + speed);
	}
	else if ((head_y-foods[index].y != 0) && (abs(head_y-foods[index].y) - speed < speed) && ((direction == Direction::kUp) || (direction == Direction::kDown))){
		head_y = (direction == Direction::kUp)?(foods[index].y + speed):(foods[index].y - speed);
	}
	else if(head_x-foods[index].x > 0){
		std::cout<<"x diff > 0: "<<(head_x-foods[index].x)<<std::endl;
		direction = Direction::kLeft;
	}
	else if (head_y-foods[index].y > 0){
		std::cout<<"y diff > 0: "<<(head_y-foods[index].y)<<std::endl;
		direction = Direction::kUp;
	}
	else if (head_x-foods[index].x < 0){
		std::cout<<"x diff < 0: "<<(head_x-foods[index].x)<<std::endl;
		direction = Direction::kRight;
	}
	else if (head_y-foods[index].y < 0){
		std::cout<<"y diff < 0: "<<(head_y-foods[index].y)<<std::endl;
		direction = Direction::kDown;
	} 
}