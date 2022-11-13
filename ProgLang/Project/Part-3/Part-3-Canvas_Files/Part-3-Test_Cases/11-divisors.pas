PROGRAM DIVISORS;
{ Tell us about the divisors of 100! }
VAR
    I: INTEGER;
    N: INTEGER;
    MODULA: INTEGER;

BEGIN

    I := 1;
    WRITE('The divisors of 100 are:');

    WHILE (I<101)
    BEGIN
        MODULA := 100;
        WHILE (MODULA > I)
           MODULA := MODULA - I;
        IF -MODULA = -I THEN
            WRITE(I);
        I := I + 1
    END;  
    
    WRITE('--- Sir, your divisors are ready ---')
END
