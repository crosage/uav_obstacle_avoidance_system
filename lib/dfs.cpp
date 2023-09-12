#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include "./nlohmann/json.hpp"
using json=nlohmann::json;
using namespace std;
const int MAX_N = 100;
const int MAX_M = 100;
int maze[MAX_N][MAX_M];
int visited[MAX_N][MAX_M];
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
int tot=0;
int n, m;
bool isValid(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m && maze[x][y] == 0 && !visited[x][y];
}
bool dfs(int x, int y) {
    printf("x=%d y=%d\n",x,y);
    if (x == n - 1 && y == m - 1) {
        return true;
    }
    visited[x][y] = ++tot;
    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];
        if (isValid(newX, newY)) {
            if (dfs(newX, newY)) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    cerr<<buffer<<"**************"<<endl;
    ifstream file("./lib/maze.json");
    if (!file.is_open()) {
        cerr << "is_not_json" << endl;
        return 1;
    }
    json maze_data;
    file>>maze_data;
    n=maze_data["n"];
    m=maze_data["m"];
    if (maze_data["maze"].is_array()) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                maze[i][j] = maze_data["maze"][i][j];
            }
        }
    }
    dfs(0,0);
    json maze_result,result;
    for (int i = 0; i < n; ++i) {
        json rowArray;
        for (int j = 0; j < m; ++j) {
            if(maze[i][j]==1){
                rowArray.push_back(-1);
            }
            else {
                rowArray.push_back(visited[i][j]);
            }
        }
        maze_result.push_back(rowArray);
    }
    result["maze"]=maze_result;
    ofstream result_file("./lib/result.json");
    if(result_file.is_open()){
        result_file<<result.dump(4);
        result_file.close();
    }
    printf("%d %d\n",n,m);
}