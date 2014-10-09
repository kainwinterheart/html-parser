#!/usr/bin/perl -w

use strict;
use warnings;

use lib 'blib/lib';
use blib 'blib/arch/auto/';

use Salvation::HTMLLike::XS ();
use Data::Dumper 'Dumper';
use Encode 'encode_utf8';

my $s = encode_utf8( shift( @ARGV ) // '' );

Salvation::HTMLLike::XS::tokenize( $s ) for 1 .. 10000;

#print "Wait...\n";

#sleep 30;

print Dumper( Salvation::HTMLLike::XS::tokenize( $s ) );

exit 0;

__END__
