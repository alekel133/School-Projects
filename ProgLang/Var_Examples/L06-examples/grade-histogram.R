## Generate a grade histogram

T=FALSE
#TRUE=FALSE
True=FALSE
true=FALSE
## Clear current variables
rm(list=ls(all=T))

print(TRUE)
#print(True)
print(T)
print(FALSE)
#print(False)
print(F)

## Read grade table and print stem-and-leaf histogram
Tbl <- read.table( "grades.txt", header=TRUE )
stem( Tbl$Midterm, scale=1 )

#T=FALSE
T=TRUE

U <- read.table( "grades.txt", header=T )
stem( U$Midterm, scale=1 )


