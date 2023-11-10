import 'package:flutter/material.dart';

class Block extends StatefulWidget {
  final int x;
  final int y;
  final int blockState;
  final Function onBlockChange;
  // 1已经遍历 0没有遍历 -1为团块
  const Block(
      {super.key, required this.x, required this.y, required this.blockState,required this.onBlockChange});

  @override
  _BlockState createState() => _BlockState();
}

class _BlockState extends State<Block> {
  late int have_visit;
  late int beenVisit;

  Color getBlockColor() {
    if (widget.blockState >= 1)
      return Color.fromARGB(255, 252, 250, 242);
    else if (widget.blockState == 0)
      return Color.fromARGB(255, 255, 255, 251);
    else if (widget.blockState==-2)
      return Colors.redAccent;
    else if (widget.blockState==-3)
      return Colors.blueAccent;
    else
      return Color.fromARGB(255, 145, 152, 159);
  }

  Widget getNumber() {
    if (widget.blockState >= 1)
      return Text("${widget.blockState}");
    if (widget.blockState==-2)
      return Text("${1}");
    else
      return Text(" ");
  }

  @override
  void initState() {
    super.initState();
    beenVisit = 0;
  }

  @override
  Widget build(BuildContext context) {
    return InkWell(
      onTap: () {
        setState(() {
          beenVisit = 1 - beenVisit;
        });
        // been_choose=been_choose;
      },
      child: AnimatedContainer(
        duration: Duration(seconds: 1),
        // height: widget.block_height,
        // width: widget.block_width,
        // color: getBlockColor(),
        child: Center(
          child: getNumber(),
        ),
        decoration: BoxDecoration(
          color:  getBlockColor(),
          border: Border.all(
            color: Colors.black, // 设置边框颜色
            width: 0.5, // 设置边框宽度
          ),
        ),
      ),
    );
  }
}
