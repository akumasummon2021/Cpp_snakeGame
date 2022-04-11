#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  //SDL_Point food;
  // new difficulty - level of the game
  int _difficulty{1};
  // new stones vector
  std::vector<SDL_Point> _stones;
  // new snake vector
  std::vector<Snake> enemySnakes;
  // new foods vector
  std::vector<SDL_Point> foods;
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
  bool eatFood(Snake s);
  bool stoneHit(SDL_Point p);
  bool stoneHit(int x, int y);
  bool positionAvailable(SDL_Point p);
};

#endif