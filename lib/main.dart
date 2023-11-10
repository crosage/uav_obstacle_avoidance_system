import 'package:flutter/material.dart';
import 'package:spfa/components/canvas.dart';
import 'sidebar.dart';
// 由sidebar读取数据，回传到父组件main，由main传递给canvas
void main() {
  runApp(MyApp());
}
class MyApp extends StatefulWidget{
  @override
  _MyAppState createState()=> _MyAppState();

}
class _MyAppState extends State<MyApp> {
  int row=1,col=1;
  List<List<int>> maze=[];
  List<List<int>> blockState=[];
  List<List<List<int>>> paths=[[[0]]];
  void _handle_n_m(int x,int y,List<List<int>> maze_data){
    setState(() {
      row=x;
      col=y;
      maze=maze_data;
    });
  }
  void _dealBlockState(List<List<int>> blockData){
    setState(() {
      blockState=blockData;
    });
  }
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        primaryColor: Color(0xFF5bc2e7),
        useMaterial3: true,
      ),
      home: Scaffold(
        appBar: AppBar(
          backgroundColor: Color(0xFF5bc2e7),
          title: Text("无人机避障系统"),
        ),
        body: Row(
          children: [
            Spacer(),
            if(maze.isNotEmpty&&blockState.isNotEmpty)
              Canvas(n: row, m: col,maze: maze,blockStates: blockState,)
            else
              Spacer(),
            Spacer(),
            VerticalDivider(),
            Sidebar(_handle_n_m,_dealBlockState)
          ],
        ),
      ),
    );
  }
}
