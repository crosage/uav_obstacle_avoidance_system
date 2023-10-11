import 'package:flutter/material.dart';
import 'package:spfa/create_maze.dart';
import 'package:spfa/result_list.dart';
import 'block.dart';
import 'dart:io';
import "config.dart";
import 'dart:convert';
import 'package:file_picker/file_picker.dart';
import "package:motion_toast/motion_toast.dart";

List<List<List<int>>> convertDynamicList(List<dynamic> dynamicList) {
  List<List<List<int>>> intList = [];
  for (var dynamicElement in dynamicList) {
    List<List<int>> nestedList = [];
    for (var innerList in dynamicElement) {
      List<int> intInnerList = [];
      for (var value in innerList) {
        intInnerList.add(value as int);
      }
      nestedList.add(intInnerList);
    }
    intList.add(nestedList);
  }
  return intList;
}

class Sidebar extends StatefulWidget {
  final Function(int, int, List<List<int>>) onCanvasChange;
  final Function(List<List<int>>) getBlockState;

  Sidebar(this.onCanvasChange, this.getBlockState);

  @override
  _SidebarState createState() => _SidebarState();
}

class _SidebarState extends State<Sidebar> {
  List<int> a = [];
  List<List<int>> maze = [];
  List<List<int>> blockState = [];
  List<List<List<int>>> pathData = [];
  int n = 1, m = 1;

  Future<String> runSystemCommand(
      String command, List<dynamic> arguments) async {
    print(command);
    final process = await Process.run(command, []);
    print(process);
    final output = await process.stdout;
    final error = await process.stderr;
    print(output);
    print(error);
    return output;
  }

  Future<void> _runDfs() async {
    final path = mazeRunPath;
    print(path);
    await Process.run(path, []);
    final result = await runSystemCommand(path, []);
    final resultPath = resultSavePath;
    print("begin\n");
    try {
      final file = File(resultPath!);
      if (await file.exists()) {
        String content = await file.readAsString();
        Map<String, dynamic> jsonData = json.decode(content);
        print(jsonData);
        pathData.clear();
        pathData = convertDynamicList(jsonData["paths"]);
        MotionToast.success(
                title: Text("Success"), description: Text("dfs运行结束"))
            .show(context);
      }
    } catch (e) {
      MotionToast.error(title: Text("Erroe"), description: Text("发生了一些错误:\n$e"))
          .show(context);
      print(e);
    }
  }

  Future<void> _readMaze(int useDefault) async {
    try {
      String filePath = "";
      if (useDefault == 0) {
        final result = await FilePicker.platform.pickFiles(
          type: FileType.custom,
          allowedExtensions: ["json"],
        );
        if (result != null) {
          filePath = result.files.single.path!;
        }
      } else {
        filePath = mazeSavePath;
      }
      final file = File(filePath!);
      if (await file.exists()) {
        String content = await file.readAsString();
        Map<String, dynamic> jsonData = json.decode(content);
        maze.clear();
        blockState.clear();
        final mazeData = jsonData["maze"];
        for (final row in mazeData) {
          if (row is List<dynamic>) {
            final List<int> intRow = [];
            final List<int> preState = [];
            for (final element in row) {
              if (element is int) {
                intRow.add(element);
                preState.add(10000);
              }
            }
            maze.add(intRow);
            blockState.add(preState);
          }
        }
        n = jsonData["n"];
        m = jsonData["m"];
        widget.getBlockState(blockState);
        widget.onCanvasChange(n, m, maze);
        MotionToast.success(
                title: Text("Success"), description: Text("创建了一个$n*$m的表格"))
            .show(context);
      }
    } catch (e) {
      MotionToast.error(title: Text("Erroe"), description: Text("发生了一些错误:\n$e"))
          .show(context);
      print(e);
    }
  }

  // final file=File("./maze.json");

  void _createMaze(BuildContext context) {
    int n = 0;
    int m = 0;
    List<List<int>> maze = [];
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return CreateMazeDialog();
      },
    ).then(
      (value) {
        _readMaze(1);
      },
    );
    // _readMaze(1);
  }

  void _dealSelectPath(List<List<int>> path) {
    setState(() {
      for (int i = 0; i < blockState.length; i++) {
        for (int j = 0; j < blockState[i].length; j++) {
          blockState[i][j] = 0;
        }
      }
      for (List<int> item in path) {
        blockState[item[0]][item[1]] = 1;
      }
      print(blockState);
      widget.getBlockState(blockState);
    });
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      width: 250,
      child: Column(
        mainAxisAlignment: MainAxisAlignment.start,
        children: [
          InkWell(
            onTap: () {},
            child: Container(
              height: 60,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.center,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  Icon(
                    Icons.menu,
                    size: 30,
                  ),
                  Text(
                    "操作与选项",
                    style: TextStyle(fontSize: 20),
                  )
                ],
              ),
            ),
          ),
          Divider(),
          InkWell(
            onTap: () {
              _readMaze(0);
            },
            child: Container(
              height: 50,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.start,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  Icon(
                    Icons.file_upload_outlined,
                    color: Colors.blueAccent,
                  ),
                  Text("从文件中导入数据")
                ],
              ),
            ),
          ),
          Divider(),
          InkWell(
            onTap: () {
              _createMaze(context);
            },
            child: Container(
              height: 50,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.start,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  Icon(
                    Icons.touch_app,
                    color: Colors.blueAccent,
                  ),
                  Text("手动创建一张表")
                ],
              ),
            ),
          ),
          Divider(),
          InkWell(
            onTap: () {
              _runDfs();
              print("############");
            },
            child: Container(
              height: 50,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.start,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  Icon(
                    Icons.looks_one_outlined,
                    color: Colors.blueAccent,
                  ),
                  Text("使用dfs运行")
                ],
              ),
            ),
          ),
          Divider(),
          InkWell(
            onTap: () {
              showDialog(
                  context: context,
                  builder: (BuildContext context) {
                    return resultList(
                      paths: pathData,
                      returnSelectPath: _dealSelectPath,
                    );
                  });
            },
            child: Container(
              height: 50,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.start,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  Icon(
                    Icons.navigate_next,
                    color: Colors.blueAccent,
                  ),
                  Text("查看dfs运行结果")
                ],
              ),
            ),
          ),
          Divider(),
          InkWell(
            onTap: () {
              _runDfs();
              // print("############");
            },
            child: Container(
              height: 50,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.start,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  Icon(
                    Icons.looks_two_outlined,
                    color: Colors.blueAccent,
                  ),
                  Text("使用A*运行")
                ],
              ),
            ),
          ),
          Divider(),
        ],
      ),
    );
  }
}
