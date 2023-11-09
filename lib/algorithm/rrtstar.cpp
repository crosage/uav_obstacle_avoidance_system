#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include "./include/nlohmann/json.hpp"
using namespace std::literals;
using json=nlohmann::json;

constexpr size_t STEP_SIZE{ 5 };

template<typename T>
struct Point {
	T x, y;
	template<typename U>
	constexpr operator Point<U>()const {
		return Point<U>{static_cast<U>(x), static_cast<U>(y)};
	}
	constexpr Point& operator+=(const Point& o) {
		x += o.x, y += o.y;
		return *this;
	}
	constexpr Point& operator-=(const Point& o) {
		x -= o.x, y -= o.y;
		return *this;
	}
};
template<typename T>
constexpr Point<T> operator-(const Point<T>& a, const Point<T>& b) {
	return { a.x - b.x,a.y - b.y };
}
template<typename T>
constexpr Point<T> operator/(const Point<T>& a, const double div) {
	return { static_cast<T>(a.x / div),static_cast<T>(a.y / div) };
}
template<typename T, typename U>
constexpr bool operator==(const Point<T>& a, const Point<U>& b) {
	return a.x == b.x && a.y == b.y;
}

class node {
	int _x, _y;
	size_t _parent;
public:
	constexpr node() = default;
	constexpr node(int x, int y) : _x(x), _y(y), _parent(0) {}
	constexpr node(int x, int y, size_t parent) : _x(x), _y(y), _parent(parent) {}
	constexpr int x()const { return _x; }
	constexpr int y()const { return _y; }
	constexpr size_t parent()const { return _parent; }
	constexpr operator Point<int>()const {
		return Point{ _x,_y };
	}
	constexpr auto operator<=>(const node& o)const = default;
};

constexpr bool node_close_end(const node& node, const Point<int>& end) {
	return (node.x() - end.x) * (node.x() - end.x) + (node.y() - end.y) * (node.y() - end.y) < 2;
}

template<typename T>
constexpr size_t dist(const Point<T>& p1, const Point<T>& p2) {
	return static_cast<size_t>(p1.x - p2.x) * (p1.x - p2.x) + static_cast<size_t>(p1.y - p2.y) * (p1.y - p2.y);
}

class Graph {
	std::vector<std::vector<int>> _data;
public:
	Graph(const std::vector<std::vector<int>>& data) : _data(data) {}
	Graph(std::vector<std::vector<int>>&& data) : _data{ std::move(data) } {}
	constexpr Point<int> straight_to(const Point<int>& p1, const Point<int>& p2)const {
		Point<float> tmp{ Point<float>(p1) };
		Point<float> step_p{ Point<float>(p2 - p1) / std::sqrt(dist(p1,p2)) };
		for (; dist(p1, Point<int>(tmp)) < dist(p1,p2); tmp += step_p) {
			if (_data[tmp.x][tmp.y] != 0) {
				tmp -= step_p;
				return tmp;
			}
		}
		return p2;
	}
	constexpr size_t size()const {
		return _data.size();
	}
	constexpr const std::vector<int>& at(size_t pos)const {
		return _data.at(pos);
	}
	constexpr const std::vector<int>& operator[](size_t pos)const {
		return _data.at(pos);
	}
};

auto rrtstar(const Graph& graph, const Point<int>& start, const Point<int>& end, std::chrono::milliseconds timeout) {
	auto time_start = std::chrono::steady_clock::now();
	node new_node{ start.x,start.y };
	std::vector<node> tree{ new_node };
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<> dis(0, std::max(graph.size(), graph.at(0).size()) - 1);
	while (graph.straight_to(new_node,end) != end) {
		//生成一个采样点
		Point<int> prand{ dis(gen) % graph.size(), dis(gen) % graph[0].size()};
		while (graph[prand.x][prand.y] != 0) {
			prand.x = dis(gen) % graph.size();
			prand.y = dis(gen) % graph[0].size();
		}
		//选择最近的节点
		size_t tmp{ 0 };
		size_t distance{ dist(Point<int>(tree[0]), prand)};
		for (size_t i{}; i < tree.size(); ++i) {
			size_t tmp_dis{ dist(Point<int>(tree[i]), prand) };
			if (tmp_dis < distance) {
				distance = tmp_dis;
				tmp = i;
			}
		}
		//将该节点向采样点延申
		Point pnew{ graph.straight_to(tree[tmp], prand) };
		{
			if (pnew == Point<int>(tree[tmp])) continue;
			size_t tmp_dis{ dist(Point<int>(tree[tmp]), pnew) };
			if (tmp_dis > STEP_SIZE) {
				pnew.x = static_cast<double>(pnew.x - tree[tmp].x()) * std::sqrt(STEP_SIZE / tmp_dis) + tree[tmp].x();
				pnew.y = static_cast<double>(pnew.y - tree[tmp].y()) * std::sqrt(STEP_SIZE / tmp_dis) + tree[tmp].y();
			}
		}

		//连接节点
		new_node = { pnew.x,pnew.y,tmp };
		tree.push_back(new_node);
		if (std::chrono::steady_clock::now() - time_start > timeout) {
			break;
		}
	}
	//运算结果
	std::vector<Point<int>> ans{end,new_node};
	while (tree[new_node.parent()] != tree[0]) {
		ans.push_back(tree[new_node.parent()]);
		new_node = tree[new_node.parent()];
	}
	ans.push_back(start);
	return ans;
}

int main() {
    // 从maze.json文件中读取数据
    std::ifstream mazeFile("D:\\flutters\\spfa\\lib\\maze.json");
    if (!mazeFile.is_open()) {
        std::cerr << "Failed to open maze.json" << std::endl;
        return 1;
    }

    nlohmann::json mazeData;
    mazeFile >> mazeData;
    std::cout<<mazeData<<std::endl;
    // 从maze.json中提取图形和起点终点信息
    std::vector<std::vector<int>> maze = mazeData["maze"];
    // 从maze.json中提取图形和起点终点信息
    Point start = {mazeData["start"][0], mazeData["start"][1]};
    Point end = {mazeData["end"][0], mazeData["end"][1]};


    // 构建图形对象
    Graph graph(maze);

    // 运行rrtstar算法
    auto startTime = std::chrono::high_resolution_clock::now();
    auto ans = rrtstar(graph, start, end, std::chrono::minutes(5));
    auto endTime = std::chrono::high_resolution_clock::now();

    // 计算算法执行时间
    auto executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // 反转结果
    std::reverse(ans.begin(), ans.end());

    // 输出结果到控制台
    for (const auto& i : ans) {
        std::cout << '(' << i.x << ", " << i.y << ") ";
    }

    // 将结果写入rrt_result.json文件
    nlohmann::json resultData;
    json points;
    for(auto i : ans){
//        std::cout<<i<<std::endl;
        std::vector<int> v;
        v.push_back(i.x);
        v.push_back(i.y);
        points.push_back(v);
        std::cout<<points<<std::endl;
    }
    resultData["path"] = points;
    resultData["execution_time_ms"] = executionTime;



    std::ofstream resultFile("D:\\flutters\\spfa\\lib\\rrt_result.json");
    if (resultFile.is_open()) {
        resultFile << resultData.dump(4);
        resultFile.close();
    } else {
        std::cerr << "Failed to write rrt_result.json" << std::endl;
    }

    return 0;
}