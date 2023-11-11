
#include <iostream>
#include <concepts>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <fstream>

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

int main() {
	std::vector<std::vector<int>> v {{10, 12, 15, 16, 20, 22, 23, 22, 17, 15, 11, 10, 7, 7, 8, 9, 8, 9, 10, 12},{10, 12, 15, 15, 15, 15, 16, 17, 14, 13, 10, 12, 10, 10, 7, 10, 11, 12, 14, 15},{8, 11, 13, 13, 13, 14, 13, 12, 11, 12, 12, 15, 16, 15, 11, 11, 12, 14, 16, 18},{8, 11, 12, 11, 11, 12, 11, 12, 15, 16, 14, 15, 17, 14, 10, 8, 13, 15, 18, 19},{10, 14, 16, 13, 13, 14, 14, 13, 16, 14, 15, 13, 17, 12, 10, 8, 10, 11, 14, 18},{12, 14, 15, 14, 14, 16, 17, 17, 19, 15, 14, 13, 15, 12, 9, 11, 14, 12, 15, 17},{18, 17, 17, 16, 16, 15, 16, 16, 18, 16, 14, 13, 12, 11, 11, 11, 13, 12, 14, 15},{17, 13, 12, 12, 16, 16, 17, 18, 19, 17, 15, 17, 17, 16, 16, 12, 16, 16, 19, 16},{15, 12, 12, 12, 15, 14, 14, 14, 16, 17, 18, 17, 17, 14, 14, 11, 17, 20, 21, 17},{12, 12, 12, 11, 14, 14, 15, 13, 13, 14, 19, 18, 19, 12, 14, 11, 18, 20, 21, 16},{12, 14, 16, 16, 17, 16, 16, 11, 14, 16, 19, 16, 16, 11, 14, 11, 18, 18, 17, 12},{12, 16, 16, 13, 12, 13, 16, 14, 14, 13, 15, 13, 16, 14, 17, 12, 15, 13, 16, 12},{14, 17, 18, 16, 12, 13, 16, 15, 14, 12, 12, 12, 14, 16, 16, 12, 12, 11, 13, 12},{17, 18, 15, 14, 8, 11, 14, 17, 15, 13, 14, 15, 17, 18, 17, 12, 11, 9, 13, 15},{20, 19, 14, 15, 11, 13, 13, 13, 13, 14, 14, 14, 13, 15, 14, 10, 8, 7, 9, 10},{20, 19, 12, 12, 8, 10, 11, 13, 15, 14, 13, 13, 14, 16, 15, 11, 11, 9, 13, 13},
		{19, 18, 13, 10, 9, 10, 11, 7, 8, 9, 11, 10, 11, 13, 12, 9, 9, 9, 11, 10},{20, 19, 17, 13, 11, 11, 14, 13, 12, 10, 9, 10, 14, 15, 17, 15, 15, 12, 12, 11},{20, 18, 16, 13, 15, 13, 13, 12, 10, 13, 11, 13, 15, 13, 16, 14, 14, 12, 11, 13},{20, 18, 16, 14, 15, 14, 14, 15, 15, 18, 12, 14, 16, 15, 16, 16, 18, 17, 16, 18}} ;
	std::vector<std::vector<std::vector<int>>> vs{};
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
	auto ans = AStar(g, { 3, 8 ,19 }, { 10, 19 ,13 });

}