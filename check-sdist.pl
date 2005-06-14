#!/usr/bin/perl

use strict;
use warnings;
use diagnostics;

our $test = 0;
for (<DATA>) {
  my $first;
  my $second;
  my $dist;

  next if /^#/ or /^$/;

  $test++;
  ($first, $second, $dist) = split /\|/;

  open FIRST,  (">first");
  open SECOND, (">second");

  print FIRST $first;
  print SECOND $second;

  close FIRST;
  close SECOND;

  my $sdist_res = (split / /, `./sdist -s first second`)[0];

  if ($sdist_res != $dist) {
    print "result differs from expecte in test $test\n\n";
    print "$first$second\n\n";
    system "./sdist first second";
    exit 2;
  }

  unlink "first";
  unlink "second";
}

__END__

# 1
(A, B, C) | (A, B, C)   | 0

# 2..3
(A, B, C) | (A, (B, C)) | 0
(A, B, C) | ((A, B), C) | 0

# 4..5
(A, (B, C)) | (A, B, C) |  0
((A, B), C) | (A, B, C) |  0

(A,B,C,D) | (A,B,C,D) | 0

# 7..9
(A,B,C,D) | ((A,B),C,D) | 0
(A,B,C,D) | (A,B,(C,D)) | 0
(A,B,C,D) | (A,(B,C),D) | 0

# 10
(A,B,C,D) | ((A,B),(C,D)) | 0

# 11..13
((A,B),C,D) | (A,B,C,D) | 1
(A,B,(C,D)) | (A,B,C,D) | 1
(A,(B,C),D) | (A,B,C,D) | 1

# 14
((A,B),(C,D)) | (A,B,C,D) | 1

# 15..16
(A,B,C,D) | ((A, B, C), D) | 0
((A, B, C), D) | (A,B,C,D) | 0

# 17..18
((A,(B,C)),(D,E)) | ((A,(B,C)),D,E) | 0
((A,(B,C)),D,E) | ((A,(B,C)),(D,E)) | 0

# 19..20
(A,(B,C),(D,E)) | ((A,(B,C)),D,E) | 0
((A,(B,C)),D,E) | (A,(B,C),(D,E)) | 0

# 21
((A,(B,C)),(D,E)) | ((A,(B,C)),(D,E)) | 0
