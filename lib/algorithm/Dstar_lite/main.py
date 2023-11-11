from d_star import DStar
import json

with open('D:\\flutters\\spfa\\lib\\algorithm\\Dstar_lite\\maze.json', 'r') as input:
    map = json.load(input)
m = map.get('n')
n = map.get('m')
start = map.get('start')  # 获取起始点的坐标
end = map.get('end')  # 获取终点的坐标
obstacle = []  # 记录障碍物的坐标
x, y = 0, 0
for i in map.get('maze'):
    for j in i:
        if j == 1:
            obstacle.append((x, y))
        y += 1
    y = 0
    x += 1
#print(obstacle)
pf = DStar(x_start=start[0], y_start=start[1], x_goal=end[0], y_goal=end[1])
for i in obstacle:
    pf.update_cell(i[0], i[1], -1)  # -1不允许通行；0允许通行
pf.replan()  # 得到路径规划的结果
path = pf.get_path()
paths = []
for i in path:
    paths.append((i.x, i.y))
paths_res = {}
paths_res['path'] = paths
print(paths_res)
with open('D:\\flutters\\spfa\\lib\\dstar_result.json', 'w') as write_file:
    write_file.write(json.dumps(paths_res))
