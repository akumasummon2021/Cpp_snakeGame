This project is capstone project of the udacity course, and the snake game is choosed. The game is already compiled and run in Udacity-VM without any problem.

## instruction of Snake Game 
- after the game lauched, the player should choose the quantity level of the obstacles(0 is low, 3 is high) and the quantity of the enemies(0 is no enemy, 2 is 2 enemies).
- after that player can see the game windows and press enter or 's'(for start) to start the game.
- in the game player use up/down/left/right to controll the player snake, which hat a blue "head" and white body. 
- the obstacles are displayed in grey rectangle in the game, if the player snake hit the stone, the game ends.
- if the player snake eat itself, the game ends.
- there is/are a enemy/enemies in the game, which hat deep green head and green body, if the player hat choosen 1 or 2 enemies before the game start. 
- if the enemy/enemies hat eaten the body of the player snake, score will be reduced 5 points and player snake will be cut. 
- if the enemy/enemies hat eaten the head of the player snake, game is over. 
- Player snake can't eat the enemy/enemies, it can only cross it.
- the enemy/enemies will set the nearest food as its goal and to eat them. But if player snake is too close to the enemies head, they will also set the head of the player snake as its goal.
- at the begin of the game, the speed of the player snake is slower than the enemy/enemies, so please not so close to them. 
- the player snake can go through the boundary of the game windows, but the enemy/enemies can not. So in emergency situation please go through the boundary to get rid of them.
- the foods are displayed in orange or red rectangle in the game, if the player snake eat a food, score add 1. 
- if the player snake eats a food, score add 1. If the player snake eats a red food, enemy/enemies will reduce theirs body. 

let's start the game!

## Filelist:
- Astar.cpp/.h: A-star algorithem will be implemented for the enemies to hunt for the food or player. 
- controller.cpp/.h: controll the player snake. 
- food.cpp/.h: class for the foods.
- game.cpp/.h: define the logic of the game.
- main.cpp: the main function of the game.
- renderer.cpp/.h: to render the game window.
- snake.cpp/.h: class for the player snake/enemy, including the methode for snake-moving, body-growing.
- stone.cpp/.h: class for the stone.

## Example
![image](https://github.com/akumasummon2021/Cpp_snakeGame/blob/development/Peek_2022-04-22_11-16.gif)



