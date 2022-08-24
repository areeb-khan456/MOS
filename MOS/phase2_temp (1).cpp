#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

struct pcb
{
  int ID, TTL, TLL;
} p;

char M[300][4], IR[5], R[4], c, ch, temp[5];
int b, IC, SI, e, TI, PI, flag[30], PTE, PTEc, TTC, TLC, RA, VA, ef;

FILE *fp, *ft;

void init();
void LOAD();
int ALLOCATE();
void STARTEXECUTION();
void EXECUTEUSERPROGRAM();
int ADDRESSMAP(int);
void MOS();
void READ();
void WRITE();
void TERMINATE(int);
void SIMULATION();
void disp();

int main()
{
  fp = fopen("input.txt", "r");
  ft = fopen("output.txt", "w");

  if (!fp)
  {
    perror("File Not found\n");
  }
  else
  {
    printf("Input File Found");

    LOAD();
    disp();
  }
  printf("Completed");

  return 0;
}

void init()
{
  for (int i = 0; i < 300; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      M[i][j] = '\0';
    }
  }

  for (int i = 0; i < 30; i++)
  {
    flag[i] = 0;
  }
  b = 0;
  e = b + 10;
  SI = 0;
  TI = 0;
  ef = 0;
  PI = 0;
  TTC = 0;
  TLC = 0;
}

void disp()
{
  int i, j, k;
  printf("\nMain Mem");
  for (i = 0; i < 30; i++)
  {
    if (flag[i] == 1)
    {
      for (j = i * 10; j < (i * 10) + 10; j++)
      {
        //printf("\n%d\t",j);
        printf("\n");
        for (k = 0; k < 4; k++)
        {
          if (M[j][k] != '\0')
            printf("%c", M[j][k]);
        }
      }
    }
  }
  printf("PTE");
  for (i = PTE; i < PTE + 10; i++)
  {
    printf("\n%d\t", i);
    for (j = 0; j < 4; j++)
    {
      printf("%c", M[i][j]);
    }
  }
}

void LOAD()
{
  int i, j, k, t, r;
  char a[2]; /*  temp[4], */
  ch = fgetc(fp);
  init();
  do
  {
    for (i = 0; i < 5; i++)
      temp[i] = '\0';
    i = 0;
    // Storing first 4 bytes
    while (i < 4 && ch != '\n')
    {
      temp[i] = ch;
      ch = fgetc(fp);
      i++;
    }
    temp[i] = '\0';
    //printf("\ntemp: %s", temp);
    if ((strcmp(temp, "$AMJ")) == 0)
    {
      i = 0;
      while (i < 4)
      {
        temp[i] = ch;
        ch = fgetc(fp);
        i++;
      }
      temp[i] = '\0';
      p.ID = atoi(&temp[0]);
      //printf("ID:%d",p.ID);
      i = 0;
      while (i < 4)
      {
        temp[i] = ch;
        ch = fgetc(fp);
        i++;
      }
      temp[i] = '\0';

      p.TTL = atoi(&temp[0]);
      //printf("\tTTL:%d",p.TTL);
      i = 0;
      while (i < 4)
      {
        temp[i] = ch;
        ch = fgetc(fp);
        i++;
      }
      temp[i] = '\0';

      p.TLL = atoi(&temp[0]);
      //printf("\tTLL:%d",p.TLL);
      // ALLOCATE fucntion generates random number 0-29
      PTE = ALLOCATE() * 10;
      //printf("\tPTE: %d",PTE);
      flag[PTE / 10] = 1;
      PTEc = PTE;
      for (i = PTE; i < PTE + 10; i++)
      {
        // Remove a[0] sustitute '0' insted
        itoa(0, a, 10);
        M[i][0] = a[0];
        M[i][1] = a[0];
        itoa(3, a, 10);
        M[i][2] = a[0];
        itoa(1, a, 10);
        M[i][3] = a[0];
      }
    }
    else if ((strcmp(temp, "$DTA")) == 0)
    {
      STARTEXECUTION();
      if (ef == 1)
      {
        if ((strcmp(temp, "$END")) != 0)
        {
          ch = fgetc(fp);
          while (ch != '$' && ch != EOF)
          {
            ch = fgetc(fp);
          }
          while (ch != '\n' && ch != EOF)
          {
            ch = fgetc(fp);
          }
        }
        else
        {
          while (ch != '\n' && ch != EOF)
          {
            ch = fgetc(fp);
          }
        }
        init();
      }
      //break;
    }
    else if ((strcmp(temp, "$END")) == 0)
    {
      while (ch != '\n' && ch != EOF)
      {
        ch = fgetc(fp);
      }
      if (ef == 1)
      {
        init();
      }
    }
    else if ((strcmp(temp, "$DTA")) != 0)
    {
      b = ALLOCATE();
      while (flag[b] == 1)
      {
        b = ALLOCATE();
      }

      // Changes done written two times
      flag[b] = 1;
      itoa(b / 10, a, 10);
      M[PTEc][2] = a[0];
      itoa(b % 10, a, 10);
      M[PTEc][3] = a[0];
      PTEc++;
      b = b * 10;
      //printf("\tPage: %d",b);
      do
      {
        for (k = 0; k < 4; k++)
        {
          M[b][k] = temp[k];
        }
        //printf("/tcopy=%s",M[b]);
        for (k = 0; k < 4; k++)
        {
          if (ch != '\n' && ch != EOF)
          {
            temp[k] = ch;
            ch = fgetc(fp);
          }
          else
            break;
        }
        //printf("\tget=%s",temp);
        b++;
      } while (ch != '\n' && ch != EOF);
      for (k = 0; k < 4; k++)
      {
        M[b][k] = temp[k];
      }
    }
    if (ch == '\n')
      ch = fgetc(fp);
  } while (ch != EOF);
}

// Difference in allocate function
int ALLOCATE()
{
  int i;
  //randomize();
  i = rand() % 30;
  return (i);
}

void STARTEXECUTION()
{
  IC = 0;
  EXECUTEUSERPROGRAM();
}

void EXECUTEUSERPROGRAM()
{
  int i, j, k;
  //char temp[4];
  for (i = 0; i < 5; i++)
  {
    IR[i] = '\0';
  }
  while (1)
  {
    RA = ADDRESSMAP(IC);

    if (PI != 0)
    {
      break;
    }
    for (i = 0; i < 5; i++)
    {
      IR[i] = '\0';
    }

    for (i = 0; i < 4; i++)
    {
      IR[i] = M[RA][i];
    }

    if (IR[0] == 'L' && IR[1] == 'R')
    {
      VA = atoi(&IR[2]);
      RA = ADDRESSMAP(VA);
      if (PI != 0)
        MOS();
      for (i = 0; i < 4; i++)
        R[i] = '\0';
      for (j = 0; j < 4; j++)
      {
        if (M[RA][j] != '\0' && M[RA][j] != '\n')
          R[j] = M[RA][j];
        else
          break;
      }
    }
    else if (IR[0] == 'S' && IR[1] == 'R')
    {
      VA = atoi(&IR[2]);
      RA = ADDRESSMAP(VA);
      if (PI != 0)
        MOS();
      for (j = 0; j < 4 && R[j] != '\0'; j++)
      {
        M[RA][j] = R[j];
      }
    }
    else if (IR[0] == 'C' && IR[1] == 'R')
    {
      VA = atoi(&IR[2]);
      RA = ADDRESSMAP(VA);
      if (PI != 0)
        MOS();
      for (i = 0; i < 4; i++)
      {
        if (R[i] == M[RA][i])
          c = 't';
        else
        {
          c = 'f';
          break;
        }
      }
    }
    else if (IR[0] == 'B' && IR[1] == 'T')
    {
      if (c == 't')
      {
        VA = atoi(&IR[2]);
        IC = VA;
        IC--;
      }
    }
    else if (IR[0] == 'G' && IR[1] == 'D')
    {
      SI = 1;
      VA = atoi(&IR[2]);
      RA = ADDRESSMAP(VA);
      MOS();
    }
    else if (IR[0] == 'P' && IR[1] == 'D')
    {
      SI = 2;
      VA = atoi(&IR[2]);
      RA = ADDRESSMAP(VA);
      MOS();
    }
    else if (IR[0] == 'H')
    {
      SI = 3;
      MOS();
      init();
      break;
    }
    else
    {
      PI = 1;
      MOS();
    }
    IC++;
    SIMULATION();
    if (ef == 1)
    {
      break;
    }
  }
}

void SIMULATION()
{
  TTC++;
  if (TTC == p.TTL)
  {
    TI = 2;
    MOS();
  }
}

int ADDRESSMAP(int VA)
{
  int t;
  char a[3];
  t = PTE + VA / 10;
  a[0] = M[t][2];
  a[1] = M[t][3];
  a[2] = '\0';
  RA = atoi(&a[0]);
  RA = RA * 10 + (VA % 10);

  // int cmp = atoi(&IR[2]);

  if (IR[2] > 99)
  {
    PI = 2;
  }

  if (RA == 310)
  {
    PI = 3;
  }

  return (RA);
}

void MOS()
{
  int t;
  char a[5];
  if (TI == 0)
  {
    if (SI == 1 && PI == 0)
    {
      READ();
      SI = 0;
    }
    else if (SI == 2 && PI == 0)
    {
      WRITE();
      SI = 0;
    }
    else if (SI == 3 && PI == 0)
    {
      TERMINATE(0);
      SI = 0;
    }
    else if (PI == 1)
    {
      PI = 0;
      TERMINATE(4);
    }
    else if (PI == 2)
    {
      PI = 0;
      TERMINATE(5);
    }
    else if (PI == 3)
    {
      if (SI == 1 || IR[0] == 'S')
      {
        IC--;
        TTC--;
        VA = atoi(&IR[2]);
        RA = ALLOCATE();
        while (flag[RA] == 1)
          RA = ALLOCATE();
        flag[RA] = 1;
        t = VA / 10 + PTE;
        itoa(RA / 10, a, 10);
        M[t][2] = a[0];
        itoa(RA % 10, a, 10);
        M[t][3] = a[0];
        RA = RA * 10;
        PI = 0;
      }
      else
        TERMINATE(6);
    }
  }
  else if (TI == 2)
  {
    if (SI == 1)
    {
      TERMINATE(3);
      SI = 0;
    }
    else if (SI == 2)
    {
      WRITE();
      TERMINATE(3);
      SI = 0;
    }
    else if (SI == 3)
    {
      TERMINATE(0);
      SI = 0;
    }
    else if (PI == 1)
    {
      PI = 0;
      TERMINATE(3);
      TERMINATE(4);
    }
    else if (PI == 2)
    {
      TERMINATE(3);
      TERMINATE(5);
      PI = 0;
    }
    else if (PI == 3)
    {
      TERMINATE(3);
      PI = 0;
    }
  }
}

void READ()
{
  char a[5]; //temp[5];
  int i, t;
  ch = fgetc(fp);
  while (ch != '\n')
  {
    for (i = 0; i < 4; i++)
    {
      temp[i] = '\0';
    }
    for (i = 0; i < 4; i++)
    {
      if (ch != '\n')
      {
        temp[i] = ch;
        ch = fgetc(fp);
      }
    }
    temp[i] = '\0';
    if ((strcmp(temp, "$END")) != 0)
    {
      for (i = 0; i < 4; i++)
      {
        if (temp[i] != '\0')
          M[RA][i] = temp[i];
      }
      RA++;
    }
    else
    {
      TERMINATE(1);
      break;
    }
  }
}

void WRITE()
{
  int i, k;
  VA = atoi(&IR[2]);
  RA = ADDRESSMAP(VA);
  TLC++;
  if (TLC > p.TLL)
  {
    TERMINATE(2);
  }
  else if (M[RA][0] == '\0')
  {
    TERMINATE(6);
  }
  else
  {
    fputc('\n', ft);
    for (i = RA; i < RA + 10; i++)
    {
      for (k = 0; k < 10; k++)
      {
        if (M[i][k] != '\0')
          fputc(M[i][k], ft);
      }
    }
  }
}

void TERMINATE(int n)
{
  fputc('\n', ft);
  printf("\n%d\t%d\t%d", p.ID, TTC, TLC);
  switch (n)
  {
  case 0:
    fprintf(ft, "%d\t%s", p.ID, "No Error");
    fprintf(ft, "\n%d\t%s\t%d\t%d\t%d\t%d\n", IC, IR, p.TTL, p.TLL, TTC, TLC);
    break;
  case 1:
    fprintf(ft, "%d\t%s", p.ID, "Out of Data");
    fprintf(ft, "IC\tTTL\tTLL\tIR\tTTC\tTLC");
    fprintf(ft, "\n%d\t%s\t%d\t%d\t%d\t%d\n", IC, IR, p.TTL, p.TLL, TTC, TLC);
    ef = 1;
    break;
  case 2:
    fprintf(ft, "%d\t%s", p.ID, "Line Limit Exceeds");
    fprintf(ft, "\n%d\t%s\t%d\t%d\t%d\t%d\n", IC, IR, p.TTL, p.TLL, TTC, TLC);
    ef = 1;
    break;
  case 3:
    fprintf(ft, "%d\t%s", p.ID, "TimeLimit Exceeds");
    fprintf(ft, "\n%d\t%s\t%d\t%d\t%d\t%d\n", IC, IR, p.TTL, p.TLL, TTC, TLC);
    ef = 1;
    break;
  case 4:
    fprintf(ft, "%d\t%s", p.ID, "Operation Code Error");
    fprintf(ft, "\n%d\t%s\t%d\t%d\t%d\t%d\n", IC, IR, p.TTL, p.TLL, TTC, TLC);
    ef = 1;
    break;
  case 5:
    fprintf(ft, "%d\t%s", p.ID, "Operand Error");
    fprintf(ft, "\n%d\t%s\t%d\t%d\t%d\t%d\n", IC, IR, p.TTL, p.TLL, TTC, TLC);
    ef = 1;
    break;
  case 6:
    fprintf(ft, "%d\t%s", p.ID, "Invalid Page Fault");
    fprintf(ft, "\n%d\t%s\t%d\t%d\t%d\t%d\n", IC, IR, p.TTL, p.TLL, TTC, TLC);
    ef = 1;
    break;
  }
}



// For GD and SR takes 2 cycles to execute and it calls Executeuserprogram two times