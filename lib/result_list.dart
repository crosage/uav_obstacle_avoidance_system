import 'package:flutter/material.dart';

class resultList extends StatefulWidget {
  final List<List<List<int>>> paths;
  final Function(List<List<int>>) returnSelectPath;
  resultList({required this.paths,required this.returnSelectPath});

  @override
  _resultListState createState() => _resultListState();
}

class _resultListState extends State<resultList> {
  String getPath(List<List<int>> path){
    String result="Points: ";
    for(List<int> item in path){
      result=result+" ${item[0]},${item[1]}->";
    }
    return result;
  }
  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      content: Container(
        width: 700,
        child: ListView.separated(
          itemCount: widget.paths.length,
          separatorBuilder: (context, index) => Divider(),
          itemBuilder: (context, index) {
            final List<List<int>> path = widget.paths[index];
            // return Text("data");
            return ListTile(
                title: Text("Path $index Length: ${path.length}"),
                subtitle: Text(getPath(path)),
                onTap: (){
                  widget.returnSelectPath(path);
                  Navigator.of(context).pop();
                },
            );
          },
        ),
      ),
    );
  }
}
