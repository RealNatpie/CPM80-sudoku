#include "stdio.h" 
#include "ctype.h"
#include "math.h"
void splash();
void initLookup();
void inputPuz();
void drawPuzzle();
void findBlanks();
void simplify();
void fillones();
void showComp();
void optOrder();
void setCursorXY();
void screenClear();
void drawGrid();
void drawSeg();
void offsetCursor();
void swPrint();
void swClear();
int solve();
char getKeypress();
void sbPrint();
int puzzle[81];
int blanks[82];
int colAdds[9][9];
int rowAdds[9][9];
int celAdds[9][9];
int AddCol[81];
int AddRow[81];
int AddCel[81];
int posBlanks[81][10];
int slvOrder[81];
int swX;
int swY;
int swI;
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
    setCursorXY(3,1);
    printf("\033[K");
    setCursorXY(4,1);
    swPrint("Press 1-9 or 0 for Blanks");
    swPrint("Navigate with W A S D");
    swPrint("Press Enter to Solve"); 
    inputPuz();
    findBlanks();
    swClear();
    swPrint("Simplifying the puzlle solutions");
    simplify();
    if(blanks[0]==0)
    {
        setCursorXY(4,1);
        drawPuzzl8(0);
        setCursorXY(21,1);
        exit(0);
    }
    swPrint("Optimizing solve order");
    optOrder();
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
void offsetCursor(ox, oy, cidx)
int ox,oy,cidx;
{
    setCursorXY(ox+1+AddRow[cidx]+(AddRow[cidx]/3),oy+2+((AddCol[cidx]/3)*2)+(AddCol[cidx]*2));
}
void setCursorXY(x,y)
int x;
int y;
{
    printf("\033[%d;%dH",x,y);
}
void screenClear()
{
    printf("\033[2J");
}
void splash()
{
    screenClear();
    setCursorXY(1,24);
    printf("Sudoku Solver by Nathanael Nunes\n");
    setCursorXY(2,28);
    printf("V1.1 build April 04 2025\n");

}
void initLookup()
{
    int i, idx, startRow, startCol, cellIndex;
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
int testAdr(ad, v)
int ad;
int v;
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
void inputPuz()
{
    char c;
    int i;


    setCursorXY(4,1);
    drawGrid();
    offsetCursor(4,1,0);

    for (i = 0; i < 81;)
    {
gcst:
        offsetCursor(4,1,i);
        c = getKeypress();
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
            case 'w':
            case 'W':
                if(i>8)
                    i -= 9;
                break;
            case 'a':
            case 'A':
                if (i>0)
                    i--;
                break;
            case 's':
            case 'S':
                if(i<72)
                    i += 9;
                break;
            case 'd':
            case 'D':
                if(i <80)
                    i++;
                break;
            case 0x0D:
                return;
                break;
            }
        }    
    }
}

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
char getKeypress()
{
    int bc, be, c;
    bc = 6;
    be = 255;
    for (;;)
    {
        c = bdos(bc, be);
        switch(c)
        {
        case 3:
            exit(0);
        case 0:
            break;
        default:
            return c;
        }

    }
    return c;
}
void findBlanks()
{
    int i;
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
void simplify()
{
    int i, vals;
stlp:
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
    vals = 0;
    for (i = 0; i < blanks[0]; i++)
    {
        if (posBlanks[i][0] == 1)
        {
            vals = 1;
            break;
        }
    }
    showComp();
    setCursorXY(4,1);
    drawPuzzle(0);
    if (vals == 1)
    {
        fillones();
        findBlanks();
        goto stlp;
    }
}
void fillones()
{
    int i;
    for (i = 0; i < blanks[0]; i++)
    {
        if (posBlanks[i][0] == 1)
        {
            puzzle[blanks[i + 1]] = posBlanks[i][1];
        }
    }
}
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
int solve()
{
    int sidx, cadd, endidx,idxPnt;
    int valpnt[81];
    int valptEnd;
    int dspcnt;
    long sltst;
    int tarval;
    dspcnt = 0;
    sltst = 0;;
    sidx = 0;
    endidx = blanks[0];
smlp:
    idxPnt = slvOrder[sidx];
    cadd = blanks[idxPnt + 1];
    valpnt[idxPnt] = 0;
    valptEnd = posBlanks[idxPnt][0];
mlp:
    dspcnt++;
    if(dspcnt==10000)
    {
        sprintf(newS,"%d solutions tested\n",sltst);
        sbPrint(newS,1);
        dspcnt = 0;
    }
    tarval = posBlanks[idxPnt][valpnt[idxPnt]+1];
    if (testAdr(cadd, tarval) > 0)
    {
        puzzle[cadd] = tarval;
        goto ntlp;
    }
    idxinc:
    valpnt[idxPnt]++;
    if (valpnt[idxPnt] == valptEnd)
    {
        goto prlp;
    }
    goto mlp;
ntlp:
    sidx++;
    if (sidx == endidx)
    {
        goto slvd;
    }
    goto smlp;
prlp:
    sltst++;
    if (sidx == 0)
    {
        goto fld;
    }
    puzzle[cadd] = 0;
    valpnt[slvOrder[sidx]] = 0;
    sidx--;
    cadd = blanks[slvOrder[sidx] + 1];
    valptEnd = posBlanks[slvOrder[sidx]][0];
    idxPnt = slvOrder[sidx];
    goto idxinc;
slvd:
    sltst++;
    sprintf(newS,"Solution# %d\n",sltst);
    sbPrint(newS,2);
    return 1;
fld:
    return 0;
}
