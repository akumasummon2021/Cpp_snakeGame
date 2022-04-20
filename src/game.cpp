#include "game.h"
#include <iostream>
#include "SDL.h"
#include <float.h>

Game::Game(std::size_t grid_width, std::size_t grid_height, int difficulty, int numsOfEnemy)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
	  astar(grid_width, grid_height),
	  _difficulty(difficulty),
	  numsOfEnemy(numsOfEnemy) {
	  
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
  int x, y, flag;
  
  while (foods.size()<nums) {	
    flag = 0;
    x = random_w(engine);
    y = random_h(engine);
	
    // Check that the location is not occupied by a snake/enemies item before placing
    // food.
	for(int i=0;i<enemySnakes.size();++i){
		if(enemySnakes[i].SnakeCell(x,y)) {
			flag = 1;
			break;
		}
	}
	
    if ((flag == 0) && !snake.SnakeCell(x, y) && !stonesHit(x, y)) {		
		Food food(x, y, ((x+y)%8==0)?Food::FoodType::FTcutEnemies : Food::FoodType::FTnormal);
		foods.emplace_back(std::move(food));
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;
  
  upDateGoal(enemySnakes, foods);
  
  // calculate the path of the snake
  astar.AstarAlgorithmen(enemySnakes);
  // update the enemies direction
  astar.updateEnemiesDirection(enemySnakes); 
  
  // update position of the enemies
  for(int i=0; i< enemySnakes.size(); ++i){
	enemySnakes[i].Update();
	// update the path of the enemies, if they reach the next path-node
	if((static_cast<int>(enemySnakes[i].head_x) == enemySnakes[i].path.back().x) && (static_cast<int>(enemySnakes[i].head_y) == enemySnakes[i].path.back().y)) enemySnakes[i].path.pop_back();
  }
  
  // update player snake
  snake.Update();
  
  int new_x;
  int new_y;  
  
  for(int i=0;i< enemySnakes.size();++i){
	// eat food check for enemy
    if(eatFood(enemySnakes[i]) != Food::FoodType::FTnoFood) {
      PlaceFood(numsOfFoods);
      // Grow snake and increase speed.
      enemySnakes[i].GrowBody();
    }
	
	new_x = static_cast<int>(enemySnakes[i].head_x);
	new_y = static_cast<int>(enemySnakes[i].head_y);
	
	// check, if enemies eat player
	if((static_cast<int>(snake.head_x) == new_x) && (static_cast<int>(snake.head_y) == new_y)){
		snake.alive = false;
		std::cout<<"be killed!!"<<std::endl;
	}
	else{
		for(auto it = snake.body.begin();it!=snake.body.end();){
			if(((*it).x == new_x) && ((*it).y == new_y)){
				it = snake.body.erase(it, snake.body.end());
				score = score>5?(score-5):0;
				break;
			}
			++it;
		}
	}
  }

  new_x = static_cast<int>(snake.head_x);
  new_y = static_cast<int>(snake.head_y);  
  
  // Check if the snake hit the stone, if yes, then no need to check food position
  if(stonesHit(new_x, new_y)) {
	  snake.alive = false;
	  std::cout<<"hit a stone!"<<std::endl;
	  return;
  }
  Food::FoodType ft;
  ft = eatFood(snake);
  // Check if food is eaten by player
  if (ft != Food::FoodType::FTnoFood) {
	if(ft == Food::FoodType::FTcutEnemies){
		//std::cout<<"eat CutEnemies"<<std::endl;
		cutEnemies();
		score += 5;
	}
	else{
		//std::cout<<"eat normal"<<std::endl;
		score++;
	}
    
    PlaceFood(numsOfFoods);
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.005;
  }  
}

// check if snake s eats the food, if yes, pop the food from the array and return the type of the food
Food::FoodType Game::eatFood(Snake &s){
  int new_x = static_cast<int>(s.head_x);
  int new_y = static_cast<int>(s.head_y);
  int flag = 0;  
  Food::FoodType ft;
  //auto itr;
  for(auto itr = foods.begin(); itr != foods.end(); ++itr){
	  if(((*itr).getX() == new_x) && ((*itr).getY() == new_y)){		  
		  ft = (*itr).getType();
		  flag = 1;
		  itr = foods.erase(itr);
		  break;
	  }
  }
  if (flag == 1) return ft;
  else return Food::FoodType::FTnoFood;
}

// place the stone in the map, only in initialization of the game will be called
void Game::PlaceStone(int level){	
	int nums;
	switch(level){
		case 1:
			nums = 4;
			break;		
		case 2:
			nums = 12;
			break;		
		case 3:
			nums = 25;
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

// check if the player snake hit the stone
bool Game::stonesHit(SDL_Point p){
	for(auto pi : _stones){
		if(pi.stoneHit(p)) return true;
	}
	return false;
}

// check if the player snake hit the stone
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
			if((p.x == food.getX()) && (p.y == food.getY())) return false;
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

void Game::upDateGoal(std::vector<Snake> &enemy, std::vector<Food> &foods){
	
	float distance_squart;
	int index;
	int abs;
	int flag = 0;
	
	// to find out the nearste food
	for(int j=0;j<enemy.size();++j){
		distance_squart = FLT_MAX;
		index = 0;
		for(int i=0;i<foods.size();++i){
			// the goal of the enemies should be different
			for(int k=0;k<j;++k){
				if ((enemy[k].goal.x == foods[i].getX()) && (enemy[k].goal.y == foods[i].getY())){
					flag = 1;
					break;
				}
			}
			// if the food is already the goal of a enemy, then find the other food
			if (flag == 1) {
				flag = 0;
				break;
			}
			
			abs = (enemy[j].head_x-foods[i].getX())*(enemy[j].head_x-foods[i].getX()) + (enemy[j].head_y-foods[i].getY())*(enemy[j].head_y-foods[i].getY());
			if(distance_squart > abs){
				index = i;
				distance_squart = abs;
			}		
		}
		enemy[j].goal.x = foods[index].getX();
		enemy[j].goal.y = foods[index].getY();

		// calc, if player is near the enemies: yes => set player as goal
		abs = (enemy[j].head_x-snake.head_x)*(enemy[j].head_x-snake.head_x) + (enemy[j].head_y-snake.head_y)*(enemy[j].head_y-snake.head_y); 
		if(abs < distance_squart){
			enemy[j].goal.x = static_cast<int>(snake.head_x);
			enemy[j].goal.y = static_cast<int>(snake.head_y);
		}		
	}
}

void Game::cutEnemies(){
	for(int i=0;i<enemySnakes.size();++i){
		int j;
		j = enemySnakes[i].size/2;
		enemySnakes[i].body.erase(enemySnakes[i].body.begin()+j, enemySnakes[i].body.end());
	}
}
