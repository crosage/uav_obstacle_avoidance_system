import 'package:flutter/material.dart';
import 'package:spfa/components/canvas.dart';
import 'package:spfa/components/canvas_with_height.dart';
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
  int _3d=0;
  int startX=0,startY=0,endX=0,endY=0;
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
  void _choose_3d(){
    setState(() {
      _3d=1-_3d;
      print("**********");
    });
  }
  void update_start_end(int startx,int starty,int endx,int endy){
    startX=startx;
    startY=starty;
    endX=endx;
    endY=endy;
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
            if(maze.isNotEmpty&&blockState.isNotEmpty&&_3d!=1)
              Canvas(n: row, m: col,maze: maze,blockStates: blockState,)
            else if (_3d==1)
              CanvasWithHeight(n:row,m: col,maze: maze,blockStates: blockState,)
            else
              Spacer(),
            Spacer(),
            VerticalDivider(),
            Sidebar(_handle_n_m,_dealBlockState,_choose_3d)
          ],
        ),
      ),
    );
  }
}
