# CPM80-sudoku

CPM80-sudoku is a basic sudoku puzzle solver for CP/M, and was specifically designed for the Altair 8800.   The program assumes the terminal is 80x24. 

The program solves using a bruit force solver, but tries some optimizations to reduce the time it takes to bruit force.

 1. Looks for Naked Singles
 2. Looks for Hidden Singles
 3. Precaches posible solution matrix to reduce testiing during bruit force
 4. Sorts blanks with with fewest possible values first as part of a MRV strategy.

Written for the Aztec C compiler and is compiled directly on an Alatair 8800 clone.   
Recommended compiler commands

Compile:

```

cc -Q -F sudoku

```

Assemble:

```

as sudoku

```

Link

```

ln sudoku.o m.lib c.lib

```