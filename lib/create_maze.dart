import 'dart:convert';
import 'dart:io';
import 'package:elegant_notification/elegant_notification.dart';
import 'package:elegant_notification/resources/arrays.dart';
import 'package:flutter/material.dart';
import 'package:spfa/config.dart';

class CreateMazeDialog extends StatefulWidget {
  @override
  _CreateMazeDialogState createState() => _CreateMazeDialogState();
}

class _CreateMazeDialogState extends State<CreateMazeDialog> {
  int n = 0;
  int m = 0;
  int startX=0,startY=0,endX=0,endY=0;
  List<List<int>> maze = [];

  Widget buildTextFieldGrid(int n, int m, List<List<int>> maze) {
    maze.clear();
    for (int rowIndex = 0; rowIndex < n; rowIndex++) {
      List<int> l = [];
      for (int colIndex = 0; colIndex < m; colIndex++) {
        l.add(0);
      }
      maze.add(l);
    }
    List<Widget> rows = [];
    rows.add(
      Row(
        children: [
          Expanded(child: TextField(
            decoration: InputDecoration(
              labelText: '输入起点的X坐标',
              labelStyle: TextStyle(
                color: Colors.grey,
              ),
            ),
            onChanged: (value){
              startX=int.tryParse(value)??0;
            },
          ),),
          Expanded(child: TextField(
            decoration: InputDecoration(
              labelText: '输入起点的Y坐标',
              labelStyle: TextStyle(
                color: Colors.grey,
              ),
            ),
            onChanged: (value){
              startY=int.tryParse(value)??0;
            },
          ),),
          Expanded(child: TextField(
            decoration: InputDecoration(
              labelText: '输入终点的X坐标',
              labelStyle: TextStyle(
                color: Colors.grey,
              ),
            ),
            onChanged: (value){
              endX=int.tryParse(value)??0;
            },
          ),),
          Expanded(child: TextField(
            decoration: InputDecoration(
              labelText: '输入终点的Y坐标',
              labelStyle: TextStyle(
                color: Colors.grey,
              ),
            ),
            onChanged: (value){
              endY=int.tryParse(value)??0;
            },
          ),),

        ],
      )
    );
    for (int rowIndex = 0; rowIndex < n; rowIndex++) {
      List<Widget> cols = [];
      for (int colIndex = 0; colIndex < m; colIndex++) {
        cols.add(
          Flexible(
            child: TextField(
              decoration: InputDecoration(
                labelText: '($rowIndex, $colIndex)',
                labelStyle: TextStyle(
                  color: Colors.grey,
                ),
              ),
              keyboardType: TextInputType.number,
              onChanged: (value) {
                maze[rowIndex][colIndex] = int.tryParse(value) ?? 0;
              },
            ),
          ),
        );
      }
      rows.add(
        Row(
          children: cols,
        ),
      );
    }
    return Column(
      children: rows,
    );
  }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: Text("创建表"),
      content: Container(
        width: 2000,
        child: SingleChildScrollView(
          child: Column(
            children: [
              TextField(
                decoration: InputDecoration(labelText: "输入N"),
                onChanged: (value) {
                  setState(() {
                    n = int.tryParse(value) ?? 0;
                  });
                },
              ),
              TextField(
                decoration: InputDecoration(labelText: "输入M"),
                onChanged: (value) {
                  setState(() {
                    m = int.tryParse(value) ?? 0;
                  });
                },
              ),
              Text("输入矩阵值:"),
              buildTextFieldGrid(n, m, maze),
              Row(
                mainAxisAlignment: MainAxisAlignment.start,
                children: [
                  IconButton(
                    onPressed: () {
                      final dataToSave = {
                        "n": n,
                        "m": m,
                        "maze": maze,
                        "start":[startX,startY],
                        "end":[endX,endY]
                      };
                      final jsonString = jsonEncode(dataToSave);
                      final file = File(mazeSavePath);
                      file.writeAsString(jsonString);
                      ElegantNotification.success(
                        width: 70,
                        // background: Colors.grey[200]!,
                        title: Text("info"),
                        description: Text("创建了一张$n*$m的表格\n  已保存至maze.json"),
                        animation: AnimationType.fromRight,
                        notificationPosition: NotificationPosition.bottomRight,
                      ).show(context);
                      Navigator.of(context).pop();
                    },
                    icon: Icon(Icons.save),
                  )
                ],
              )
            ],
          ),
        ),
      ),
    );
  }
}
