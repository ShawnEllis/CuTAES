
#include <iostream>
#include "Queue.h"
#include <string>
#include "StringUtil.h"

#define ERROR_OC cout << "Error Occured in " << type << "at " << place << endl;

using namespace std;

void testSubscript();
void testPlusEqualSingle();
void testPlusEqualQueue();
void testPlusSingle();
void testPlusQueue();
void testMinusEqualSingle();
void testMinusEqualQueue();
void testMinusSingle();
void testMinusQueue();



string error = "Error Occured in :";

int main(){
  testSubscript();
  testPlusEqualSingle();
  testPlusEqualQueue();
  testPlusSingle();
  testPlusQueue();
  testMinusEqualSingle();
  testMinusEqualQueue();
  testMinusSingle();
  testMinusQueue();

  return 0;
}

void testSubscript(){
  cout << "Testing []" << endl;
  string type = "Subscript ";
  Queue<int> test;
  /*  Empty  */
  string place = "Empty ";
  if (test[0] != 0)
    ERROR_OC
//  cout << type << "Empty Test: " << (test[0] == 0? "null": StringUtil::itos(*test[0])) << endl;
  /*  First  */
  place = "First ";
  test.pushBack(2);
  if (test[0] == 0)
    ERROR_OC
//  cout << type << "First Test: " << (test[0] == 0? "null": StringUtil::itos(*test[0])) << endl;
  /*  Last  */
  place = "Last ";
  test.pushBack(3);
  if (test[1] == 0)
    ERROR_OC
//  cout << type << "First Test: " << (test[0] == 0? "null": StringUtil::itos(*test[0])) << endl;
//  cout << type << "Last Test: " << (test[1] == 0? "null" : StringUtil::itos(*test[1])) << endl;
  /*  Middle  */
  place = "Middle ";
  test.pushBack(4);
  if (test[1] == 0)
    ERROR_OC
//  cout << type << "First Test: " << (test[0] == 0? "null": StringUtil::itos(*test[0])) << endl;
//  cout << type << "Middle Test: " << (test[1] == 0? "null": StringUtil::itos(*test[1])) << endl;
//  cout << type << "Last Test: " << (test[2] == 0? "null" : StringUtil::itos(*test[2])) << endl;
  /*  Out Of Bounds*/
  place = "Out of Bounds ";
  if (test[5000] != 0)
    ERROR_OC
//  cout << type << "Out of Bounds Test " << (test[5000] == 0? "null" : StringUtil::itos(*test[5000])) << endl;
  cout << "Done Testing []" << endl;
}

void testPlusEqualSingle(){
  cout << "Testing += on Single values" << endl;
  string type = "Addition Assignment Single";
  Queue<int> test;
  int i;
  int *null = 0;
  /*  Empty += Null  */
  string place = "Empty += NULL ";
  test += null;
  if (test.getSize() != 0)
    ERROR_OC
  /*  Empty += Valid */
  place = "Empty += Valid ";
  i = 1;
  test += &i;
  if (test.getSize() != 1 || (*test[0] != 1))
    ERROR_OC
  /*  Non-Empty += Null  */
  place = "Non-Empty += NULL ";
  test += null;
  if (test.getSize() != 1 || (*test[0] != 1))
    ERROR_OC
  /*  Non-Empty += Valid  */
  place = "Non-Empty += Valid ";
  i = 2;
  test += &i;
  if (test.getSize() != 2 || (*test[1] != 2))
    ERROR_OC
  cout << "Done Testing += on Single values" << endl;
}

void testPlusEqualQueue(){
  cout << "Testing += on Queue values" << endl;
  string type = "Addition Assignment Queue";
  Queue<int> test1, test2, test3;
  int i;
  /*  Empty += Null  */
  string place = "Empty += NULL ";
  test1 += test2;
  if (test1.getSize() != 0)
    ERROR_OC
  /*  Empty += Valid */
  place = "Empty += Valid ";
  i = 1;
  test2 += &i;
  test1 += test2;
  if (test1.getSize() != 1 || (*test1[0] != 1))
    ERROR_OC
  /*  Non-Empty += Null  */
  place = "Non-Empty += Null ";
  test1 += test3;
  if (test1.getSize() != 1 || (*test1[0] != 1))
    ERROR_OC
  /*  Non-Empty += Valid  */
  place = "Non-Empty += NULL ";
  test1 += test2;
  if (test1.getSize() != 2 || (*test1[1] != 1))
    ERROR_OC
  cout << "Done Testing += on Queue values" << endl;
}

void testPlusSingle(){
  cout << "Testing + on Single values" << endl;
  string type = "Addition Single";
  Queue<int> test, toAdd;
  int *null = 0;
  int i;
  /*  Empty + Null  */
  string place = "Empty + NULL ";
  test = toAdd + null;
  if (test.getSize() != 0)
    ERROR_OC
  /*  Empty + Valid */
  place = "Empty + Valid ";
  i = 1;
  test = toAdd + &i;
  if (test.getSize() != 1 || (*test[0] != 1))
    ERROR_OC
  /*  Non-Empty + Null  */
  place = "Non-Empty + NULL ";
  test = test + null;
  if (test.getSize() != 1 || (*test[0] != 1))
    ERROR_OC
  /*  Non-Empty + Valid  */
  place = "Non-Empty + Valid ";
  test = test + &i;
  if (test.getSize() != 2 || (*test[1] != 1))
    ERROR_OC 
  cout << "Done Testing + on Single values" << endl;
}

void testPlusQueue(){
  cout << "Testing + on Queue values" << endl;
  string type = "Addition Single";
  Queue<int> test, toAdd, empty; 
  int i;
  /*  Empty + Null  */
  string place = "Empty + Empty ";
  test = test + toAdd;
  if (test.getSize() != 0)
    ERROR_OC
  /*  Empty + Valid */
  place = "Empty + Valid ";
  i = 1;
  toAdd += &i;
  test = empty + toAdd;
  if (test.getSize() != 1 || (*test[0] != 1))
    ERROR_OC
  /*  Non-Empty + Null  */
  place = "Non-Empty + Empty ";
  test = (test + empty);
  if (test.getSize() != 1 || (*test[0] != 1))
    ERROR_OC
  /*  Non-Empty + Valid  */
  place = "Non-Empty + Valid ";
  test = (test + test);
  if (test.getSize() != 2 || (*test[1] != 1))
    ERROR_OC 
  cout << "Done Testing + on Queue values" << endl;
}


void testMinusEqualSingle(){
  cout << "Testing -= on Single values" << endl;
  string type = "Subtraction Assignment Single";
  Queue<int> test;
  int intArr[] = {3, 2, 4, 5, 6};
  int i;
  int *null = 0;
  test = (test + &intArr[0] + &intArr[1] + &intArr[2] + &intArr[3] + &intArr[4]);
  
  /*  Non-Empty -= Null  */
  string place = "Non-Empty -= NULL ";
  test -= null;
  if (test.getSize() != 5)
    ERROR_OC
  /*  Non-Empty -= Valid (first)*/
  place = "Non-Empty -= Valid (first position)";
  i = 3;
  test -= &i;
  if (test.getSize() != 4 || (*test[0] != 2))
    ERROR_OC
  /*  Non-Empty -= Valid (last)*/
  place = "Non-Empty -= Valid (last position)";
  i = 6;
  test -= &i;
  if (test.getSize() != 3 || (*test[2] != 5))
    ERROR_OC
  /*  Non-Empty -= Valid (middle)*/
  place = "Non-Empty -= Valid (middle position)";
  i = 4;
  test -= &i;
  if (test.getSize() != 2 || (*test[0] != 2) || (*test[1] != 5))
    ERROR_OC
  /* Empty -= Null  */
  place = "Empty -= NULL ";
  test -= &intArr[1];
  test -= &intArr[3];
  test -= null;
  if (test.getSize() != 0)
    ERROR_OC
  /*  Empty -= Valid  */
  place = "Empty -= Valid ";
  test -= &i;
  if (test.getSize() != 0)
    ERROR_OC
  cout << "Done Testing -= on Single values" << endl;
}

void testMinusEqualQueue(){
  cout << "Testing -= on Queue values" << endl;
  string type = "Subtraction Assignment Queue";
  Queue<int> test1, test2, test3;
  int intArr[] = {3, 2, 4, 5, 6};
  test1 = (test1 + &intArr[0] + &intArr[1] + &intArr[2] + &intArr[3] + &intArr[4]);
  test2 = (test2 + &intArr[0] + &intArr[3] + &intArr[4]);
  
  /*  Non-Empty -= Null  */
  string place = "Empty -= NULL ";
  test1 -= test3;
  if (test1.getSize() != 5)
    ERROR_OC
  /*  Non-Empty -= Valid (partial)*/
  place = "Empty -= Valid (partial) ";
  test1 -= test2;  
  if (test1.getSize() != 2 || (*test1[0] != intArr[1]) || (*test1[1] != intArr[2]))
    ERROR_OC
  /*  Non-Empty -= Valid (complete)*/
  place = "Empty -= Valid (complete) ";
  test2.clear();
  test2 = (test2 + &intArr[1] + &intArr[2]);
  test1 -= test2;
  if (test1.getSize() != 0)
    ERROR_OC
  /*  Empty -= Null  */
  place = "Non-Empty -= Null ";
  test1 -= test3;
  if (test1.getSize() != 0)
    ERROR_OC
  /*  Empty -= Valid  */
  place = "Non-Empty -= NULL ";
  test1 -= test2;
  if (test1.getSize() != 0)
    ERROR_OC
  cout << "Done Testing -= on Queue values" << endl;
}

void testMinusSingle(){
  cout << "Testing - on Single values" << endl;
  string type = "Subtraction Single";
  Queue<int> test, toSub;
  int i;
  int *null = 0;
  int intArr[] = {3, 2, 4, 5, 6};
  toSub = (toSub + &intArr[0] + &intArr[1] + &intArr[2] + &intArr[3] + &intArr[4]); 
  /*  Non-Empty - Null  */
  string place = "Empty - NULL ";
  test = (toSub - null);
  if (test.getSize() != 5)
    ERROR_OC
  test.clear();
  /*  Non-Empty - Valid (first)*/
  place = "Empty - Valid (first) ";
  i = 3;
  test = (toSub - &i);
  if (test.getSize() != 4 || (*test[0] != 2))
    ERROR_OC
  test.clear();
  /*  Non-Empty - Valid (last)*/
  place = "Empty - Valid (last) ";
  i = 6;
  test = (toSub - &i);
  if (test.getSize() != 4 || (*test[3] != 5))
    ERROR_OC
  test.clear();
  /*  Non-Empty - Valid (middle)*/
  place = "Empty - Valid (middle) ";
  i = 4;
  test = (toSub - &i);
  if (test.getSize() != 4 || (*test[2] == 4))
    ERROR_OC
  test.clear();
  /*  Empty - Null  */
  place = "Non-Empty - NULL ";
  test = (test - null);
  if (test.getSize() != 0)
    ERROR_OC
  test.clear();
  /*  Empty - Valid  */
  place = "Non-Empty - Valid ";
  test = (test - &i);
  if (test.getSize() != 0)
    ERROR_OC
  cout << "Done Testing - on Single values" << endl;
}

void testMinusQueue(){
  cout << "Testing - on Queue values" << endl;
  string type = "Subtraction Single";
  Queue<int> test1, test2, test3;
  int intArr[] = {3, 2, 4, 5, 6};
  test1 = (test1 + &intArr[0] + &intArr[1] + &intArr[2] + &intArr[3] + &intArr[4]);
  test2 = (test2 + &intArr[0] + &intArr[3] + &intArr[4]); 
  
  /*  Non-Empty - Null  */
  string place = "Empty - Empty ";
  test3 = test1 - test3;
  if (test3.getSize() != test1.getSize())
    ERROR_OC
  test3.clear();
  /*  Non-Empty - Valid (partial) */
  place = "Empty - Valid (partial) ";
  test3 = test1 - test2;
  if (test3.getSize() != (test1.getSize() - test2.getSize()) || (*test3[0] != intArr[1]) || (*test3[1] != intArr[2]))
    ERROR_OC
  test3.clear();
  /*  Non-Empty - Valid (complete) */
  place = "Empty - Valid (complete) ";
  test3 = test1 - test1;
  if (test3.getSize() != 0)
    ERROR_OC
  test3.clear();
  /*  Empty - Null  */
  place = "Non-Empty - Empty ";
  test3 = test3 - test3;
  if (test3.getSize() != 0)
    ERROR_OC
  test3.clear();
  /*  Empty - Valid  */
  place = "Non-Empty - Valid ";
  test3 = test3 - test2;
  if (test3.getSize() != 0)
    ERROR_OC
  cout << "Done Testing - on Queue values" << endl;
}
