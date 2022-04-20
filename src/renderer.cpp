#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const snake, std::vector<Food> foods, std::vector<Stone> stones, std::vector<Snake> enemy) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);
  
  // Render Stones
  SDL_SetRenderDrawColor(sdl_renderer, 0xA9, 0xA9, 0xA9, 0xFF);
  for (Stone const &stone : stones) {
    block.x = stone._kordinate.x * block.w;
    block.y = stone._kordinate.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }  

  // Render foods
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  for (Food const &food : foods) {  
	block.x = food.getX() * block.w;
	block.y = food.getY() * block.h;
	if(food.getType() == Food::FoodType::FTnormal){
		//std::cout<<"normal FT!!"<<std::endl;
		SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
	}
	else {
		//std::cout<<"cutEnemies FT!!"<<std::endl;
		SDL_SetRenderDrawColor(sdl_renderer, 0xEE, 0x63, 0x63, 0xFF);
	}
	SDL_RenderFillRect(sdl_renderer, &block);
  }
  
  // Render snake's body
  SnakeRender(snake, block, false);

  for(auto const &s : enemy){
	  SnakeRender(s, block, true);
  }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::SnakeRender(Snake snake, SDL_Rect &block, bool isEnemy){
	
  if(!isEnemy){
	  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  }	
  else{
	  SDL_SetRenderDrawColor(sdl_renderer, 0x9A, 0xFF, 0x9A, 0xFF);
  }
  
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
      if(!isEnemy){
	    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
	  }	
      else{
	    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x8B, 0x00, 0xFF);
      }	      
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);	
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
