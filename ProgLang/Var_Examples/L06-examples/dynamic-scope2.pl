# dynamic scoping example -- perl
# perl dynamic-scope2.pl
$x = 5;
$y = 3;
print "base x: ", $x, "\n";
print "base y: ", $y, "\n";


sub sub1 {
  print "\tsub1 called\n";
  print "\tsub1 x: ", $x, "\n";
  print "\tsub1 y: ", $y, "\n";
  print "\tsub1 set x\n";
  $x = 7;
  print "\tsub1 x: ", $x, "\n";
  print "\tsub1 y: ", $y, "\n";

  sub sub2 {
    print "\t\tsub2 called\n";
    print "\t\tsub2 x: ", $x, "\n";
    print "\t\tsub2 y: ", $y, "\n";
    print "\t\tsub2 set x\n";
    $x = 9;
    print "\t\tsub2 x: ", $x, "\n";
    print "\t\tsub2 y: ", $y, "\n";
  }
  sub2();
  print "\tsub1 x: ", $x, "\n";
  print "\tsub1 y: ", $y, "\n";
  print "\tsub1 set y\n";
  $y = 10;
  print "\tsub1 x: ", $x, "\n";
  print "\tsub1 y: ", $y, "\n";
  print "\tsub1 returned\n";
}

sub1();
print "base x: ", $x, "\n";
print "base y: ", $y, "\n";
