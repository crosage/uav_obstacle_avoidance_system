import 'package:flutter/material.dart';
import 'package:spfa/components/block.dart';
import 'package:spfa/components/block_with_height.dart';

class CanvasWithHeight extends StatefulWidget {
  final int n;
  final int m;
  final List<List<int>> maze;
  final List<List<int>> blockStates;
  CanvasWithHeight({
    required this.n,
    required this.m,
    required this.maze,
    required this.blockStates,
  });

  @override
  _CanvasState createState() => _CanvasState();
}

// 状态：当前搜索深度，需要传入搜索后的深度状态图，传入图的n，m，传入图的原始状态
class _CanvasState extends State<CanvasWithHeight> {
  //0为道路，1为障碍 -2起点 -3终点
  int get_block_state(int x, int y) {
    if (widget.maze[x][y] == -2) {
      return -2;
    } else if (widget.maze[x][y] == -3) {
      return -3;
    } else {
      return widget.blockStates[x][y];;
    }
  }

  int get_height(int x, int y) {
    return widget.maze[x][y];
  }

  @override
  Widget build(BuildContext context) {
    print(widget.maze);
    print(widget.blockStates);
    return Container(
        width: 700,
        height: 700,
        child: SingleChildScrollView(
          child: Column(
            children: [
              GridView.builder(
                gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                  crossAxisCount: widget.m,
                ),
                itemCount: widget.n * widget.m,
                itemBuilder: (BuildContext context, int index) {
                  final int row = index ~/ widget.m;
                  final int col = index % widget.m;
                  return BlockWithHeight(
                    x: row,
                    y: col,
                    blockState: get_block_state(row, col),
                    onBlockChange: () {},
                    height: get_height(row, col),
                  );
                },
                shrinkWrap: true,
              ),
            ],
          ),
        ),
    );
  }
}
