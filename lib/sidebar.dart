import 'package:elegant_notification/elegant_notification.dart';
import 'package:elegant_notification/resources/arrays.dart';
import 'package:flutter/material.dart';
import 'package:spfa/components/canvas_with_height.dart';
import 'package:spfa/components/elevated_button.dart';
import 'package:spfa/create_maze.dart';
import 'package:spfa/result_list.dart';
import 'components/block.dart';
import 'dart:math';
import 'dart:io';
import "config.dart";
import 'dart:convert';
import 'package:file_picker/file_picker.dart';

// import "package:motion_toast/motion_toast.dart";
List<List<List<int>>> convertDynamicList(List<dynamic> dynamicList) {
  print("dynamicList");
  print(dynamicList);
  List<List<List<int>>> intList = [];
  for (var dynamicElement in dynamicList) {
    List<List<int>> nestedList = [];
    for (var innerList in dynamicElement) {
      List<int> intInnerList = [];
      print(innerList);
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
  int startX = 1, startY = 1, endX = 1, endY = 1;

  Future<String> runSystemCommand(
      String command, List<String> arguments) async {
    // print(command);
    print("**********");
    print(command);
    print(arguments);
    print("**********");
    final process = await Process.run(command, arguments);
    // print(process);
    final output = await process.stdout;
    final error = await process.stderr;
    print(output);
    print(error);
    return output;
  }
  List<List<int>> generateRandomMatrix(int rows, int columns) {
    final Random random = Random();
    List<List<int>> matrix = List.generate(rows, (i) => List.generate(columns, (j) => random.nextInt(100)));
    return matrix;
  }
  void saveMatrixToFile(List<List<int>> matrix, String filePath) {
    File file = File(filePath);
    IOSink sink = file.openWrite();
    try {
      for (List<int> row in matrix) {
        String rowString = row.join(' ');
        sink.writeln(rowString);
      }
    } finally {
      sink.close();
    }
    print('Matrix has been saved to $filePath');
  }
  void generateMazeWithHeight(){
    int maxRows=30,maxCols=30;
    List<List<int>> matrix = generateRandomMatrix(maxRows, maxCols);
  }

  Future<void> _runDfs() async {
    final path = mazeRunPath;
    print(path);
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
        ElegantNotification.info(
          width: 70,
          // background: Colors.grey[200]!,
          title: Text("info"),
          description: Text("dfs运行结束"),
          animation: AnimationType.fromRight,
          notificationPosition: NotificationPosition.bottomRight,
        ).show(context);
      }
    } catch (e) {
      ElegantNotification.error(
        width: 70,
        // background: Colors.grey[200]!,
        title: Text("info"),
        description: Text("发生错误:\n$e"),
        animation: AnimationType.fromRight,
        notificationPosition: NotificationPosition.bottomRight,
      ).show(context);
      print(e);
    }
  }

  Future<void> _generate() async {
    // final resultPath = resultSavePath;
    try {
      final path = generatePath;
      print(path);
      final result = await runSystemCommand(path, []);
      ElegantNotification.success(
        width: 70,
        // background: Colors.grey[200]!,
        title: Text("info"),
        description: Text("随机生成完成"),
        animation: AnimationType.fromRight,
        notificationPosition: NotificationPosition.bottomRight,
      ).show(context);
    } catch (e) {
      ElegantNotification.error(
        width: 70,
        // background: Colors.grey[200]!,
        title: Text("info"),
        description: Text("发生错误:\n$e"),
        animation: AnimationType.fromRight,
        notificationPosition: NotificationPosition.bottomRight,
      ).show(context);
    }
  }

  Future<void> _generateWithHeight() async {
    // final resultPath = resultSavePath;
    try {
      final path = generateWithHeightPath;
      print(path);
      final result = await runSystemCommand("python", [path]);
      ElegantNotification.success(
        width: 70,
        // background: Colors.grey[200]!,
        title: Text("info"),
        description: Text("随机生成完成"),
        animation: AnimationType.fromRight,
        notificationPosition: NotificationPosition.bottomRight,
      ).show(context);
    } catch (e) {
      ElegantNotification.error(
        width: 70,
        // background: Colors.grey[200]!,
        title: Text("info"),
        description: Text("发生错误:\n$e"),
        animation: AnimationType.fromRight,
        notificationPosition: NotificationPosition.bottomRight,
      ).show(context);
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
      // print("****************");
      if (await file.exists()) {
        String content = await file.readAsString();
        Map<String, dynamic> jsonData = json.decode(content);
        maze.clear();
        blockState.clear();
        final mazeData = jsonData["maze"];
        startX = jsonData["start"][0];
        startY = jsonData["start"][1];
        endX = jsonData["end"][0];
        endY = jsonData["end"][1];
        for (final row in mazeData) {
          if (row is List<dynamic>) {
            final List<int> intRow = [];
            final List<int> preState = [];
            for (final element in row) {
              if (element is int) {
                intRow.add(element);
                preState.add(0);
              }
            }
            maze.add(intRow);
            blockState.add(preState);
          }
        }
        n = jsonData["n"];
        m = jsonData["m"];
        maze[startX][startY] = -2;
        maze[endX][endY] = -3;
        widget.getBlockState(blockState);
        widget.onCanvasChange(n, m, maze);
        // ElegantNotification.info(description: )
        ElegantNotification.success(
          width: 70,
          title: Text("info"),
          description: Text("创建了一个$n*$m的表格"),
          animation: AnimationType.fromRight,
          notificationPosition: NotificationPosition.bottomRight,
        ).show(context);
      }
    } catch (e) {
      ElegantNotification.error(
        width: 70,
        title: Text("info"),
        description: Text("$e"),
        animation: AnimationType.fromRight,
        notificationPosition: NotificationPosition.bottomRight,
      ).show(context);
      print(e);
    }
  }

  Future<void> _runAstar() async {
    final path = astarRunPath;
    final result = await runSystemCommand(path, []);
    final resultPath = astarResultSavePath;
    try {
      final file = File(resultPath!);
      if (await file.exists()) {
        String content = await file.readAsString();
        Map<String, dynamic> jsonData = json.decode(content);
        pathData.clear();
        List<dynamic> l = [];
        l.add(jsonData["astar"]);
        pathData = convertDynamicList(l);
        ElegantNotification.info(
          width: 70,
          title: Text("info"),
          description: Text("astar运行结束"),
          animation: AnimationType.fromRight,
          notificationPosition: NotificationPosition.bottomRight,
        ).show(context);
      }
    } catch (e) {
      ElegantNotification.error(
        width: 70,
        title: Text("info"),
        description: Text("发生错误:\n$e"),
        animation: AnimationType.fromRight,
        notificationPosition: NotificationPosition.bottomRight,
      ).show(context);
      print(e);
    }
  }
  Future<void> _runRrt() async {
    final path = rrtstarRunPath;
    final result = await runSystemCommand(path, []);
    final resultPath = rrtstarSavePath;
    try {
      final file = File(resultPath!);
      if (await file.exists()) {
        String content = await file.readAsString();
        Map<String, dynamic> jsonData = json.decode(content);
        pathData.clear();
        List<dynamic> l = [];
        l.add(jsonData["path"]);
        pathData = convertDynamicList(l);
        ElegantNotification.info(
          width: 70,
          // background: Colors.grey[200]!,
          title: Text("info"),
          description: Text("rrtstar运行结束"),
          animation: AnimationType.fromRight,
          notificationPosition: NotificationPosition.bottomRight,
        ).show(context);
      }
    } catch (e) {
      ElegantNotification.error(
        width: 70,
        // background: Colors.grey[200]!,
        title: Text("info"),
        description: Text("发生错误:\n$e"),
        animation: AnimationType.fromRight,
        notificationPosition: NotificationPosition.bottomRight,
      ).show(context);
      print(e);
    }
  }

  Future<void> _runDstar() async {
    final path = dstarRunPath;
    final result = await runSystemCommand("python", ["${path}"]);
    final resultPath = dstarResultSavePath;
    try {
      final file = File(resultPath!);
      if (await file.exists()) {
        String content = await file.readAsString();
        Map<String, dynamic> jsonData = json.decode(content);
        pathData.clear();
        List<dynamic> l = [];
        l.add(jsonData["path"]);
        pathData = convertDynamicList(l);
        ElegantNotification.info(
          width: 70,
          // background: Colors.grey[200]!,
          title: Text("info"),
          description: Text("D*运行结束"),
          animation: AnimationType.fromRight,
          notificationPosition: NotificationPosition.bottomRight,
        ).show(context);
      }
    } catch (e) {
      ElegantNotification.error(
        width: 70,
        // background: Colors.grey[200]!,
        title: Text("info"),
        description: Text("发生错误:\n$e"),
        animation: AnimationType.fromRight,
        notificationPosition: NotificationPosition.bottomRight,
      ).show(context);
      print(e);
    }
  }
  void _createMaze(BuildContext context) {
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

      for (int i = 0; i < path.length; i++) {
        List<int> item = path[i];
        blockState[item[0]][item[1]] = i + 1;
      }

      print(blockState);
      widget.getBlockState(blockState);
    });
  }


  @override
  Widget build(BuildContext context) {
    return SingleChildScrollView(
        child:Container(
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
          if (n != 1 && m != 1)
            Container(
              height: 50,
              child: Row(
                mainAxisAlignment: MainAxisAlignment.start,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  Icon(
                    Icons.crop_square_sharp,
                    color: Colors.greenAccent,
                  ),
                  Text("当前为$n*$m的矩阵")
                ],
              ),
            ),
          if (n != 1 && m != 1) Divider(),
          IconLabelButton(
            onPress: () {
              _readMaze(0);
            },
            icon: Icon(
              Icons.file_upload_outlined,
              color: Colors.blueAccent,
            ),
            text: "从文件中导入数据",
          ),
          Divider(),
          IconLabelButton(
            onPress: () {
              _createMaze(context);
            },
            icon: Icon(
              Icons.touch_app,
              color: Colors.blueAccent,
            ),
            text: "手动创建一张表",
          ),
          Divider(),
          IconLabelButton(
            onPress: () {
              _runDfs();
              String currentDirectory = Directory.current.path;
              print('当前运行路径: $currentDirectory');
            },
            icon: Icon(
              Icons.looks_one_outlined,
              color: Colors.blueAccent,
            ),
            text: "使用dfs运行",
          ),
          Divider(),
          IconLabelButton(
            onPress: () {
              _runAstar();
            },
            icon: Icon(
              Icons.looks_two_outlined,
              color: Colors.blueAccent,
            ),
            text: "使用A*运行",
          ),
          Divider(),
          IconLabelButton(
            onPress: () {
              _runDstar();
            },
            icon: Icon(
              Icons.looks_3_outlined,
              color: Colors.blueAccent,
            ),
            text: "使用D*运行",
          ),
          Divider(),
          IconLabelButton(
            onPress: () {
              _runRrt();
            },
            icon: Icon(
              Icons.looks_4_outlined,
              color: Colors.blueAccent,
            ),
            text: "使用rrt*运行",
          ),
          Divider(),
          IconLabelButton(
            onPress: () {
              showDialog(
                context: context,
                builder: (BuildContext context) {
                  return CanvasWithHeight(n: n, m: m, maze: maze, blockStates: blockState);
                },
              );
            },
            icon: Icon(
              Icons.looks_5_outlined,
              color: Colors.blueAccent,
            ),
            text: "包含高度情况的拓展A*",
          ),
          Divider(),
          IconLabelButton(
            onPress: () {
              showDialog(
                context: context,
                builder: (BuildContext context) {
                  return resultList(
                    paths: pathData,
                    returnSelectPath: _dealSelectPath,
                  );
                },
              );
            },
            icon: Icon(
              Icons.navigate_next,
              color: Colors.blueAccent,
            ),
            text: "查看运行结果",
          ),
          Divider(),
          IconLabelButton(
            onPress: () {
              _generate();
            },
            icon: Icon(
              Icons.cached,
              color: Colors.blueAccent,
            ),
            text: "随机生成01矩阵",
          ),
          Divider(),
          IconLabelButton(
            onPress: () {
              _generateWithHeight();
            },
            icon: Icon(
              Icons.cached,
              color: Colors.blueAccent,
            ),
            text: "随机生成带高度矩阵",
          ),
        ],
      ),
    ),)
    ;
  }
}
