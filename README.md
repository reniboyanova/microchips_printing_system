# S05-Team1

Common conventions for the project : 

The #define Guard
All header files should have #define guards to prevent multiple inclusion. The format of the symbol name should be <PROJECT>_<PATH>_<FILE>_H_.

To guarantee uniqueness, they should be based on the full path in a project's source tree. For example, the file foo/src/bar/baz.h in project foo should have the following guard:

#ifndef FOO_BAR_BAZ_H_
#define FOO_BAR_BAZ_H_

...

#endif  // FOO_BAR_BAZ_H_
  
  
Class Data Members
Data members of classes, both static and non-static, are named with m_ in front.

class TableInfo {
  ...
 private:
  std::string m_table;  // OK - underscore at end.
  static Pool<TableInfo>* m_pool;  // OK.
  int quantity quantity // NOT OK
};
  
Function Names
Regular functions have mixed case; accessors and mutators may be named like variables.

Ordinarily, functions should start with a lower letter and have a capital letter for each new word.
  
addTableEntry()
deleteUrl()
openFileOrDie()
