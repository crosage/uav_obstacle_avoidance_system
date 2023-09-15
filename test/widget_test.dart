// This is a basic Flutter widget test.
//
// To perform an interaction with a widget in your test, use the WidgetTester
// utility in the flutter_test package. For example, you can send tap and scroll
// gestures. You can also use WidgetTester to find child widgets in the widget
// tree, read text, and verify that the values of widget properties are correct.

import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';

import 'package:spfa/main.dart';

void main() {
  Map<String, dynamic> data = {
    "maze": [
      [1, 0, 0],
      [0, 0, 0],
      [0, 0, 0],
    ],
    "paths": [
      [
        [0, 0],
        [1, 0],
        [2, 0],
        [2, 1],
        [2, 2],
      ],
      // 其他路径...
    ],
  };

  // 从数据中提取paths并存储在一个List中
  List<List<List<int>>> paths = (data["paths"] as List<dynamic>).map((path) {
    return (path as List<dynamic>).map((step) {
      return (step as List<dynamic>).cast<int>().toList();
    }).toList();
  }).toList();

  // 打印路径数据
  for (var path in paths) {
    print(path);
  }
  });
}
