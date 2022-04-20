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
  
  while((difficulty<0) || (difficulty>=4)){
	std::cout<<"please enter quantity level of the obstacles(0-3): ";
	std::cin>>difficulty;
	if((difficulty<0) || (difficulty>=4)){
		std::cout<<"input not inside the range(0-3)! use default value 1"<<std::endl;
	}
  }

  while((numsOfEnemy<0) || (numsOfEnemy>=3)){  
	std::cout<<"please enter quantity level of the Enemies(0-2): ";
	std::cin>>numsOfEnemy;
	if((numsOfEnemy<0) || (numsOfEnemy>=3)){
		std::cout<<"input not inside the range(0-2)! use default value 1"<<std::endl;
		numsOfEnemy = 1;
	}	
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