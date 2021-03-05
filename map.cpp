#include "map.h"

using namespace mapsys;

inline int Map::get_index(SDL_Point p) {
	return std::max(std::min(p.y, m_height - 1), 0) * m_width + (p.x + m_width) % m_width;
}
inline int Map::get_index(int x, int y) {
	return std::max(std::min(y, m_height - 1), 0) * m_width + (x + m_width) % m_width;
}
inline void Map::correct_point(SDL_Point& spot) {
	if (spot.x < 0) spot.x = m_width - spot.x;
	else if (spot.x > m_width - 1) spot.x = 0;

	if (spot.y < 0 || spot.y > m_height - 1)
		spot.y = m_height - 1;
}
inline SDL_Point Map::get_correct_point(int x, int y) {
	SDL_Point p = { x, y };
	correct_point(p);
	return p;
}
inline SDL_Point Map::get_random_point() {
	return { int(rng() % m_width), int(rng() % m_height) };
}
inline double Map::get_distance(SDL_Point& a, SDL_Point& b) {
	return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}
inline bool Map::check_presence(std::vector<SDL_Point>& v, SDL_Point& p) {
	for (SDL_Point& s : v) {
		if (s.x == p.x && s.y == p.y)
			return true;
	}
	return false;
}
inline int Map::check_land(int x, int y) {
	try {
		if (elevations[get_index(x, y)] > 1)
			return 0;
		else if (elevations[get_index(x, y)] > 0)
			return 1;
		else
			return 2;
	}
	catch (_exception& e) {
		get_correct_point(x, y);

		if (elevations[get_index(x, y)] > 1)
			return 0;
		else if (elevations[get_index(x, y)] > 0)
			return 1;
		else
			return 2;
	}
}
inline double Map::check_tile(std::vector<double>& board, int x, int y) {
	int xa, ya;
	try {
		return board[get_index(x, y)];
	}
	catch (_exception& e) {
		try {
			if (x >= m_width) xa = 0;
			else if (x < 0) xa = m_width - 1;
			else xa = x;

			if (y >= m_height) ya = 0;
			else if (y < 0) ya = m_height - 1;
			else ya = y;

			return board[get_index(xa, ya)];
		}
		catch (_exception& e2) {
			return 1;
		}
	}
}
inline double Map::check_tile2(std::vector<double>& board, int x, int y) {
	return abs(check_tile(board, x, y));
}
inline bool Map::checklandlocked(int x, int y) {
	if (check_land(x, y - 1) == 0 && check_land(x, y) == 0 &&
		check_land(x, y + 1) == 0 && check_land(x - 1, y - 1) == 0 &&
		check_land(x - 1, y) == 0 && check_land(x - 1, y + 1) == 0 &&
		check_land(x + 1, y - 1) == 0 && check_land(x + 1, y) == 0 &&
		check_land(x + 1, y + 1) == 0)
		return true;
	else return false;
}
inline bool Map::checkwaterlocked(int x, int y) {
	if (check_land(x, y - 1) != 0 && check_land(x, y) != 0 &&
		check_land(x, y + 1) != 0 && check_land(x - 1, y - 1) != 0 &&
		check_land(x - 1, y) != 0 && check_land(x - 1, y + 1) != 0 &&
		check_land(x + 1, y - 1) != 0 && check_land(x + 1, y) != 0 &&
		check_land(x + 1, y + 1) != 0)
		return true;
	else return false;
}
inline bool Map::checkcoastlocked(int x, int y) {
	if (check_land(x, y - 1) > 0 && check_land(x, y) > 0 &&
		check_land(x, y + 1) > 0 && check_land(x - 1, y - 1) > 0 &&
		check_land(x - 1, y) > 0 && check_land(x - 1, y + 1) > 0 &&
		check_land(x + 1, y - 1) > 0 && check_land(x + 1, y) > 0 &&
		check_land(x + 1, y + 1) > 0)
		return true;
	else return false;
}
void Map::load() {
	;
}
void Map::save() {
	;
}
void Map::init() {

}
void Map::elevate(long seed) {
	std::cout << "Begin." << '\n';
	rng.seed(seed);
	elevations.resize(m_tilecount);
	
	for (double& e : elevations) {
		e = 0;
	}
	for (double& e : base) {
		e = 0;
	}

	std::cout << "Seed for seeds." << '\n';
	if (m_tilecount > 399)
		central_seed = { int(rng() % m_width) / 2 + m_width / 4, int(rng() % m_height) / 2 + m_height / 4 };
	else
		central_seed = { m_width / 2, m_height / 2 };
	seeds.push_back(central_seed);
	elevations[get_index(central_seed.x, central_seed.y)] = 9;

	std::vector<SDL_Point> spots;
	int seed_limit = int((m_width + m_height) / 2.8) + 8;
	double max_seed_dist = pow(((m_width + m_height) / 2.3), 1.1);
	double min_land_ratio;

	if (m_tilecount < 150) {
		min_land_ratio = 0.9;
		max_seed_dist = (m_width + m_height) / 1.5;
	}
	else if (m_tilecount < 750) min_land_ratio = 0.8;
	else if (m_tilecount < 1500) min_land_ratio = 0.6;
	else if (m_tilecount < 3000) min_land_ratio = 0.4;
	else min_land_ratio = 0.25;

	SDL_Point spot;
	bool clusterable;
	int obsession = 0, a;
	std::cout << "Distribute Seeds." << '\n';
	while (seeds.size() < seed_limit) {
		std::cout << seeds.size() << "/" << seed_limit << '\n';
		spot = get_random_point();
		if (get_distance(spot, central_seed) < max_seed_dist) {
			clusterable = false;
			obsession = 0;
			for (SDL_Point& s : seeds) {
				++obsession;
				if (get_distance(spot, s) < 4 || seeds.size() < seed_limit / 3.2) {
					clusterable = true;
					break;
				}
				if (obsession > 500)
					break;
			}

			if (clusterable) {
				if (elevations[get_index(spot)] < 1) {
					elevations[get_index(spot)] = 1.2;
					seeds.push_back(SDL_Point(spot));
					spots.push_back(SDL_Point(spot));
				}
				else
					elevations[get_index(spot)] += 0.7;
			}
			if (obsession > 500) break;
		}
		else {
			elevations[get_index(rng() % m_height, rng() % m_width)] += 0.1;
			if (!check_presence(spots, spot))
				spots.push_back(SDL_Point(spot));
		}
	}

	std::cout << "Distribute More Seeds." << '\n';
	int i, j, sc = int(std::max(1, m_tilecount / 1000));
	for (i = 0; i < sc; i++) {
		spot = get_random_point();
		seeds.push_back(SDL_Point(spot));
		spots.push_back(SDL_Point(spot));
	}

	std::string abi = "";
	i = 0;
	for (double d : elevations) {
		i++;
		if (i % m_width == 0)
			abi += '\n';

		if (d > 0)
			abi += std::to_string(int(d));
		else
			abi += '_';
		

	}
	std::cout << abi << '\n';

	std::cout << "Erupt volcanoes." << '\n';
	sc = int(pow(((m_width + m_height) * 10.5), pow((m_width / 10), 0.11)));
	SDL_Point anchor;
	std::cout << sc << '\n';
	for (i = 0; i < sc; i++) {
		anchor = spots[rng() % spots.size()];
		if (rng() % 256 < 128) spot = { anchor.x + int(rng() % 3) - 1, anchor.y + int(rng() % 3) - 1 };
		else spot = { anchor.x + int(rng() % 3) - 1, anchor.y + int(rng() % 3) - 1 };

		correct_point(spot);
		spots.push_back(SDL_Point(spot));
		elevations[get_index(spot)] += 0.4;

		if (rng() % 100 < 82) {
			if (rng() % 100 < 78)
				elevations[get_index(get_correct_point(spot.x, spot.y - 1))] += 0.06 / std::max(1.0, m_tilecount / 11000.0);
			if (rng() % 100 < 78)
				elevations[get_index(get_correct_point(spot.x, spot.y + 1))] += 0.06 / std::max(1.0, m_tilecount / 11000.0);
			if (rng() % 100 < 78)
				elevations[get_index(get_correct_point(spot.x - 1, spot.y))] += 0.06 / std::max(1.0, m_tilecount / 9500.0);
			if (rng() % 100 < 78)
				elevations[get_index(get_correct_point(spot.x + 1, spot.y))] += 0.06 / std::max(1.0, m_tilecount / 9000.0);
		}
		elevations[get_index(get_random_point())] -= 0.1;
	}

	int max_island_count = int(pow(m_width + m_height, 0.8) / 2);
	int island_count = 0;
	std::vector<SDL_Point> islands;

	std::cout << "Add islands." << '\n';
	while (island_count < max_island_count) {
		spot = get_random_point();
		if (checkwaterlocked(spot.x, spot.y)) {
			rng() % 128 < 64 ? elevations[get_index(spot)] = 1.2 : elevations[get_index(spot)] = 2;
			++island_count;
			spots.push_back(SDL_Point(spot));
			islands.push_back(SDL_Point(spot));
			if (rng() % 100 < 59) {
				for (i = 0; i < 3; i++) {
					elevations[get_index(spot.y + (rng() % 2) * 2 - 1, spot.x + (rng() % 2) * 2 - 1)] += 1.2;
				}
			}
		}
	}

	for (i = 0; i < m_tilecount * 0.03; i++) {
		spot = get_random_point();
		if (elevations[get_index(spot)] > 2) {
			if (rng() % 100 < 50)
				elevations[get_index(spot)] /= (rng() % 6 + 18) * 10;
			else
				elevations[get_index(spot)] /= (rng() % 6 + 13) * 10;
		}
	}

	std::cout << "Modify map heights." << '\n';
	for (j = 0; j < m_height; j++) {
		for (i = 0; i < m_width; i++) {
			if (elevations[get_index(i, j)] > (rng() % 11 + 35) / 10.0 &&
				elevations[get_index(i, j)] > std::max(elevations[get_index(i, j)],
					std::max(elevations[get_index(i, j)], std::max(elevations[get_index(i, j)],
						elevations[get_index(i, j)])))) {
				peaks.push_back(SDL_Point({ i, j }));
				elevations[get_index(i, j)] = pow(elevations[get_index(i, j)], 1.4);
			}
			if (1.6 < elevations[get_index(i, j)] && elevations[get_index(i, j)] < 2.6)
				elevations[get_index(i, j)] = pow(elevations[get_index(i, j)], 0.86);
		}
	}

	int p;
	std::cout << "Expand the map 64-fold." << '\n';
	//fullrows.reserve(elevations.size() * 32);
	base.reserve(elevations.size() * 16);
	elevations.reserve(elevations.size() * 64);
	
	for (p = 0; p < 4; p++) {
		std::cout << "Quadruple map size.	x" << (p + 1) << "/" << 4 << '\n';
		base.clear();
		base.resize(0);
		for (double& d : elevations) {
			base.push_back(d);
			d = 0;
		}

		elevations.resize(elevations.size() * 4);
		for (j = 0; j < m_height; j++) {
			for (i = 0; i < m_width; i++) {
				elevations[i * 2 + (j * 2) * (m_width * 2)] = base[get_index(i, j)];
			}
		}

		for (j = 0; j < m_height; j++) {
			for (i = 0; i < m_width; i++) {
				elevations[i * 2 + 1 + (j * 2) * (m_width * 2)] = (base[get_index(i, j)] + base[get_index(get_correct_point(i + 1, j))]) / (rng() % 6 + 18) * 10;
			}
		}

		m_width *= 2;
		m_height *= 2;
		for (j = 1; j < m_height; j += 2) {
			for (i = 0; i < m_width; i++) {
				elevations[get_index(i, j)] = (elevations[get_index(i, j - 1)] + elevations[get_index(i, j + 1)]) / (rng() % 6 + 18) * 10;
			}
		}


		if (p < 2) {
			for (double& tile : elevations) {
				if (tile > 5 && rng() % 100 < 68)
					tile -= 0.14 * (rng() % 5 + 2);
			}
		}

		for (SDL_Point& p : seeds)
			p = { p.x * 2, p.y * 2 };

		if (p < 3) {
			for (i = 0; i < m_tilecount * 0.02; i++) {
				spot = get_random_point();
				if ((elevations[get_index(spot)] > 2 || (!check_presence(seeds, spot) || rng() % 100 > 94)) &&
					(spot.x != central_seed.x || spot.y != central_seed.y)) {
					if (rng() % 100 < 40)
						elevations[get_index(spot)] /= (rng() % 6 + 18) * 10;
					else if (rng() % 100 < 50)
						elevations[get_index(spot)] /= (rng() % 6 + 13) * 10;
					else
						elevations[get_index(spot)] -= (rng() % 6) * 0.1;
				}
			}
		}
	}

	std::cout << "Smooth the map." << '\n';
		for (p = 0; p < 2; p++) {
			for (j = 1; j < m_height - 1; j++) {
				for (i = 0; i < m_width; i++) {
					elevations[get_index(i, j)] = (elevations[get_index(get_correct_point(i - 1, j))] + elevations[get_index(get_correct_point(i + 1, j))] +
						elevations[get_index(get_correct_point(i, j - 1))] + elevations[get_index(get_correct_point(i, j + 1))]
						) / (rng() % 5 + 38) * 10;
				}
			}
		}

	std::cout << "Modify map heights again." << '\n';
	for (j = 0; j < m_height; j++) {
		for (i = 0; i < m_width; i++) {
			if (elevations[get_index(i, j)] > 1) {
				elevations[get_index(i, j)] = pow(elevations[get_index(i, j)], 1.1);
			}
			else if (elevations[get_index(i, j)] < 1) {
				elevations[get_index(i, j)] = (1 - elevations[get_index(i, j)]) / 2;
				if (elevations[get_index(i, j)] < 0)
					elevations[get_index(i, j)] = 0;
			}

			spot = { i, j };
			if (check_presence(peaks, spot)) {
				elevations[get_index(i, j)] = pow(elevations[get_index(i, j)], 1.5);
			}
		}
	}

	std::cout << "Correct mountain positions." << '\n';
	for (SDL_Point& p : peaks) {
		p = { p.x * 16, p.y * 16 };
		correct_point(p);

		if (elevations[get_index(i, j)] > 0)
			elevations[get_index(i, j)] = pow(elevations[get_index(i, j)], 1.2);
	}

	std::cout << "Fix oceans." << '\n';
	for (j = 0; j < m_height; j++) {
		for (i = 0; i < m_width; i++) {
			if (elevations[get_index(i, j)] > 1) {
				elevations[get_index(i, j)] = pow(elevations[get_index(i, j)], 0.8) * pow(11, 0.2);
				elevations[get_index(i, j)] += MAX_ELEVATION / 2 - 1;
			}
			else {
				elevations[get_index(i, j)] = 1 - pow(elevations[get_index(i, j)], 1);
				elevations[get_index(i, j)] = 1 / pow(1 / elevations[get_index(i, j)], 0.9);
				if (elevations[get_index(i, j)] < 0.5)
					elevations[get_index(i, j)] = pow(elevations[get_index(i, j)], 2.05);
				else
					elevations[get_index(i, j)] = pow(elevations[get_index(i, j)], 1.3);
				elevations[get_index(i, j)] *= MAX_ELEVATION / 2;
			}
		}
	}

	for (j = 0; j < m_height; j++) {
		for (i = 0; i < m_width; i++) {
			if (elevations[get_index(i, j)] > 1 && elevations[get_index(i, j)] < WATER_LEVEL + 4) {
				elevations[get_index(i, j)] = pow(elevations[get_index(i, j)], 0.55) * pow(WATER_LEVEL + 4.0, 0.45);
			}
			if (elevations[get_index(i, j)] > WATER_LEVEL - 3) {
				elevations[get_index(i, j)] = pow(elevations[get_index(i, j)], 0.8) * pow(MAX_ELEVATION + 4.0, 0.2);
			}
		}
	}

	std::cout << "Smooth the map again." << '\n';
	for (p = 0; p < 1; p++) {
		for (j = 1; j < m_height - 1; j++) {
			for (i = 0; i < m_width; i++) {
				elevations[get_index(i, j)] = (elevations[get_index(get_correct_point(i - 1, j))] + elevations[get_index(get_correct_point(i + 1, j))] +
					elevations[get_index(get_correct_point(i, j - 1))] + elevations[get_index(get_correct_point(i, j + 1))]
					) / 4;
			}
		}
	}

	base.clear();
	base.resize(0);
	
	for (int j = 0; j < m_height; j++) {
		for (int i = 0; i < m_width; i++) {
			a = 0;
			spot = { i, j };
			for (int y = -1; y < 2; y++) {
				for (int x = -1; x < 2; x++) {
					a += elevations[get_index(get_correct_point(i + x, j + y))];
				}
			}
			a /= 9;
		}
	}

	std::cout << "Elevation complete." << '\n';
}
void Map::ecolyze() {
	std::cout << "Find temperature." << '\n';
	temperatures.reserve(elevations.size());
	hydrations.reserve(elevations.size());
	for (int j = 0; j < m_height; j++) {
		for (int i = 0; i < m_width; i++) {

		}
	}

	for (int j = 0; j < m_height; j++) {
		for (int i = 0; i < m_width; i++) {

		}
	}
}
void Map::render_elev(SDL_Renderer* rend) {
	int color;
	for (int j = 0; j < m_height; j++) {
		for (int i = 0; i < m_width; i++) {
			color = int(std::max(std::min(int(elevations[get_index(i, j)]) * 1.0 / MAX_ELEVATION * 255, 255.0), 0.0));
			if (false) { //elevations[get_index(i, j)] < WATER_LEVEL + .5 && elevations[get_index(i, j)] > WATER_LEVEL - .5) {
				SDL_SetRenderDrawColor(rend, 255, color, color, 255);
				SDL_RenderFillRect(rend, &SDL_Rect({ i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE }));
			}
			else {
				SDL_SetRenderDrawColor(rend, color, color, color, 255);
				SDL_RenderFillRect(rend, &SDL_Rect({ i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE }));
			}
		}
	}
}
void Map::render_slope(SDL_Renderer* rend) {
	int color;
	for (int j = 0; j < m_height; j++) {
		for (int i = 0; i < m_width; i++) {
			;
		}
	}
}
void Map::render_temp(SDL_Renderer* rend) { ; }
void Map::render_hydr(SDL_Renderer* rend) { ; }
void Map::render_fert(SDL_Renderer* rend) { ; }
void Map::render_biom(SDL_Renderer* rend) { ; }
void Map::render_name(SDL_Renderer* rend) { ; }
void Map::render(SDL_Renderer* rend, int& slider) {
	switch (slider) {
	case 0:
		render_elev(rend);
	case 1:
		render_biom(rend);
	case 2:
		render_temp(rend);
	case 3:
		render_fert(rend);
	case 4:
		render_hydr(rend);
	case 5:
		render_name(rend);
	}
}