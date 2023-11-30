#if 1

#include <iostream>
#include <concepts>
#include <vector>
#include <queue>
#include <map>
#include<set>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <numbers>
#include "./include/nlohmann/json.hpp"
using json=nlohmann::json;
constexpr int r = 2;			//???????

template<std::integral T>
class Point {
	T _x, _y;

public:
	constexpr Point()noexcept :_x{}, _y{} {}
	constexpr Point(T x, T y) : _x(x), _y(y) {}
	constexpr bool operator==(const Point& o)const = default;
	constexpr auto operator<=>(const Point& o)const = default;
	template<std::integral Ty>
	constexpr operator Point<Ty>() {
		return { static_cast<Ty>(_x),static_cast<Ty>(_y) };
	}
	constexpr bool atf()const {
		return _x >= 0 && _y >= 0;
	}
	constexpr T getX()const {
		return _x;
	}
	constexpr T getY()const {
		return _y;
	}
};

template<std::integral T1, std::integral T2>
auto mhddis(const Point<T1>& p1, const Point<T2>& p2) {
	return abs(p1.getX() - p2.getX()) + abs(p1.getY() - p2.getX());
}

template<std::integral T>
class Graph {
	std::vector<std::vector<T>> _g;

public:
	constexpr Graph() :_g{} {}
	constexpr Graph(const std::vector<std::vector<T>>& g) : _g(g) {}
	constexpr Graph(std::vector<std::vector<T>>&& g) : _g(std::move(g)) {}
	constexpr T get(T x, T y)const {
		return _g.at(x).at(y);
	}
	constexpr T get(const Point<T>& p)const {
		return get(p.getX(), p.getY());
	}
	constexpr bool empty()const {
		return _g.empty();
	}
	constexpr bool no_solid_on_round(T x, T y)const {
		for (int i = 0; i < 360; ++i) {
			double rad = i * std::numbers::pi / 180.;
			long long rx = x + static_cast<long long>(r * cos(rad));
			long long ry = static_cast<long long>(y + r * sin(rad));
			if (rx < 0 || rx >= _g.size() || ry < 0 || ry >= _g[0].size()) {
				return false;
			}
			if (_g[rx][ry] == 1) {
				return false;
			}
		}
		return true;
	}
	constexpr std::vector<Point<T>> neighbors(const T x, const T y)const {
		std::vector<Point<T>> ans{};
		if ((x - 1 >= 0) && _g[x - 1][y] == 0 && no_solid_on_round(x-1,y)) {
			ans.push_back({ x - 1,y });
		}
		if (x + 1 < _g.size() && _g[x + 1][y] == 0 && no_solid_on_round(x + 1, y)) {
			ans.push_back({ x + 1,y });
		}
		if (y - 1 >= 0 && _g[x][y - 1] == 0 && no_solid_on_round(x, y - 1)) {
			ans.push_back({ x,y - 1 });
		}
		if (y + 1 < _g[0].size() && _g[x][y + 1] == 0 && no_solid_on_round(x, y + 1)) {
			ans.push_back({ x,y + 1 });
		}
		return ans;
	}
	constexpr std::vector<Point<T>> neighbors(const Point<T>& current)const {
		if (current.atf()) {
			return neighbors(current.getX(), current.getY());
		}
		return {};
	}
};

template<typename T1, typename T2>
struct cmp {
	bool operator()(const std::pair<T1, Point<T2>>& _left, const std::pair<T1, Point<T2>>& _right)const {
		return _left.first > _right.first;
	}
};

template<typename T, typename U = size_t>
std::vector<Point<T>> AStar(const Graph<T>& graph, const Point<T>& start, const Point<T>& goal) {
	if (graph.empty()) return {};
	if (graph.get(start) != 0 || graph.get(goal) != 0) return {};

	std::priority_queue<std::pair<U, Point<T> >, std::vector<std::pair<U, Point<T>> >, cmp<U, T> > front_queue(cmp<U, T>{});
	front_queue.push({ static_cast<U>(0),start });
	std::map<Point<T>, Point<T> > came_from{};
	came_from[start] = {};
	std::map<Point<T>, U> cost_so_far{};
	cost_so_far[start] = 0;
	while (!front_queue.empty()) {
		Point<T> current = front_queue.top().second;
		front_queue.pop();
		if (current == goal) {
			break;
		}
		for (const auto& next : graph.neighbors(current)) {
			U next_cost = cost_so_far[current] + 1;
			if (cost_so_far.find(next) == cost_so_far.end() || next_cost < cost_so_far[next]) {
				cost_so_far[next] = next_cost;
				auto cost = next_cost + mhddis(goal, next);
				front_queue.push({ cost,next });
				came_from[next] = current;
			}
		}
	}

	std::vector<Point<T> > ans{ goal };
	{
		auto it = came_from.find(goal);
		if (it == came_from.end()) return {};
		Point<T> t = came_from[goal];
		while (t != start) {
			ans.push_back(t);
			t = came_from[t];
		}
		ans.push_back(start);
	}
	return ans;
}

int main() {
	Graph<int> g{ {{0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                  {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                  {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                  {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                  {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
                  {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
                  {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
                  {1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}} };
	auto t = AStar(g, { 2,4 }, { 21,19 });
		json points;
//    		puts("**");
    	for(const auto&i:t){
//    	    puts("haha");
    		points.push_back(std::vector{i.getX(),i.getY()});
//    		std::cout << points << '\n';
    	}

    	json result;
    	result["path"]=points;
    	std::ofstream result_file("D:\\flutters\\spfa\\lib\\astar_gran_result.json");
    	if(result_file.is_open()){
    		result_file<<result;
    		result_file.close();
    	}
	for (const auto& i : t) {
		std::cout << "(" << i.getX() << ", " << i.getY() << ")\n";
	}
}


#endif