#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include "./nlohmann/json.hpp"
using json=nlohmann::json;
using namespace std;
json maze_data;
const int MAX_N = 100;
const int MAX_M = 100;
int maze[MAX_N][MAX_M];

int visited[MAX_N][MAX_M];
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};
int tot=0;
int n, m;
vector <vector<pair<int,int>>> Path{};
vector<pair<int,int>> lu{};
int cnt=0;
bool isValid(int x, int y) {
//    printf("x=%d y=%d maze[%d][%d]=%d vis=%d\n",x,y,x,y,maze[x][y],visited[x][y]);
    return x >= 0 && x < n && y >= 0 && y < m && maze[x][y] == 0 && !visited[x][y];
}
void dfs(int x, int y) {
    printf("push x=%d y=%d\n",x,y);
    visited[x][y] = 1;
    lu.push_back(make_pair(x,y));
    if (x == int(maze_data["end"][0]) && y == int(maze_data["end"][1])) {

        Path.push_back(lu);
        int sz=lu.size();
        visited[x][y] = 0;
        lu.pop_back();
        return ;
    }

    for (int i = 0; i < 4; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];
        if (isValid(newX, newY)) {
            dfs(newX, newY);
            visited[newX][newY]=0;
        }
        tot++;
    }
//    visited[x][y]=0;
//    printf("pop x=%d y=%d\n",x,y);
    lu.pop_back();
}

int main() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    cerr<<buffer<<"**************"<<endl;
    ifstream file("D:\\flutters\\spfa\\lib\\maze.json");
    if (!file.is_open()) {
        cerr << "is_not_json" << endl;
        return 1;
    }

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
    printf("%d %d\n",int(maze_data["end"][0]),int(maze_data["end"][1]));
    dfs(int(maze_data["start"][0]),int(maze_data["start"][1]));
    sort(Path.begin(),Path.end(),[](const vector<pair<int,int>>& a, const vector<pair<int,int>>& b)->bool{
        return a.size() < b.size();
    });
    json maze_result,path_result,result;
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
    int sz=Path.size();
    for(int i=0;i<sz;i++){
        json rowArray;
        int pathSize=Path[i].size();
        for(int j=0;j<pathSize;j++){
            rowArray.push_back(Path[i][j]);
        }
        path_result.push_back(rowArray);
    }
    result["paths"]=path_result;
    ofstream result_file("D:\\flutters\\spfa\\lib\\result.json");
    if(result_file.is_open()){
        result_file<<result;
        result_file.close();
    }
    printf("%d %d\n",n,m);
}