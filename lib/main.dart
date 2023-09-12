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
  int row=1,col=1;
  List<List<int>> maze=[[0]];
  List<List<int>> block_state=[[0]];
  void _handle_n_m(int x,int y,List<List<int>> maze_data,List<List<int>> dfs_data){
    setState(() {
      row=x;
      col=y;
      maze=maze_data;
      block_state=dfs_data;
    });
  }
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
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
            canvas(n: row, m: col,maze: maze,block_states: block_state,),
            Spacer(),
            VerticalDivider(),
            sidebar(_handle_n_m)
          ],
        ),
      ),
    );
  }
}
