#include "game.h"
#include <iostream>
#include "SDL.h"
#include <float.h>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
	  astar(grid_width, grid_height) {
	  
	for(int i=0;i<numsOfEnemy;++i) {
		Snake tmp(grid_width, grid_height, i);
		tmp.speed = (0.15f);
		enemySnakes.emplace_back(std::move(tmp));
	}		
	PlaceStone(_difficulty);		
	PlaceFood(numsOfFoods);	
	// initialize a* Gamemap
	astar.placeStoneOnMap(_stones);
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
    renderer.Render(snake, foods, _stones, enemySnakes);

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

void Game::PlaceFood(int nums) {
  int x, y;
  
  while (foods.size()<nums) {	
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y) && !stonesHit(x, y)) {
		SDL_Point food;
		food.x = x;
		food.y = y;
		foods.push_back(food);
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;
  
  upDateGoal(enemySnakes, foods);/*
  std::cout<<"snake 1 goal, x: "<<enemySnakes[0].goal.x<<std::endl;
  std::cout<<"snake 1 goal, y: "<<enemySnakes[0].goal.y<<std::endl;
  std::cout<<"snake 2 goal, x: "<<enemySnakes[1].goal.x<<std::endl;
  std::cout<<"snake 2 goal, y: "<<enemySnakes[1].goal.y<<std::endl;
  
*/
  
  astar.updateEnemiesDirection(enemySnakes);
  std::cout<<"After updateEnemiesDirection"<<std::endl;
  astar.AstarAlgorithmen(enemySnakes);
  std::cout<<"After AstarAlgorithmen"<<std::endl;
  
  std::cout<<"After pop_front"<<std::endl;
  for(int i=0; i< enemySnakes.size(); ++i){
	  enemySnakes[i].Update();	  
  }  
  
  snake.Update();
  
  // eat food check for enemy
  for(int i=0;i< enemySnakes.size();++i){
    if (eatFood(enemySnakes[i])) {
      PlaceFood(numsOfFoods);
      // Grow snake and increase speed.
      enemySnakes[i].GrowBody();
    } 	
  }
  
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  
  // Check if the snake hit the stone, if yes, then no need to check food position
  if(stonesHit(new_x, new_y)) {
	  snake.alive = false;
	  std::cout<<"hit a stone!"<<std::endl;
	  return;
  }

  // Check if there's food over here
  if (eatFood(snake)) {
    score++;
    PlaceFood(numsOfFoods);
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.005;
  }  
}

bool Game::eatFood(Snake s){
  int new_x = static_cast<int>(s.head_x);
  int new_y = static_cast<int>(s.head_y);	
  
  //auto itr;
  for(auto itr = foods.begin(); itr != foods.end(); ++itr){
	  if(((*itr).x == new_x) && ((*itr).y == new_y)){
		  itr = foods.erase(itr);
		  return true;
	  }
  }
  return false;
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
		Stone tmp(random_w(engine), random_h(engine));
		while(!positionAvailable(tmp._kordinate)){
			tmp.setPosition(random_w(engine), random_h(engine));
		}
		_stones.emplace_back(std::move(tmp));
	}
}

bool Game::stonesHit(SDL_Point p){
	for(auto pi : _stones){
		if(pi.stoneHit(p)) return true;
	}
	return false;
}

bool Game::stonesHit(int x, int y){
	for(auto pi : _stones){
		if(pi.stoneHit(x, y)) return true;
	}
	return false;
}

bool Game::positionAvailable(SDL_Point p){
	// suggest, if point p is available: stone, food, snake
	
	// for Stone:
	if(stonesHit(p)) return false;
	
	// for food:
	if(foods.size() != 0){
		for(auto food : foods){
			if((p.x == food.x) && (p.y == food.y)) return false;
		}
	}		
	
	// for snake
	if(snake.SnakeCell(p.x, p.y)) return false;
	
	for(auto s : enemySnakes) {
		if(snake.SnakeCell(p.x, p.y)) return false;
	}
	
	return true;
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::upDateGoal(std::vector<Snake> &enemy, std::vector<SDL_Point> &foods){
	
	float distance_squart;
	int index;
	int flag = 0;
	
	for(int j=0;j<enemy.size();++j){
		distance_squart = FLT_MAX;
		index = 0;
		for(int i=0;i<foods.size();++i){
			for(int k=0;k<j;++k){
				if ((enemy[k].goal.x == foods[i].x) && (enemy[k].goal.y == foods[i].y)){
					flag = 1;
					break;
				}
			}
			
			if (flag == 1) {
				flag = 0;
				break;
			}
			
			int abs = (enemy[j].head_x-foods[i].x)*(enemy[j].head_x-foods[i].x) + (enemy[j].head_y-foods[i].y)*(enemy[j].head_y-foods[i].y);
			if(distance_squart > abs){
				index = i;
				distance_squart = abs;
			}		
		}
		enemy[j].goal.x = foods[index].x;
		enemy[j].goal.y = foods[index].y;
	}
	

}
