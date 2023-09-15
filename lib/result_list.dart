import 'package:flutter/material.dart';

class resultList extends StatefulWidget{
  @override
  _resultListState createState()=> _resultListState();
}
class _resultListState extends State<resultList>{
  @override
  Widget build(BuildContext context){
    return AlertDialog(
      content: Container(
        width: 700,
        child: SingleChildScrollView(
          child: ListView(

          ),
        ),
      ),
    );
  }
}