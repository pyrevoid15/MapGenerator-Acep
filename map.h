#pragma once
#include "vectors.h"

namespace mapsys {
	typedef enum {
		ocean = 0, coast, river, swamp, desert, grassland,
		rainforest, forest, tundra, taiga, lake, beach, mountain, peak
	} biome;
	
	struct Map {
		std::mt19937 rng;
		int m_width = 40, m_height = 40, m_tilecount = m_width * m_height, mg_progress = 0;
		constexpr static int MAX_ELEVATION = 25;
		constexpr static int WATER_LEVEL = 8;
		constexpr static int MAX_TEMPERATURE = 10;
		constexpr static int MAX_HYDRATION = 5;
		constexpr static int MAX_FERTILITIES = 5;
		constexpr static int TILE_SIZE = 1;

		std::vector<double> base;
		std::vector<vectors::Vector> slopes;
		std::vector<double> elevations;
		std::vector<double> temperatures;
		std::vector<double> hydrations;
		std::vector<double> fertilities;
		std::vector<biome> biomes;
		std::vector<int> name_id;
		std::vector<const char*> names;
		std::vector<SDL_Point> rivers;
		std::vector<SDL_Point> peaks;
		std::vector<SDL_Point> seeds;
		SDL_Point central_seed;

		inline int get_index(SDL_Point p);
		inline int get_index(int x, int y);
		inline void correct_point(SDL_Point& spot);
		inline SDL_Point get_correct_point(int x, int y);
		inline SDL_Point get_random_point();
		inline double get_distance(SDL_Point& a, SDL_Point& b);
		inline bool check_presence(std::vector<SDL_Point>& v, SDL_Point& p);
		inline int check_land(int x, int y);
		inline double check_tile(std::vector<double>& board, int x, int y);
		inline double check_tile2(std::vector<double>& board, int x, int y);
		inline bool checklandlocked(int x, int y);
		inline bool checkwaterlocked(int x, int y);
		inline bool checkcoastlocked(int x, int y);

		void load();
		void save();
		void init();
		void elevate(long seed);
		void ecolyze();

		void render_elev(SDL_Renderer*);
		void render_slope(SDL_Renderer*);
		void render_temp(SDL_Renderer*);
		void render_hydr(SDL_Renderer*);
		void render_fert(SDL_Renderer*);
		void render_biom(SDL_Renderer*);
		void render_name(SDL_Renderer*);
		void render(SDL_Renderer*, int&);
	};
}

