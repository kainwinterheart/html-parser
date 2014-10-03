#!/usr/bin/perl -w

use strict;
use warnings;

use lib 'blib/lib';
use blib 'blib/arch/auto/';

use Salvation::HTMLLike::XS ();
use Data::Dumper 'Dumper';

print Dumper( Salvation::HTMLLike::XS::tokenize( shift( @ARGV ) ) );

exit 0;

__END__
