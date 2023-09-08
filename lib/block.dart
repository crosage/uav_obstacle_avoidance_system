import 'package:flutter/material.dart';

class block extends StatefulWidget {
  final int x;
  final int y;
  final double block_width;
  final double block_height;
  final int is_block;

  const block(
      {super.key,
      required this.x,
      required this.y,
      required this.block_height,
      required this.block_width,
      required this.is_block});

  @override
  _BlockState createState() => _BlockState();
}

class _BlockState extends State<block> {
  late int have_visit;
  late bool been_choose;
  @override
  void initState(){
    super.initState();
    been_choose=false;
  }
  @override
  Widget build(BuildContext context) {
    return InkWell(
        onTap: () {
          setState(() {
            been_choose=!been_choose;
          });
          // been_choose=been_choose;
        },
        child: AnimatedContainer(
          duration: Duration(seconds: 3),
          // height: widget.block_height,
          // width: widget.block_width,
          color: been_choose?Colors.grey:Colors.white70,
          child: Center(
            child: Text("0"),
          ),
        ));
  }
}
