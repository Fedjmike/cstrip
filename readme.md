cstrip
======

A simple program to strip whitespace and comments from C family source files. Similar to [JSMin](http://www.crockford.com/javascript/jsmin.html) by Douglas Crockford.

Usage
-----

    cstrip <file>
    
Prints the result to stdout.

Examples
--------

    $ cstrip cc.c | wc -c
    10982
    $ cstrip c4.c | wc -c
    12731

Todo
----

- Avoid removing whitespace between operators that form a different, compound operator.
- Add configuration options:
  - Leave comments
  - Leave whitespace
  - Leave newlines
  - Leave indentation
  - Language
