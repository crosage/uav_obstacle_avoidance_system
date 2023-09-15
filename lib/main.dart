import 'package:flutter/material.dart';
import 'package:spfa/canvas.dart';
import 'sidebar.dart';

void main() {
  runApp(MyApp());
}
class MyApp extends StatefulWidget{
  @override
  _MyAppState createState()=> _MyAppState();

}
class _MyAppState extends State<MyApp> {
  int row=1,col=1,step=1;
  List<List<int>> maze=[[0]];
  List<List<int>> blockState=[[0]];
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
  void _handlePathData(List<List<List<int>>> pathData){
    setState(() {
      paths=pathData;
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
            Canvas(n: row, m: col,maze: maze,blockStates: blockState,depth: step,),
            Spacer(),
            VerticalDivider(),
            Sidebar(_handle_n_m,_dealBlockState,_handlePathData)
          ],
        ),
      ),
    );
  }
}
