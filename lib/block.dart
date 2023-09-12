import 'package:flutter/material.dart';

class block extends StatefulWidget {
  final int x;
  final int y;
  final int block_state;
  // 1已经遍历 0没有遍历 -1为团块
  const block(
      {super.key, required this.x, required this.y, required this.block_state});

  @override
  _BlockState createState() => _BlockState();
}

class _BlockState extends State<block> {
  late int have_visit;
  late int been_visit;

  Color getBlockColor() {
    if (widget.block_state == 1) return Colors.yellow[200]!;
    else if(widget.block_state==0) return Colors.yellow[50]!;
    else return Colors.yellow[400]!;
  }

  @override
  void initState() {
    super.initState();
    been_visit = 0;
  }

  @override
  Widget build(BuildContext context) {
    return InkWell(
        onTap: () {
          setState(() {
            been_visit = 1-been_visit;
          });
          // been_choose=been_choose;
        },
        child: AnimatedContainer(
          duration: Duration(seconds: 3),
          // height: widget.block_height,
          // width: widget.block_width,
          color: getBlockColor(),
          child: Center(
            child: Text("0"),
          ),
        ));
  }
}
