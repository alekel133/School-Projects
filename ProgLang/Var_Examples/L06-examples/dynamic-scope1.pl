# dynamic scoping example -- perl
# perl dynamic-scope1.pl
$x = 5;
$y = 3;

sub sub1 {
  $x = 7;
  print "sub1 x: ", $x, "\n";

  sub sub2 {
    $x = 9;
    print "sub2 x: ", $x, "\n";
  }
  sub2();
  $y = 10;
}
sub1();
print "base x: ", $x, "\n";
print "base y: ", $y, "\n";
