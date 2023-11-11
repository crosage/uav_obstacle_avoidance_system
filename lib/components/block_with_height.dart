import 'package:flutter/material.dart';

class BlockWithHeight extends StatefulWidget {
  final int x;
  final int y;
  final int blockState;
  final Function onBlockChange;
  final int height;
  // 1已经遍历 0没有遍历 -1为团块
  const BlockWithHeight(
      {super.key,required this.height, required this.x, required this.y, required this.blockState,required this.onBlockChange});

  @override
  _BlockState createState() => _BlockState();
}

class _BlockState extends State<BlockWithHeight> {
  late int have_visit;
  late int beenVisit;


  Color getBlockColor() {
    print("x=${widget.x} y=${widget.y} h=${widget.height} ${widget.blockState}");
    if(widget.height==-2){
      return Colors.greenAccent;
    }
    if(widget.height==-3){
      return Colors.yellowAccent;
    }
    if(widget.blockState>0){
      return Colors.white;
    }
    Color baseColor=Colors.black;
    double opacity = (widget.height / 100.0).clamp(0.0, 1.0);
    // print("x=${widget.x} y=${widget.y} h=${widget.height} ");
    Color adjustedColor=baseColor.withOpacity(opacity);
    return adjustedColor;
  }

  Widget getNumber() {
    return Text(widget.height.toString());
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
