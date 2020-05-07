<div align="center">
    <img src='logo.png' width="70%">
</div>

***

## Introduction

This is a compiler built for a subset of Go, called **GoLite**. Golite includes many Go features, including:
- multiple variable declarations in one line
- short declarations
- untagged structs
- slices
- optional semi-colons. 

Our compiler is wrtiten in C, and we used the flex (fast lexical analyzer) and bison toolchains. Our target language is Java.

## Usage
```
./build.sh
./run.sh <mode> <file>
+ mode: scan|tokens|parse|pretty|symbol|typecheck|codegen
+ file: path to file (absolute or relative)
```
