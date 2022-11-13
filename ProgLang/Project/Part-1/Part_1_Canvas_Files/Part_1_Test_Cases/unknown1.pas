PROGRAM AREA;
{ Another TIPS programm for computing some areas. }
VAR
    HEIGHT: REAL;
    WIDTH: REAL;
    PI: REAL;
    RAREA: REAL;
    SAREA: REAL;
    AREA: REAL;
BEGIN
PI := 3.14159;
READ (HEIGHT);
READ (WIDTH);
{ area of rectangle }
RAREA := HEIGHT * WIDTH;
{ area of something else }
SAREA := PI * WIDTH * WIDTH / 8.0;
AREA := RAREA - SAREA;
WRITE (' INPUT VALUES: ');
WRITE (HEIGHT);
WRITE (WIDTH);
WRITE (' AREA = ');
WRITE (AREA)
END