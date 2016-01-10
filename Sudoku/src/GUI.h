/******************************************************************************
 * SUDOKU ./reader.c                                                          *
 * Steuerung für die Händische Manipulation von Sudokus.
 * @author Felix Schütze, dhbw@felix-schuetze.de
 ******************************************************************************/

void gotoxy(int x,int y)
{
  printf("%c[%d;%df",0x1B,y,x);
}

int loop()
{
  int x = 0, y = 0;
  char c = 0;
  system("clear");


  while(1)
  {
    x = rand()%50;
    y = rand()%25;
    c = rand()%'z' + ' ';
    gotoxy(x,y);
    printf("%c", c);
    fflush(stdout);
    sleep(1);
  }
}
