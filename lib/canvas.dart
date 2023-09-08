import 'package:flutter/material.dart';
import 'package:spfa/block.dart';

class canvas extends StatefulWidget {
  final int n;
  final int m;

  canvas({required this.n, required this.m});

  @override
  _CanvasState createState() => _CanvasState();
}

class _CanvasState extends State<canvas> {
  @override
  Widget build(BuildContext context) {
    return Container(
      width: 700,

      child: GridView.builder(
        gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: widget.m,
          // childAspectRatio: 0.8
        ),
        itemCount: widget.n * widget.m,
        itemBuilder: (BuildContext context, int index) {
          final int row = index ~/ widget.m;
          final int col = index % widget.m;
          return

              block(
                  x: row,
                  y: col,
                  block_height: 20,
                  block_width: 20,
                  is_block: 0);
        },
        shrinkWrap: true,

      ),
    );
  }
}
