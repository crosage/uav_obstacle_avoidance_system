
#include <iostream>
#include <concepts>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include "./include/nlohmann/json.hpp"
using json=nlohmann::json;

template<std::integral T>
class Point {
	T _x, _y, _z;

public:
	constexpr Point()noexcept :_x{}, _y{}, _z{} {}
	constexpr Point(T x, T y, T z) :_x(x), _y(y), _z(z) {}
	constexpr bool operator==(const Point& o)const = default;
	constexpr auto operator<=>(const Point& o)const = default;
	template<std::integral Ty>
	constexpr operator Point<Ty>() {
		return { static_cast<Ty>(_x),static_cast<Ty>(_y),static_cast<Ty>(_z) };
	}
	constexpr bool atf()const {
		return _x >= 0 && _y >= 0 && _z >= 0;
	}
	constexpr T getX()const {
		return _x;
	}
	constexpr T getY()const {
		return _y;
	}
	constexpr T getZ()const {
		return _z;
	}
};

template<std::integral T1, std::integral T2>
auto mhddis(const Point<T1>& p1, const Point<T2>& p2) {
	return abs(p1.getX() - p2.getX()) + abs(p1.getY() - p2.getY()) + abs(p1.getZ() - p2.getZ());
}

template<std::integral T>
class Graph {
	std::vector<std::vector<std::vector<T>>> _g;

public:
	constexpr Graph() :_g{} {}
	constexpr Graph(const std::vector<std::vector<std::vector<T>>>& g) :_g(g) {}
	constexpr Graph(std::vector<std::vector<std::vector<T>>>&& g) :_g(std::move(g)) {}
	constexpr T get(T x, T y, T z)const {
		return _g.at(x).at(y).at(z);
	}
	constexpr T get(const Point<T>& p)const {
		return get(p.getX(), p.getY(), p.getZ());
	}
	constexpr bool empty()const {
		return _g.empty();
	}
	constexpr std::vector<Point<T>> neighbors(const T x, const T y, const T z)const {
		std::vector<Point<T>> ans{};
		if ((x - 1 >= 0) && _g[x - 1][y][z] == 0) {
			ans.push_back({ x - 1,y,z });
		}
		if (x + 1 < _g.size() && _g[x + 1][y][z] == 0) {
			ans.push_back({ x + 1,y,z });
		}
		if (y - 1 >= 0 && _g[x][y - 1][z] == 0) {
			ans.push_back({ x,y - 1,z });
		}
		if (y + 1 < _g[0].size() && _g[x][y + 1][z] == 0) {
			ans.push_back({ x,y + 1,z });
		}
		if (z - 1 >= 0 && _g[x][y][z - 1] == 0) {
			ans.push_back({ x,y,z - 1 });
		}
		if (z + 1 < _g[0][0].size() && _g[x][y][z + 1] == 0) {
			ans.push_back({ x,y,z + 1 });
		}
		return ans;
	}
	constexpr std::vector<Point<T>> neighbors(const Point<T>& current)const {
		if (current.atf()) {
			return neighbors(current.getX(), current.getY(), current.getZ());
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

	std::priority_queue<std::pair<U, Point<T> >, std::vector<std::pair<U, Point<T> >>, cmp<U, T>>  front_queue{ cmp<U, T>{} };
	front_queue.push({ static_cast<U>(0),start});
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

struct pt{
	int x, y;
		constexpr auto operator<=>(const pt& o) const = default;
	constexpr bool operator==(const pt& o) const = default;
};

int main() {
    std::ifstream input_file("D:\\flutters\\spfa\\lib\\algorithm\\test.json");
    if(!input_file.is_open()){
        std::cerr<<"Fail to open input.json"<<std::endl;
        return 1;
    }
    puts("**************");
    json input_json;
    input_file>>input_json;
	std::vector<std::vector<int>> v(input_json["maze"]);
	std::vector<std::vector<std::vector<int>>> vs{};
	puts("**************");
	vs.resize(v.size());
	for (int i = 0; i < v.size(); ++i) {
		vs[i].resize(v[i].size());
	}

	for (auto& i : vs) {
		for (auto& j : i) {
			j.resize(30);
		}
	}
	for (int i = 0; i < v.size(); ++i) {
		for (int j = 0; j < v[i].size(); ++j) {
			vs[i][j][v[i][j]] = 1;
		}
	}
	Graph g{vs};
			puts("**");
	auto ans = AStar(g, { int(input_json["start"][0]), int(input_json["start"][1]), int(input_json["start_height"]) },
						{ int(input_json["end"][0]), int(input_json["end"][1]), int(input_json["end_height"])});
		puts("**");
	json points;
		puts("**");
	std::set<pt> s;
	for(const auto&i:ans){
		s.insert({i.getX(),i.getY()});
	}
	for(const auto&i:s){
	    puts("haha");
		points.push_back(std::vector{i.x,i.y});
		std::cout << points << '\n';
	}

	json result;
	result["path"]=points;
	std::ofstream result_file("D:\\flutters\\spfa\\lib\\astar_3d_result.json");
	if(result_file.is_open()){
		result_file<<result;
		result_file.close();
	}

}