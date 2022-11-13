PROGRAM NOTEX;
{ Testing logical calculations. }
VAR
    A: INTEGER;
BEGIN
    { Modify A to test other truth values. }
    A := 0;
    IF NOT A THEN
    	WRITE('TRUE')
    ELSE
        A := NOT (3<4)
    { Indeed, what is truth? }
END
