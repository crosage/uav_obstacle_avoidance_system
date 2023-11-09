import 'package:flutter/material.dart';
import 'package:spfa/block.dart';

class Canvas extends StatefulWidget {
  final int n;
  final int m;
  final List<List<int>> maze;
  final List<List<int>> blockStates;

  Canvas({
    required this.n,
    required this.m,
    required this.maze,
    required this.blockStates,
  });

  @override
  _CanvasState createState() => _CanvasState();
}

// 状态：当前搜索深度，需要传入搜索后的深度状态图，传入图的n，m，传入图的原始状态
class _CanvasState extends State<Canvas> {
  int get_block_state(int x, int y) {
    if (widget.maze[x][y] == 0) {
      return widget.blockStates[x][y];
    } else {
      return -1;
    }
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      width: 700,
      child: GridView.builder(
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: widget.m,
        ),
        itemCount: widget.n * widget.m,
        itemBuilder: (BuildContext context, int index) {
          final int row = index ~/ widget.m;
          final int col = index % widget.m;
          return Block(x: row, y: col, blockState: get_block_state(row, col));
        },
        shrinkWrap: true,
      ),
    );
  }
}
