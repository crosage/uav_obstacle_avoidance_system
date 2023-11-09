#include <iostream>
#include <concepts>
#include <vector>
#include <queue>
#include <map>
#include <random>
#include <fstream>
//#include "./include/nl
#include "./include/nlohmann/json.hpp"
using json=nlohmann::json;
/* * * 因为不是项目，我就没按项目标准写类和函数，就怎么方便怎么来，写的类可能有些怪 * * */

constexpr auto min_dis = 1;		//无人机距离墙壁最小距离
constexpr auto cost_weight = 0.5;

template<typename T>
	requires std::is_same_v<T,int> || std::is_same_v<T,long long> || std::is_same_v<T,long>	//要求模板类型为有符号整型
struct Point {
	T _x, _y;
	constexpr Point() :_x(-1), _y(-1) {}
	constexpr Point(T x, T y) :_x(x), _y(y) {}
	constexpr Point(const Point&) = default;
	constexpr Point(Point&&) = default;
	constexpr Point& operator=(const Point&) = default;
	constexpr auto operator<=>(const Point& o)const = default;
	constexpr bool atf()const {		//判定点在第一象限
		return _x >= 0 && _y >= 0;
	}
	template<typename Ty>
	constexpr operator Point<Ty>() {
		return Point<Ty>{static_cast<Ty>(_x), static_cast<Ty>(_y)};
	}
	friend std::ostream& operator<<(std::ostream& os, const Point& p) {	//重载<<运算符，方便打印点
		return os << '(' << p._x << ',' << p._y << ')';
	}
};

template<typename T>
	requires std::is_same_v<T, int> || std::is_same_v<T, long long> || std::is_same_v<T, long>
inline T mhddis(const Point<T>& goal, const Point<T>& p) {	//曼哈顿距离
	return abs(goal._x - p._x) + abs(goal._y - p._y);
}

template<typename T>
	requires std::is_same_v<T, int> || std::is_same_v<T, long long> || std::is_same_v<T, long>
struct Graph {		//地图类，其中元素为0表可用，不为0表不可用
	std::vector<std::vector<T>> _data;
	constexpr Graph() :_data{} {}
	constexpr Graph(const std::vector<std::vector<T>>& data) :_data(data) {}
	constexpr Graph(std::vector<std::vector<T>>&& data) :_data{ std::move(data) } {}
	constexpr std::vector<Point<T>> neighbors(const Point<T> current, const Point<T>& goal, bool avoid_ob = false)const {	//获得当前点周围的下一个判定点
		if (current.atf())		//负数点判定
			return neighbors(current._x, current._y, goal, avoid_ob);
		return {};
	}
	constexpr std::vector<Point<T>> neighbors(const T x, const T y, const Point<T>& goal, bool avoid_ob = false)const {
		std::vector<Point<T>> ans{};
		int _dis = (avoid_ob ? min_dis + 1 : 1);
		if ((x - 1) >= 0 && [&]{
			for (int i = x - 1; i >= 0 && i >= x - _dis; --i) {
				if (_data[i][y] == 1) {
					return false;
				}
				else if (goal._x == i && goal._y == y) {
					return true;
				}
			}
			return true;
			}()) {
			ans.push_back({ x - 1,y });
		}
		if (x + 1 < _data.size() && [&] {
			for (int i = x + 1; i < _data.size() && i <= x + _dis; ++i) {
				if (_data[i][y] == 1) {
					return false;
				}
				else if (goal._x == i && goal._y == y) {
					return true;
				}
			}
			return true;
			}()) {
			ans.push_back({ x + 1,y });
		}
		if (y - 1 >= 0 && [&] {
			for (int i = y - 1; i >= 0 && i >= y - _dis; --i) {
				if (_data[x][i] == 1) {
					return false;
				}
				else if (goal._x == x && goal._y == i) {
					return true;
				}
			}
			return true;
			}()) {
			ans.push_back({ x,y - 1 });
		}
		if (y + 1 < _data[0].size() && [&] {
			for (int i = y + 1; i < _data[0].size() && i <= y + _dis; ++i) {
				if (_data[x][i] == 1) {
					return false;
				}
				else if (goal._x == x && goal._y == i) {
					return true;
				}
			}
			return true;
			}()) {
			ans.push_back({ x,y + 1 });
		}
		return ans;
	}
	constexpr std::vector<Point<T>> neighbors_o(const Point<T> current)const {
		if (current.atf())		//负数点判定
			return neighbors_o(current._x, current._y);
		return {};
	}
	constexpr std::vector<Point<T>> neighbors_o(const T x, const T y)const {
		std::vector<Point<T>> ans{};
		if ((x - 1) >= 0 && _data[x - 1][y] == 0) {
			ans.push_back({ x - 1,y });
		}
		if (x + 1 < _data.size() && _data[x + 1][y] == 0) {
			ans.push_back({ x + 1,y });
		}
		if (y - 1 >= 0 && _data[x][y - 1] == 0) {
			ans.push_back({ x,y - 1 });
		}
		if (y + 1 < _data[0].size() && _data[x][y + 1] == 0) {
			ans.push_back({ x,y + 1 });
		}
		return ans;
	}
	constexpr double get_total_effect(const Point<T>& p)const {
		double ans{};
		for (int i = 0; i < _data.size(); ++i) {
			for (int j = 0; j < _data[0].size(); ++j) {
				if (_data[i][j] == 1) {
					ans += mhddis({ i,j }, p);
				}
			}
		}
		return ans;
	}
	constexpr T get(const Point<T>& p)const {
		return get(p._x, p._y);
	}
	constexpr T get(T x, T y)const {
		return _data[x][y];
	}
};


template<typename T1, typename T2>
	requires std::is_same_v<T2, int> || std::is_same_v<T2, long long> || std::is_same_v<T2, long>
struct cmp {
	bool operator()(const std::pair<T1, Point<T2>>& _left, const std::pair<T1, Point<T2>>& _right)const {
		return _left.first > _right.first;
	};
};

template<typename T>
	requires std::is_same_v<T, int> || std::is_same_v<T, long long> || std::is_same_v<T, long>
std::vector<Point<T>> AStar(const Graph<T>& graph, Point<T> start, Point<T> goal, bool avoid_ob = false) {
	if (graph._data.empty()) return {};		//地图判空
	if (graph.get(start) != 0 || graph.get(goal) != 0) return {};	//始末点判可用
	//A*算法主干
	std::priority_queue<std::pair<T, Point<T>>, std::vector<std::pair<T,Point<T>>>,cmp<T,T>> front_queue(cmp<T,T>{});
	front_queue.push({ 0,start });
	std::map<Point<T>, Point<T>> came_from{};
	came_from[start] = Point<T>{};
	std::map<Point<T>, T> cost_so_far{};
	cost_so_far[start] = 0;
	while (!front_queue.empty()) {
		auto current = front_queue.top().second;
		front_queue.pop();
		if (current == goal) {
			break;
		}
		for (const auto& next : graph.neighbors(current, goal, avoid_ob)) {
			auto next_cost = cost_so_far[current] + 1;
			if (cost_so_far.find(next) == cost_so_far.end() || next_cost < cost_so_far[next]) {
				cost_so_far[next] = next_cost;
				auto cost = next_cost + mhddis(goal, next);
				front_queue.push({ cost,next });
				came_from[next] = current;
			}
		}
	}
	//A*算法结束
	std::vector<Point<T>> ans{ goal };	//答案存储（逆序）
	{
		auto it = came_from.find(goal);
		if (it == came_from.end()) return {};	//不可达判定
		Point<T> t = came_from[goal];
		while (t != start) {
			ans.push_back(t);
			t = came_from[t];
		}
		ans.push_back(start);
	}
	return ans;
}

template<typename T>
	requires std::is_same_v<T, int> || std::is_same_v<T, long long> || std::is_same_v<T, long>
std::vector<Point<T>> AStar_o(const Graph<T>& graph, Point<T> start, Point<T> goal) {
	if (graph._data.empty()) return {};		//地图判空
	if (graph.get(start) != 0 || graph.get(goal) != 0) return {};	//始末点判可用
	//A*算法主干
	std::priority_queue<std::pair<double, Point<T>>, std::vector<std::pair<double, Point<T>>>, cmp<double,T>> front_queue(cmp<double,T>{});
	front_queue.push({ 0,start });
	std::map<Point<T>, Point<T>> came_from{};
	came_from[start] = Point<T>{};
	std::map<Point<T>, T> cost_so_far{};
	cost_so_far[start] = 0;
	while (!front_queue.empty()) {
		Point<T> current = front_queue.top().second;
		front_queue.pop();
		if (current == goal) {
			break;
		}
		for (const auto& next : graph.neighbors_o(current)) {
			auto next_cost = cost_so_far[current] + 1;
			if (cost_so_far.find(next) == cost_so_far.end() || next_cost < cost_so_far[next]) {
				cost_so_far[next] = next_cost;
				auto cost = (next_cost + mhddis(goal, next))*cost_weight + graph.get_total_effect(current)*(1-cost_weight);
				front_queue.push({ cost,next });
				came_from[next] = current;
			}
		}
	}
	//A*算法结束
	std::vector<Point<T>> ans{ goal };	//答案存储（逆序）
	{
		auto it = came_from.find(goal);
		if (it == came_from.end()) return {};	//不可达判定
		Point<T> t = came_from[goal];
		while (t != start) {
			ans.push_back(t);
			t = came_from[t];
		}
		ans.push_back(start);
	}
	return ans;
}

template<::std::ranges::range T>
void printc(T&& t){
	for (const auto& i : t) {
		::std::cout << i << ' ';
	}
}

template<typename T>
	requires requires(T t) { std::cout << t; }
void printc(T&& t) {
	std::cout << t;
}

template<class...Args>
void printc(Args...args) {
	(printc(args), ...);
}


int main() {
    std::ifstream input_file("D:\\flutters\\spfa\\lib\\maze.json");
    if(!input_file.is_open()){
        std::cerr<<"Fail to open input.json"<<std::endl;
        return 1;
    }
    json input_json;
    input_file>>input_json;
    std::cout<<"********"<<std::endl;
    std::cout<<input_json["maze"]<<std::endl;
//    std::cout<<typedef(input_json["map"])<<std::endl;
    Graph<int> g(input_json["maze"]);
    std::cout<<"********"<<std::endl;
//	Graph<int> g{ {{0,0,0,1},{1,0,0,0},{0,1,0,0},{0,0,0,0}} };
	auto t1 = AStar(g, { int(input_json["start"][0]),int(input_json["start"][1]) }, { int(input_json["end"][0]),int(input_json["end"][1])});
	auto t2 = AStar(g, { int(input_json["start"][0]),int(input_json["start"][1]) }, { int(input_json["end"][0]),int(input_json["end"][1]) }, true);
	auto t3 = AStar_o(g, { int(input_json["start"][0]),int(input_json["start"][1]) }, { int(input_json["end"][0]),int(input_json["end"][1]) });
	std::reverse(t1.begin(), t1.end());
	std::reverse(t2.begin(), t2.end());
	std::reverse(t3.begin(),t3.end());
    json points;
    for(auto i : t1){
        std::cout<<i<<std::endl;
        std::vector<int> v;
        v.push_back(i._x);
        v.push_back(i._y);
        points.push_back(v);
        std::cout<<points<<std::endl;
    }

//    std::cout<<result<<std::endl;
    json result;
    result["astar"]=points;
    std::ofstream result_file("D:\\flutters\\spfa\\lib\\astar_result.json");
    if(result_file.is_open()){
        result_file<<result;
        result_file.close();
    }
	printc(t1,"\n\n",t2,"\n\n",t3);

}