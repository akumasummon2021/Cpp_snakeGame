#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "stone.h"
#include "SDL.h"
#include "Astar.h"
#include "food.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, int difficulty, int numsOfEnemy);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  
  Astar astar;
  void upDateGoal(std::vector<Snake> &enemySnakes, std::vector<Food> &foods);

 private:
  Snake snake;
  //SDL_Point food;
  // new difficulty - level of the game
  int _difficulty{1};
  // new stones vector
  std::vector<Stone> _stones;
  // new snake vector
  std::vector<Snake> enemySnakes;
  // new foods vector
  std::vector<Food> foods;
  int numsOfFoods{5};  

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood(int nums);
  void Update();
  // new function
  void PlaceStone(int level);
  int numsOfEnemy{1};
  Food::FoodType eatFood(Snake &s);
  bool stonesHit(SDL_Point p);
  bool stonesHit(int x, int y);
  bool positionAvailable(SDL_Point p);
  void cutEnemies();
};


#endif