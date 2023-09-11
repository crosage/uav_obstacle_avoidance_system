import 'package:flutter/material.dart';

class block extends StatefulWidget {
  final int x;
  final int y;
  final int block_state;

  const block(
      {super.key, required this.x, required this.y, required this.block_state});

  @override
  _BlockState createState() => _BlockState();
}

class _BlockState extends State<block> {
  late int have_visit;
  late bool been_visit;

  Color getBlockColor() {
    if (widget.block_state == 1) return Colors.black;
    if (!been_visit) {
      return Colors.transparent;
    }
    return Colors.black45;
  }

  @override
  void initState() {
    super.initState();
    been_visit = false;
  }

  @override
  Widget build(BuildContext context) {
    return InkWell(
        onTap: () {
          setState(() {
            been_visit = !been_visit;
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
