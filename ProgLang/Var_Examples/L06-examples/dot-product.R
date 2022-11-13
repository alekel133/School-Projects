## Define Dot Product of vectors
dot <- function( x, y ) {
  return( sum(x*y) )
}

## Dot product of scalar variables
a <- 2
b <- 3

dot(a,b)

## Dot product of 2D vectors
c <- c( 2, 2 )
d <- c( 3, 3 )

dot(c,d)

## Dot product of mixed-type 2D vectors
e <- c( 1,   2,   3 )
f <- c( 1.2, 2.2, 3.2 )

dot(e,f)