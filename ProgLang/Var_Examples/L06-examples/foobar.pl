# static vs heap memory example -- perl
# perl foobar.pl

my $str = "Foo";
print $str, "\n";
$str .= "bar";
print $str, "\n";
