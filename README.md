# CS 360 - Systems Programming

<h1> Assignment 3:  <p> count readable regular files of a path </p></h1>

02/17/2021

<h2> Instruction: </h2>

<p>"Write a program (in C) called assignment3.c. The program will recursively traverse the directories and their files, 
starting with the specified path, and count every regular file that is readable by the current process."</p>

<h3> Program Interface (Required) </h3>

<h4> int readable(char *inputPath) </h4>
<p>"Where: <inputPath> is a parameter that specifies a path in the directory system.
If no parameter is present (NULL), then your program will operate on the current
working directory. Your function should return the number of regular files that
are readable."</p>
    
<h5> functions:  chdir, getcwd, access, opendir, readdir, closedir, lstat </h5>
