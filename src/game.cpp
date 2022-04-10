#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceStone(_difficulty);		  
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, _stones);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !stoneHit(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  
  // Check if the snake hit the stone, if yes, then no need to check food position
  if(stoneHit(new_x, new_y)) {
	  snake.alive = false;
	  std::cout<<"hit a stone!"<<std::endl;
	  return;
  }

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.005;
  }
}

void Game::PlaceStone(int level){	
	int nums;
	switch(level){
		case 1:
			nums = 4;
			break;		
		case 2:
			nums = 8;
			break;		
		case 3:
			nums = 12;
			break;
		default:
			nums = 0;
			break;
	}
	
	for(int i=0;i<nums;++i){
		SDL_Point tmp;
		tmp.x = random_w(engine);
		tmp.y = random_h(engine);
		while(!positionAvailable(tmp)){
			tmp.x = random_w(engine);
			tmp.y = random_h(engine);			
		}
		_stones.push_back(tmp);
	}
}

bool Game::stoneHit(SDL_Point p){
	for(auto pi : _stones){
		if((p.x == pi.x) && (p.y == pi.y)) return true;
	}
	return false;
}

bool Game::stoneHit(int x, int y){
	for(auto pi : _stones){
		if((x == pi.x) && (y == pi.y)) return true;
	}
	return false;
}

bool Game::positionAvailable(SDL_Point p){
	// suggest, if point p is available: stone, food, snake
	
	// for Stone:
	if(stoneHit(p)) return false;
	
	// for food:
	// if(food != NULL)
	if((p.x == food.x) && (p.y == food.y)) return false;
	
	// for snake
	snake.SnakeCell(p.x, p.y);
	
	return true;
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }