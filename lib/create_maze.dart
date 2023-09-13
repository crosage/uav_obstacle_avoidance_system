import 'dart:convert';
import 'dart:io';
import 'package:flutter/material.dart';
import 'package:spfa/config.dart';

class CreateMazeDialog extends StatefulWidget {
  @override
  _CreateMazeDialogState createState() => _CreateMazeDialogState();
}

class _CreateMazeDialogState extends State<CreateMazeDialog> {
  int n = 0;
  int m = 0;
  List<List<int>> maze = [];

  Widget buildTextFieldGrid(int n, int m, List<List<int>> maze) {
    maze.clear();
    for(int rowIndex=0;rowIndex<n;rowIndex++){
      List<int> l=[];
      for(int colIndex=0;colIndex<m;colIndex++){
        l.add(0);
      }
      maze.add(l);
    }
    List<Widget> rows = [];
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
                        };
                        final jsonString=jsonEncode(dataToSave);
                        final file=File(mazeSavePath);
                        file.writeAsString(jsonString);
                        Navigator.of(context).pop();
                      },
                      icon: Icon(Icons.save))
                ],
              )
            ],
          ),
        ),
      ),
    );
  }
}
