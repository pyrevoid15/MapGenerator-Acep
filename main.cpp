#include "map.h"

mapsys::Map* map;
SDL_Window* win;
SDL_Renderer* ren;
SDL_Event e;
bool is_playing;
int slider;

int main(int args, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	win = SDL_CreateWindow("MapgenA", 30, 30, 700, 700, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	ren = SDL_CreateRenderer(win, -1, 0);

	map = new mapsys::Map();
	map->elevate(954690086657);
	is_playing = true;
	slider = 0;
	SDL_Point drivf;

	SDL_SetWindowSize(win, map->m_width * mapsys::Map::TILE_SIZE, map->m_height * mapsys::Map::TILE_SIZE);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderClear(ren);
	map->render(ren, slider);
	SDL_RenderPresent(ren);

	while (is_playing) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				is_playing = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_LSHIFT) {
					slider > 0 ? --slider : slider;
					map->render(ren, slider);
					SDL_RenderPresent(ren);
				}
				else if (e.key.keysym.sym == SDLK_RSHIFT) {
					slider < 5 ? ++slider : slider;
					map->render(ren, slider);
					SDL_RenderPresent(ren);
				}
			}
			else if (e.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&drivf.x, &drivf.y);
				SDL_SetWindowTitle(win, std::string("MapgenA - (" + std::to_string(drivf.x) + ", " +
					std::to_string(drivf.y) + ") - " + std::to_string(map->elevations[map->get_index(drivf)])).c_str());
			}
		}
		
		SDL_Delay(30);
	}

	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
	return 0;
}