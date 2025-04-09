#include "stdio.h" 
#include "ctype.h"
#include "math.h"
void splash();
void initLookup();
void inputPuz();
void drawPuzzle();
void findBlanks();
void simplify();
char nakedSingle();
char hiddenSingle();
char nakedPair();
void showComp();
void optOrder();
void fillBlank();
void adrFillBlank();
char posPurge();
char rowPurge();
char colPurge();
char celPurge();
char testAdr();
/* void setCursorXY(int X, int Y)*/
/* sets the cursor to X,Y position*/
/* X and Y are 1 based*/
void setCursorXY();
/* void screenClear()*/
/* clears the screen*/
void screenClear();
/* void drawGrid()*/
/* draws the grid of the puzzle*/
void drawGrid();
void drawSeg();

void offsetCursor();
/* void swPrint(char *st)*/
/* prints string st in next line of side window*/
void swPrint();
/* void swClear()*/
/* erases after Y position for all lines that have text*/
void swClear();
int solve();
char getKeypress();

void sbPrint();
/* puzzle storage*/
/* modified during solver use oringinal blanks stored in blanks*/
char puzzle[81];
/*blanks[0] = number of blanks*/
/*blanks[n] = address of blanks in puzzle[]*/
char blanks[82];
char schpad[82];
/*lookup tables*/
char colAdds[9][9];
char rowAdds[9][9];
char celAdds[9][9];
char AddCol[81];
char AddRow[81];
char AddCel[81];
char colIndex[9][10];
char rowIndex[9][10];
char celIndex[9][10];
/*posBlanks[n][0] = number of possiblities*/
/*posBlanks[n][1] = first possiblity*/
/* n = blanks index number */
char posBlanks[81][10];
/*slvOrder[n] = index of blanks[]*/
/* used to optimize order in witch they blanks are tested*/
char slvOrder[81];
/*swX is the X origin for the side window*/
/*swY is the Y origin for the side window*/
/*swI is used by the function to point to the current line*/
int swX;
int swY;
int swI;
/*string buffer for creating strings to seed to the side window or status bar*/
char newS[100];
int sbX;
int sbY;
void main()
{
    swX = 4;
    swY = 30;
    swI = 0;
    sbX = 18;
    sbY = 1;
    splash();
    initLookup();
    /*Clear line about creating lookup tables*/
    setCursorXY(3,1);
    printf("\033[K");
    /*Show instructions in SideWindow*/
    setCursorXY(4,1);
    swPrint("Press 1-9 or 0 for Blanks");
    swPrint("Navigate with W A S D");
    swPrint("Press Enter to Solve");
    /*start puzle input*/ 
    inputPuz();
    /*Clear sidwindow of input instuctions*/
    swClear();
    
    
    swPrint("Simplifying the puzlle solutions");
    simplify();
    /*Detect early solve*/
    if(blanks[0]==0)
    {
        setCursorXY(4,1);
        drawPuzzl8(0);
        setCursorXY(21,1);
        exit(0);
    }
    /*Sort for blanks with fiewest possiblities to reduce branch complexity*/
    swPrint("Optimizing solve order");
    optOrder();
    /*Start Bruit force solving*/
    swPrint("Begining bruit force solver");
    if (solve() == 1)
    {
        setCursorXY(4,1);
        drawPuzzle(0);
        setCursorXY(21,1);
    }
    else
    {
        printf("No soultion\n");
    }
}
void swPrint(st)
char *st;
{
    setCursorXY(swX+swI,swY);
    printf("%s",st);
    swI++;
}
/* void sbPrint(char *st, int line)*/
/* prints string st in line line of status bar*/
void sbPrint(st, line)
char *st;
int line;
{
    setCursorXY(sbX+line,sbY);
    printf("\033[K");
    printf("%s",st);
}
void swClear()
{
int i;
    for(i=0;i<=swI;i++)
    {
        setCursorXY(swX+i,swY);
        printf("\033[K");
    }
    swI=0;
}
/* void drawGrid()*/
/* draws the grid of the puzzle*/  
void drawGrid()
{
    drawSeg();
    drawSeg();
    drawSeg();
    printf("*************************\n");
}
void drawSeg()
{
    printf("*************************\n");
    printf("*       *       *       *\n");
    printf("*       *       *       *\n");
    printf("*       *       *       *\n");
}


/* void offsetCursor(int ox, int oy, int cidx)*/
/* offsets the cursor to the X,Y position of the puzzle*/
/* ox and oy are the X and Y position of the puzzle*/
/* cidx is the index of the puzzle array*/
void offsetCursor(ox, oy, cidx)
int ox,oy,cidx;
{
    setCursorXY(ox+1+AddRow[cidx]+(AddRow[cidx]/3),oy+2+((AddCol[cidx]/3)*2)+(AddCol[cidx]*2));
}
/* void setCursorXY(int x, int y)*/
/* sets the cursor to X,Y position*/
/* X and Y are 1 based*/
void setCursorXY(x,y)
int x;
int y;
{
    printf("\033[%d;%dH",x,y);
}
/* void screenClear()*/
/* clears the screen*/
void screenClear()
{
    printf("\033[2J");
}
/* void splash()*/
/* prints the splash screen*/
void splash()
{
    screenClear();
    setCursorXY(1,24);
    printf("Sudoku Solver by Nathanael Nunes\n");
    setCursorXY(2,28);
    printf("V1.2 build April 08 2025\n");

}
/* void initLookup()*/
/* generates the lookup tables for the puzzle*/
void initLookup()
{
    char i, idx, startRow, startCol, cellIndex;
    printf("Generateing Lookup tables.\n");
    for (i = 0; i < 81; i++)
    {
        AddCol[i] = i % 9;
        AddRow[i] = i / 9;
        AddCel[i] = AddCol[i] / 3 + (AddRow[i] / 3) * 3;
    }

    for (i = 0; i < 9; i++)
    {
        for (idx = 0; idx < 9; idx++)
        {
            colAdds[i][idx] = i + idx * 9;
            rowAdds[i][idx] = i * 9 + idx;
            startRow = (i / 3) * 3;
            startCol = (i % 3) * 3;
            cellIndex = startRow * 9 + startCol;
            celAdds[i][idx] = cellIndex + (idx / 3) * 9 + (idx % 3);
        }
    }
}

/* int testAdr(int ad, intv)*/
/* test if value v can be placed in address ad*/
/* returns 1 if v can be placed*/
char testAdr(ad, v)
char ad;
char v;
{
    int i;
    for (i = 0; i < 9; i++)
    {
        if (ad != colAdds[AddCol[ad]][i] && puzzle[colAdds[AddCol[ad]][i]] == v)
            return 0;
        if (ad != rowAdds[AddRow[ad]][i] && puzzle[rowAdds[AddRow[ad]][i]] == v)
            return 0;
        if (ad != celAdds[AddCel[ad]][i] && puzzle[celAdds[AddCel[ad]][i]] == v)
            return 0;
    }
    return 1;
}
/* void inputPuz()*/
/* inputs the puzzle from the keyboard*/
void inputPuz()
{
    char c;
    char i;

/* Draw the puzzle gird*/
    setCursorXY(4,1);
    drawGrid();
    offsetCursor(4,1,0);

    for (i = 0; i < 81;)
    {
gcst:
        /* place cursor over current address*/
        offsetCursor(4,1,i);
        c = getKeypress();
        /* check if the key is a number*/
        if(isdigit(c)>0)
        {
            if(testAdr(i,c-48)>0 || c == '0')
            {
                puzzle[i++]=c-48;
                if(c=='0')
                    printf(" ",c);
                else
                    printf("%c",c);
            }
        }

        else
        {
            switch(c)
            {
                /*Up*/
            case 'w':
            case 'W':
                if(i>8)
                    i -= 9;
                break;
                /*Left*/
            case 'a':
            case 'A':
                if (i>0)
                    i--;
                break;
                /*Down*/
            case 's':
            case 'S':
                if(i<72)
                    i += 9;
                break;
                /*Right*/
            case 'd':
            case 'D':
                if(i <80)
                    i++;
                break;
                /*Enter*/
            case 0x0D:
                return;
                break;
            }
        }    
    }
}
/* void drawPuzzle(int noc)*/
/* draws the puzzle with noc numbers*/
/* if noc is 0 it draws the whole puzzle*/
void drawPuzzle(noc)
int noc;
{
    int i;
    if(noc < 1 || noc > 81)
    {
        noc = 81;
    }
    printf("*************************\n*");
    for (i = 0; i < noc; i++)
    {
        if(puzzle[i]>0)
        {
            printf(" %d",puzzle[i]);
        }
        else
        {
            printf("  ");
        }
        if(AddCol[i+1] % 3 == 0)
        {
            printf(" *");
        }
        if(AddCol[i+1] == 0)
        {
            printf("\n*");
            if(AddRow[i+1] % 3 == 0)
            {
                printf("************************\n");
                if(i!=80)
                {
                    printf("*");
                }
            }
        }
    }
}
/* char getKeypress()*/
/* gets a keypress from the keyboard*/
/* returns the keypress*/
char getKeypress()
{
    int bc, be, c;
    bc = 6;
    be = 255;
    for (;;)
    {
        /*call cp/m bdos*/
        c = bdos(bc, be);
        switch(c)
        {
            /* control break */
        case 3:
            exit(0);
            /*no key pressed*/
        case 0:
            break;
            /* any key press*/
        default:
            return c;
        }

    }
    return c;
}
/* void findBlanks()*/
/* finds the blanks in the puzzle*/
/* stores the address of the blanks in blanks[]*/
/* blanks[0] = number of blanks*/
/* blanks[n] = address of blanks in puzzle[]*/
void findBlanks()
{
    char i;
    blanks[0] = 0;
    for (i = 0; i < 81; i++)
    {
        if (puzzle[i] == 0)
        {
            blanks[blanks[0] + 1] = i;
            blanks[0]++;
        }
    }
}
/* void findIndex*/
/* finds the index of the blanks in the puzzle*/
/* stores the index of the blanks in colIndex[], rowIndex[], celIndex[]*/
/* colIndex[n][0] = number of blanks in column n*/
/* rowIndex[n][0] = number of blanks in row n*/
/* celIndex[n][0] = number of blanks in cell n*/
/* colIndex[n][i] = index of the i-th blank in column n*/
/* rowIndex[n][i] = index of the i-th blank in row n*/
/* celIndex[n][i] = index of the i-th blank in cell n*/
void findIndex()
{
    int i;
    char iaddr;
    /* clear the index arrays*/
    for(i=0;i<9;i++)
    {
        colIndex[i][0] = 0;
        rowIndex[i][0] = 0;
        celIndex[i][0] = 0;
    }
    /* fill the index arrays*/

    for(i=1;i<=blanks[0];i++)
    {
        iaddr = blanks[i];
        colIndex[AddCol[iaddr]][++colIndex[AddCol[iaddr]][0]] = i;
        rowIndex[AddRow[iaddr]][++rowIndex[AddRow[iaddr]][0]] = i;
        celIndex[AddCel[iaddr]][++celIndex[AddCel[iaddr]][0]] = i;
    }
}
void findPosible()
{
    char i,vals;
  for (i = 0; i < blanks[0]; i++)
    {
        posBlanks[i][0] = 0;
        for (vals = 1; vals < 10; vals++)
        {
            if (testAdr(blanks[i + 1], vals) > 0)
            {
                posBlanks[i][posBlanks[i][0] + 1] = vals;
                posBlanks[i][0]++;
            }
        }
    }
    findIndex();
}
void trimPosible()
{
    char i,k,vals;
    for (i = 0; i < blanks[0]; i++)
    {
        for (k = 1; k < 10; k++)
        {
            vals = posBlanks[i][k];
            if (vals != 0)
            {
                if (testAdr(blanks[i + 1], vals) == 0)
                {
                    posBlanks[i][k] = 0;
                    posBlanks[i][0]--;
                }
            }
        }
    }
}

/* void simplify()*/
/* simplifies the puzzle by removing possiblities*/
/* if a blank has only one possiblity it is filled in*/
/* it will repeat until no more simplification is possible*/
/* it will also remove possiblities from other blanks*/
void simplify()
{
    char i, vals;
    findBlanks();
    findPosible();
stlp:
  /*findPosible(); */
    vals = 0;
    vals += nakedSingle();
    vals += hiddenSingle();
    vals += nakedPair();
    
    showComp();
    setCursorXY(4,1);
    drawPuzzle(0);
    if (vals !=0)
    {
        /*findBlanks();*/
        goto stlp;
    }
}
void fillBlank(index, value)
char index,value;
{
    char addr,i,j;
    /* fill in the blank with the value*/
    addr = blanks[index + 1];
    puzzle[addr] = value;
    /* remove the blank from the list*/
    for(i=index;i<blanks[0];i++)
    {
        blanks[i+1] = blanks[i+2];
        /* move the blanks down*/
        for (j=0;j<10;j++)
        {
            posBlanks[i][j] = posBlanks[i+1][j];
        }
    }
    /* remove the blank from the index*/

    blanks[0]--;
    findIndex();
}
void adrFillBlank(adr, value)
char adr, value;
{
    char i;
    for (i = 0; i < blanks[0]; i++)
    {
        if (blanks[i + 1] == adr)
        {
            fillBlank(i, value);
            break;
        }
    }
}
/* void nakedSingle()*/
/* fills in the blanks with only one possiblity*/
char nakedSingle()
{
    char i,hit;
    hit = 0;
    for (i = 0; i < blanks[0]; i++)
    {
        if (posBlanks[i][0] == 1)
        {
            /*puzzle[blanks[i + 1]] = posBlanks[i][1];*/
            sprintf(newS, "Naked single Row %d Col %d Value %d\n", AddRow[blanks[i]] + 1, AddCol[blanks[i]] , posBlanks[i][1]);
            sbPrint(newS,1);
            fillBlank(i,posBlanks[i][1]);
            i=0;
            hit = 1;
        }
    }
    /*
    if(hit!=0)
    {
        findBlanks();
        findPosible();
    }
    */
    return hit;
}
char hiddenSingle()
{
    char hit, row, col, cell, sell,val;
    char poscnt[9];
    char pos[9];
    hit = 0;
    for (row = 0; row < 9; row++)
    {
        setmem(poscnt,9,0);
        for(cell = 0; cell < 9;cell++)
        {
            if(puzzle[rowAdds[row][cell]]==0)
            {
                for(val=1;val<10;val++)
                {
                    
                    if(testAdr(rowAdds[row][cell],val)>0)
                    {
                        poscnt[val-1]++;
                        pos[val-1] = rowAdds[row][cell];
                    }
                }
            }
        }
        for(cell=0;cell<9;cell++)
        {
            if(poscnt[cell]==1)
            {
                /*puzzle[pos[cell]] = cell+1;*/
                sprintf(newS,"Hidden single Row %d Col %d Value %d\n",row,cell,poscnt[cell]);
                sbPrint(newS,2);
                adrFillBlank(pos[cell],cell+1);
                hit = 1;
            }
        }
    }
    for(col=0;col<9;col++)
    {
        setmem(poscnt,9,0);
        for(cell=0;cell<9;cell++)
        {
            if(puzzle[colAdds[col][cell]]==0)
            {
                for(val=1;val<10;val++)
                {
                    if(testAdr(colAdds[col][cell],val)>0)
                    {
                        poscnt[val-1]++;
                        pos[val-1] = colAdds[col][cell];
                    }
                }
            }
        }
        for(cell=0;cell<9;cell++)
        {
            if(poscnt[cell]==1)
            {
                /*puzzle[pos[cell]] = cell+1;*/
                adrFillBlank(pos[cell],cell+1);
                hit = 1;
            }
        }
    }
    for(sell=0;sell<9;sell++)
    {
        setmem(poscnt,9,0);
        for(cell=0;cell<9;cell++)
        {
            if(puzzle[celAdds[sell][cell]]==0)
            {
                for(val=1;val<10;val++)
                {
                    if(testAdr(celAdds[sell][cell],val)>0)
                    {
                        poscnt[val-1]++;
                        pos[val-1] = celAdds[sell][cell];
                    }
                }
            }
        }
        for(cell=0;cell<9;cell++)
        {
            if(poscnt[cell]==1)
            {
                /*puzzle[pos[cell]] = cell+1;*/
                adrFillBlank(pos[cell],cell+1);
                hit = 1;
            }
        }
    }
    /*if(hit!=0)
    {
        findBlanks();
        findPosible();
    }*/
    return hit;
}
/* char posPurge()*/
/* purges the possiblity val from the blank pidex*/
/* it will remove the possiblity from the blank*/
/* it will return 1 if the possiblity was removed*/
/* it will return 0 if the possiblity was not removed*/
/* it will also remove the blank from the index*/
char posPurge(pidex, val)
char pidex,val;
{
    char i,j;
    /* check if the blank is empty*/
    if(posBlanks[pidex][0]==0)
    {
        return 0;
    }
    /* check if the possiblity is in the blank*/
    for(i=1;i<=posBlanks[pidex][0];i++)
    {
        /* check if the possiblity is in the blank*/
        if(posBlanks[pidex][i]==val)
        {
            /* remove the possiblity from the blank*/
            for(j=i;j<posBlanks[pidex][0];j++)
            {
                /* move the possiblities down*/
                posBlanks[pidex][j] = posBlanks[pidex][j+1];
            }
         
            /* decrease possiblities count*/
            posBlanks[pidex][0]--;
            return 1;
        }
    }
    return 0;
}
/* char rowPurge(char row, char exempt1, char exempt2, char val1, char val2)*/
/* purges the possiblity val from the row*/
/* it will remove the possiblity from the row*/
/* it will return 1 if the possiblity was removed*/
/* it will return 0 if the possiblity was not removed*/
char rowPurge(row, exempt1, exempt2, val1, val2)
char row, exempt1, exempt2, val1, val2;
{
    char i,j,hit,ridx;
    /* check if the row is empty*/
    if(rowIndex[row][0]==0)
    {
        return 0;
    }
    hit = 0;
    /* check if the possiblity is in the row*/
    for(i=0;i<rowIndex[row][0];i++)
    {
        ridx = rowIndex[row][i];
        /* check if the possiblity is in the row*/
        if(ridx!=exempt1 && ridx!=exempt2)
        {
            /* remove the possiblity from the row*/
            hit += posPurge(ridx,val1);
            hit += posPurge(ridx,val2);
        }
    }
    return hit;
}
char colPurge(col, exempt1, exempt2, val1, val2)
char col, exempt1, exempt2, val1, val2;
{
    char i,j,hit,cidx;
    hit = 0;
    for(i=0;i<colIndex[col][0];i++)
    {
        cidx = colIndex[col][i];
        if(cidx!=exempt1 && cidx!=exempt2)
        {
           hit += posPurge(cidx,val1);
           hit +=posPurge(cidx,val2);
        }
    }
    return hit;
}
char celPurge(cel, exempt1, exempt2, val1, val2)
char cel, exempt1, exempt2, val1, val2;
{
    char i,j,hit,cidx;
    hit = 0;
    for(i=0;i<celIndex[cel][0];i++)
    {
        cidx = celIndex[cel][i];

        if(cidx!=exempt1 && cidx!=exempt2)
        {
           hit += posPurge(cidx,val1);
           hit += posPurge(cidx,val2);
        }
    }
    return hit;
}

char nakedPair()
{
    char i,j,k;
    char pair[2];
    char hit;
    char pindex[2];
    char findCount;
    findCount = 0;
    hit = 0;
    /*Start with rows*/
    for(i=0;i<9;i++)
    {
        for(j=0;j<rowIndex[i][0];j++)
        {
            if(posBlanks[rowIndex[i][j]][0]==2)
            {
                pair[0] = posBlanks[rowIndex[i][j]][1];
                pair[1] = posBlanks[rowIndex[i][j]][2];
                pindex[0] = rowIndex[i][j];
                for(k=j+1;k<rowIndex[i][0];k++)
                {
                    
                        if(posBlanks[rowIndex[i][k]][0]==2 && posBlanks[rowIndex[i][k]][1]==pair[0] && posBlanks[rowIndex[i][k]][2]==pair[1])
                        {
                          pindex[1] = rowIndex[i][k];
                          hit += rowPurge(i,pindex[0],pindex[1],pair[0],pair[1]);
                          if(hit>0)
                          {
                          sprintf(newS,"%d Row %d Pair %d,%d\n",++findCount,i+1,pair[0],pair[1]);
                            sbPrint(newS,3);
                          }
                          break;
                        }
                        
                       
                        
                    
                }
            }
        }
    }
    /*Start with columns*/
    findCount = 0;
    for(i=0;i<9;i++)
    {
        for(j=0;j<colIndex[i][0];j++)
        {
            if(posBlanks[colIndex[i][j]][0]==2)
            {
                pair[0] = posBlanks[colIndex[i][j]][1];
                pair[1] = posBlanks[colIndex[i][j]][2];
                pindex[0] = colIndex[i][j];
                for(k=j+1;k<colIndex[i][0];k++)
                {
                    
                        if(posBlanks[colIndex[i][k]][0]==2 && posBlanks[colIndex[i][k]][1]==pair[0] && posBlanks[colIndex[i][k]][2]==pair[1])
                        {
                          pindex[1] = colIndex[i][k];
                          hit += colPurge(i,pindex[0],pindex[1],pair[0],pair[1]);
                            if(hit>0)
                            {                            
                          sprintf(newS,"%d Col %d Pair %d,%d\n",++findCount,i,pair[0],pair[1]);
                            sbPrint(newS,3);
                            }
                          break;
                        }
                        
                       
                        
                    
                }
            }
        }
    }
    /*Start with cells*/
    findCount = 0;
    for(i=0;i<9;i++)
    {
        for(j=0;j<celIndex[i][0];j++)
        {
            if(posBlanks[celIndex[i][j]][0]==2)
            {
                pair[0] = posBlanks[celIndex[i][j]][1];
                pair[1] = posBlanks[celIndex[i][j]][2];
                pindex[0] = celIndex[i][j];
                for(k=j+1;k<celIndex[i][0];k++)
                {
                    
                        if(posBlanks[celIndex[i][k]][0]==2 && posBlanks[celIndex[i][k]][1]==pair[0] && posBlanks[celIndex[i][k]][2]==pair[1])
                        {
                          pindex[1] = celIndex[i][k];
                          hit += celPurge(i,pindex[0],pindex[1],pair[0],pair[1]);
                          sprintf(newS,"%d Cell %d Pair %d,%d\n",findCount,i,pair[0],pair[1]);
                            sbPrint(newS,3);
                          break;
                        }
                        
                       
                        
                    
                }
            }
        }
    }
    return hit;
}
/* void showComp()*/
/* shows the complexity of the puzzle*/
/* the complexity is the number of possiblities for each blank*/

void showComp()
{
    int i, isum;
    double cpx;
    cpx = 1;
    for (i = 0; i < blanks[0]; i++)
    {
        cpx *= posBlanks[i][0];
    }
    sprintf(newS,"Unknowns: %d Complexity: %.0f\n", blanks[0], cpx);
    sbPrint(newS,0);
}
/* void optOrder()*/
/* optimizes the order of the blanks to be tested*/
/* it sorts the blanks by the number of possiblities*/
/* the blanks with the fewest possiblities are tested first*/
/* this reduces the number of branches in the search tree*/
void optOrder()
{
    int idx, val, opnt;
    opnt = 0;
    for (val = 1; val < 10; val++)
    {
        for (idx = 0; idx < blanks[0]; idx++)
        {
            if (posBlanks[idx][0] == val)
            {
                slvOrder[opnt++] = idx;
            }
        }
    }
}
/* int solve()*/
/* solves the puzzle using a brute force algorithm*/
/* it uses a depth first search to find the solution*/
/* it will return 1 if a solution is found*/
/* it will return 0 if no solution is found*/
/* it will also print the number of solutions tested*/
int solve()
{
    /* cadd is the address of the current blank*/
    /* sidx is the index of the current blank being tested*/
    /* endidx is the total number of blanks to test*/
    /* idxPnt is the index of the current blank in the slvOrder*/
    char sidx, cadd, endidx,idxPnt;
    /* valpnt is the index of the current possiblity being tested*/
    char valpnt[81];
    char valptEnd;
    int dspcnt;
    long sltst;
    /* tarval is the temprary address value*/
    char tarval;
    /* dspcnt counts the loop cycles for status messages*/
    dspcnt = 0;
    /* sltst counts the number of solutions tested*/
    sltst = 0;
    /* sidx is the index of the current blank being tested*/
    sidx = 0;
    /* endidx is the total number of blanks to test*/
    endidx = blanks[0];
    /* start of main loop  it inits stuff*/
smlp:
    idxPnt = slvOrder[sidx];
    cadd = blanks[idxPnt + 1];
    valpnt[idxPnt] = 0;
    valptEnd = posBlanks[idxPnt][0];
    /* main loop*/
mlp:
    dspcnt++;
    /* display current solution test ever 10000 cycles*/
    if(dspcnt==10000)
    {
        sprintf(newS,"%lu solutions tested\n",sltst);
        sbPrint(newS,1);
        dspcnt = 0;
    }
    /* check if the current value is valid*/
    /* if it is valid set the value in the puzzle*/
    /* if it is not valid go to the next value*/

    tarval = posBlanks[idxPnt][valpnt[idxPnt]+1];
    if (testAdr(cadd, tarval) > 0)
    {
        puzzle[cadd] = tarval;
        goto ntlp;
    }
    /* test next possible value in current blank*/
    idxinc:
    valpnt[idxPnt]++;
    if (valpnt[idxPnt] == valptEnd)
    {
        goto prlp;
    }
    goto mlp;
    /* move to next loop*/
ntlp:
/* point to the next blank*/
    sidx++;
    /* check if we have reached the end of the blanks*/
    if (sidx == endidx)
    {
        /*conrdadulations we did it everyone*/
        goto slvd;
    }
    /*start proccessing the next blank*/
    goto smlp;
    /* move to previous loop*/
prlp:
    sltst++;
    /* if we faill back to index 0 and there are no more option puzzle cannot be solved*/
    if (sidx == 0)
    {
        goto fld;
    }
    /* store 0 in currently unsolved puzzle address*/
    puzzle[cadd] = 0;
    /* set the test value pointer for this blank back to 0*/
    valpnt[slvOrder[sidx]] = 0;
    /* set the index pointer to the previous blank*/
    sidx--;
    /* set the current blank address to the previous blank*/
    cadd = blanks[slvOrder[sidx] + 1];
    /* reset the end of blanks for previous blank*/
    valptEnd = posBlanks[slvOrder[sidx]][0];
    /* set the current value pointer to the previous blank */
    idxPnt = slvOrder[sidx];
    /* resume prevous loop where it left off*/
    goto idxinc;
slvd:
    sltst++;
    sprintf(newS,"Solution# %lu\n",sltst);
    sbPrint(newS,2);
    return 1;
fld:
    return 0;
}
