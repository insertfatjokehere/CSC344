# Assignment 1 (C)

Write a program that

 * Checks that all "begin" symbols in a C program are matched with their "end" symbols `{...}`, `[...]`, `(...)`, `"..."`, and `'...'`, and if not, reports excess, missing, or improperly nested ones.
 * Counts the number of identifiers (including keywords). For example, reporting 7 for 
 
```c
void func(int arg) { int index = 27; index += 1; }
```

You can use any C library functions except for regular expression or parse libraries in your program.
