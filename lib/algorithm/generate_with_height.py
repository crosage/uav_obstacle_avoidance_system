import numpy as np
import json
from scipy.ndimage import uniform_filter


def generate_matrix(rows, cols):
    # 生成一个随机矩阵
    matrix = np.random.randint(0, 30, size=(rows, cols))

    # 平滑处理矩阵
    matrix_smoothed = uniform_filter(matrix.astype(float), size=3)

    print(matrix_smoothed)
    return matrix_smoothed.astype(int)


def set_start_end(matrix):
    rows, cols = matrix.shape
    start_row, start_col = np.random.randint(0, rows), np.random.randint(0, cols)
    end_row, end_col = np.random.randint(0, rows), np.random.randint(0, cols)

    # 设置起点和终点高度，确保不低于地面高度
    start_height = max(np.random.randint(0, 20), matrix[start_row, start_col])
    end_height = max(np.random.randint(0, 20), matrix[end_row, end_col])

    return matrix, (start_row, start_col, start_height), (end_row, end_col, end_height)

def convert_to3d(matrix):
    rows,cols=matrix.shape
    matrix_3d = [[[] for _ in range(cols)] for _ in range(rows)]
    print(matrix_3d)
    for i in range(rows):
        for j in range(cols):
            for z in range(30):
                if z<matrix[i][j] :
                    matrix_3d[i][j].append(1)
                else :
                    matrix_3d[i][j].append(0)
    return matrix_3d

# 设置矩阵大小
rows, cols = 20, 20

# 生成矩阵
my_matrix = generate_matrix(rows, cols)

# 设置起点和终点
my_matrix, start_point, end_point = set_start_end(my_matrix)

data = {
    "n":20,
    "m":20,
    "start": [start_point[0],start_point[1]],
    "end": [end_point[0],end_point[1]],
    "maze": my_matrix.tolist(),
    "start_height":int(start_point[2]),
    "end_height":int(end_point[2]),
    "algorithm":convert_to3d(my_matrix)
}
print(my_matrix.tolist())
print(start_point[2])
print(end_point[2])
print(convert_to3d(my_matrix))
with open("D:\\flutters\\spfa\\lib\\algorithm\\test.json","w") as f:
    json.dump(data,f)
# 打印生成的矩阵和起点终点信息
print("Generated Matrix:")
print(my_matrix)
print("\nStart Point:", start_point)
print("End Point:", end_point)
