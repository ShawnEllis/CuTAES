
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

string error = "Error Occured in :";

int main(){
  testSubscript();
  testPlusEqualSingle();
  testPlusEqualQueue();
  testPlusSingle();
  testPlusQueue();

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
