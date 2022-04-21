#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argv, char** args) {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  
  int difficulty = -1, numsOfEnemy = -1;
  
  std::cout<<"please enter quantity level of the obstacles(0-3): ";
  std::cin>>difficulty;
  while(std::cin.fail() || (difficulty<0) || (difficulty>=4)){
	std::cin.clear(); 
    std::cin.ignore(100, '\n'); 
	std::cout<<"input not inside the range(0-3)!"<<std::endl;
	std::cout<<"please enter quantity level of the obstacles(0-3): ";
	std::cin>>difficulty;
  }

  std::cout<<"please enter quantity of the Enemies(0-2): ";
  std::cin>>numsOfEnemy;
  while(std::cin.fail() || (numsOfEnemy<0) || (numsOfEnemy>=3)){
	std::cin.clear(); 
    std::cin.ignore(100, '\n'); 
	std::cout<<"input not inside the range(0-2)!"<<std::endl;
	std::cout<<"please enter quantity level of the obstacles(0-2): ";
	std::cin>>numsOfEnemy;
  }


  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight, difficulty, numsOfEnemy);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}