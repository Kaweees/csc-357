# CSC 357

[<img src="assets/img/c-logo.png" align="right" width="150">](https://github.com/Kaweees/csc-357)


Code formatting can be manually called by (<kbd>alt</kbd>+<kbd>shift</kbd>+<kbd>f</kbd>)


Linux
```sh
sudo apt install clang-format
```

Windows
```ps1
winget install -e --id LLVM.LLVM
```

I tend to be pedantic about how I write code. Here is how I name my variables:
```c
/* Preprocessor directives are in all caps delimited by underscores */
MY_DEFINE
/* Types are in capital case */
MyType
/* Functions are in camel case */
myFunction()
/* variables are in snake case */
my_variable
```
