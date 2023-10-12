import 'package:flutter/material.dart';

class Block extends StatefulWidget {
  final int x;
  final int y;
  final int blockState;
  // 1已经遍历 0没有遍历 -1为团块
  const Block(
      {super.key, required this.x, required this.y, required this.blockState});

  @override
  _BlockState createState() => _BlockState();
}
class _BlockState extends State<Block> {
  late int have_visit;
  late int beenVisit;

  Color getBlockColor() {
    if (widget.blockState >=1) return Colors.grey[200]!;
    else if(widget.blockState==0) return Colors.grey[50]!;
    else return Colors.grey[400]!;
  }
  Widget getNumber(){
    if(widget.blockState>=1) return Text("${widget.blockState}");
    else return Text(" ");
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
            beenVisit = 1-beenVisit;
          });
          // been_choose=been_choose;
        },
        child: AnimatedContainer(
          duration: Duration(seconds: 1),
          // height: widget.block_height,
          // width: widget.block_width,
          color: getBlockColor(),
          child: Center(
            child: getNumber(),
          ),
        ));
  }
}
