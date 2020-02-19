# Golite

## Group Members
- Matthew Lesko 260692352
- Braedon McDonald 260753678
- Sophia Hu 260705681

## Rebasing on master
```
$ git fetch origin
$ git rebase origin/master
```

## Instructions
The tests that you must evaluate contain the prefix `meaningful*.go`. Please ignore the rest.
Please evaluate only the `.go` programs that begin with the prefix `meaningful*.go` in `programs/`. The rest are for our tests.

## Coding convention:

`branching`: Branch naming should follow convention `name`-`feature/bug`-`short_title`, eg: `sophia-feature-scanner` where possible.

## Report:

[Report Link.](https://docs.google.com/document/d/1H60gGayERn9IGiQ-lXdMc65WfG8EbcELD4KvlptWegU/edit?usp=sharing)

## Resources Consulted:
[Optional Semi-colons.](https://github.com/comp520/Examples/blob/master/flex%2Bbison/optional-semicolon/tiny.l)

# Assignment Template

To organize your assignments and project for class, please follow the skeleton provided in this repository

We provide 3 scripts for convenience and two directories for organization:

* `programs`: Test programs are organized by compilation phase and by expected result. Valid programs should output `OK` and status code `0`, while invalid programs should output `Error: <description>` and status code `1`.
  * `Scan+parse`: Runs both the scanner and parser phases
  * `Typecheck`: Runs until the end of the typechecker phase
  * `Codegen`: Runs until your compiler outputs the target code
* `src`: Source code for your assignment
* `build.sh`: Builds your compiler using `Make` or similar. You should replace the commands here if necessary to build your compiler
* `run.sh`: Runs your compiler using two arguments (mode - $1 and input file - $2). You should replace the commands here if necessary to invoke your compiler
* `test.sh`: Automatically runs your compiler against test programs in the programs directory and checks the output

Comments found in both files provide more details as to the requirements. Since a large portion of grading is performed automatically, please ensure to follow the input/output specifications **exactly**. You must be able to run the provided scripts on the SOCS Trottier machines.
