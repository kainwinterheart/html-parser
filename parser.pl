#!/usr/bin/perl -w

use utf8;
use strict;
use warnings;
use boolean;

use constant {

    TOKEN_TAG_OPEN => 1,
    TOKEN_TAG_CLOSE => 2,
    TOKEN_STRING => 3,
    TOKEN_ATTR_NAME => 4,
    TOKEN_ATTR_VALUE => 5,
    TOKEN_TAG => 6,
    TOKEN_DATA => 7,
    TOKEN_TAG_NOCONTENT => 8,

    TAGMAP => {
        strong => 'hilight',
        tpl => 'template',
        msp => 'misspell',
    },
};

{
    my $s ="«A B C». D E <strong color=red>REDWORD</strong>. <tpl><msp>F</msp> G</tpl><strong color=red>REDFOO</strong>";
    my @content = ();

    create_struct( tokenize_str( $s ), \@content );

    require Data::Dumper;

    print Data::Dumper::Dumper( [ $s, \@content ] );
}

exit 0;

sub create_struct {

    my ( $tokens, $out ) = @_;

    while( defined( my $token = shift( @$tokens ) ) ) {

        if( $token -> { 'type' } == TOKEN_DATA ) {

            push( @$out, $token -> { 'word' } );

        } elsif( $token -> { 'type' } == TOKEN_TAG_OPEN ) {

            push( @$out, {
                content => ( my $node = [] ),
                map( { $_ -> { 'name' } => $_ -> { 'value' } } @{ $token -> { 'attrs' } } ),
                type => TAGMAP -> { $token -> { 'word' } },
            } );

            create_struct( $tokens, $node );

        } elsif( $token -> { 'type' } == TOKEN_TAG_CLOSE ) {

            last;
        }
    }
}

sub tokenize_str {

    my ( $s ) = @_;

    my $pos = 0;
    my $length = length( $s );

    my @tokens = ();

    my $tag = undef;
    my @attrs = ();
    my $prev_word = undef;

    my $word = '';
    my $type = undef;
    my $tag_type = undef;

    my $in_tag = false;
    my $quoted = false;
    my $escaped = false;
    my $space_in_attr_name = false;

    my %quotes = map( { $_ => 1 } qw{ ' " } );
    my $prev_quote = undef;

    my $word_re = qr/[\w-]/;
    my $space_re = qr/\s/;

    while( defined( my $char = substr( $s, $pos++, 1 ) ) ) {

        if( $in_tag && ( $char eq '\\' ) ) {

            if( $quoted ) {

                die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );

                $escaped = ! $escaped;

                next if $escaped;

                $word .= $char;

            } else {

                die( 'Unexpected character' );
            }

        } elsif( $in_tag && exists $quotes{ $char } && ( ! defined $prev_quote || ( $prev_quote eq $char ) ) ) {

            die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );

            if( $quoted ) {

                if( $escaped ) {

                    $word .= $char;

                } else {

                    $quoted = false;

                    die( 'Unexpected token' ) unless defined $prev_word;

                    push( @attrs, {
                        name => $prev_word,
                        value => $word,
                    } );

                    $word = '';
                    undef $prev_word;
                    undef $type;
                    undef $prev_quote;
                }

            } else {

                die( 'Unexpected character' ) if defined $type && ( $type != TOKEN_ATTR_VALUE );

                $quoted = true;
                $type = TOKEN_STRING;
                $prev_quote = $char;
            }

        } elsif( $quoted ) {

            $word .= $char;

        } elsif( $char eq '<' ) {

            die( 'Unexpected character' ) if defined $tag_type;

            if( length( $word ) > 0 ) {

                push( @tokens, {
                    type => TOKEN_DATA,
                    word => $word,
                } );

                $word = '';
            }

            $type = TOKEN_TAG;
            $tag_type = TOKEN_TAG_OPEN;
            $in_tag = true;
            $tag = '';

        } elsif( $in_tag && ( $char eq '/' ) ) {

            die( 'Unexpected character' ) if ! defined $tag_type || ( $tag_type != TOKEN_TAG_OPEN );

            if( length( $tag ) > 0 ) {

                $tag_type = TOKEN_TAG_NOCONTENT;

            } else {

                $tag_type = TOKEN_TAG_CLOSE;
            }

        } elsif( $in_tag && ( $char eq '>' ) ) {

            die( 'Unexpected character' ) if ! defined $tag_type;

            if( length( $tag ) > 0 ) {

                if( length( $word ) > 0 ) {

                    if( defined $prev_word ) {

                        push( @attrs, {
                            name => $prev_word,
                            value => $word,
                        } );

                        undef $prev_word;

                    } else {

                        $prev_word = $word;
                    }
                }

            } else {

                if( length( $word ) > 0 ) {

                    $tag = $word;

                } else {

                    die( 'No tag name' );
                }
            }

            if( defined $prev_word ) {

                push( @attrs, {
                    name => $prev_word,
                    value => undef,
                } );

                undef $prev_word;
            }

            if( ( $tag_type == TOKEN_TAG_CLOSE ) && ( scalar( @attrs ) > 0 ) ) {

                die( 'Closing tag can\'t contain attributes' );
            }

            push( @tokens, {
                type => $tag_type,
                word => $tag,
                attrs => [ @attrs ],
            } );

            $word = '';
            undef $type;
            undef $tag_type;
            $in_tag = false;
            undef $tag;
            undef @attrs;

        } elsif( $in_tag && ( $char eq ':' ) ) {

            die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );
            die( 'Unexpected character' ) if ( length( $word ) == 0 ) || ( length( $tag ) > 0 );

            $tag = $word;
            $word = '';
            $type = TOKEN_ATTR_NAME;

        } elsif( $in_tag && ( $char eq '=' ) ) {

            die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );
            die( 'Unexpected character' ) if ! defined $type || ( $type != TOKEN_ATTR_NAME );
            die( 'Unexpected character' ) if defined $prev_word;

            $prev_word = $word;
            $word = '';
            $type = TOKEN_ATTR_VALUE;
            $space_in_attr_name = false;

        } elsif( $in_tag && ( $char =~ $space_re ) ) {

            if( defined $type && length( $word ) > 0 ) {

                die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );

                if( $type == TOKEN_TAG ) {

                    die( 'Unexpected character' ) if ( length( $tag ) > 0 );

                    $tag = $word;
                    $word = '';
                    undef $type;

                } elsif( $type == TOKEN_ATTR_VALUE ) {

                    die( 'Unexpected token' ) unless defined $prev_word;

                    push( @attrs, {
                        name => $prev_word,
                        value => $word,
                    } );

                    $word = '';
                    undef $prev_word;
                    undef $type;

                } elsif( $type == TOKEN_ATTR_NAME ) {

                    $space_in_attr_name = true;
                }
            }

        } elsif( $in_tag ) {

            die( 'Unexpected token' ) if( $char !~ $word_re );
            die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );

            if( $space_in_attr_name ) {

                $space_in_attr_name = false;

                push( @attrs, {
                    name => $word,
                    value => undef,

                } ) if ( length( $word ) > 0 );

                $word = $char;
                $type = TOKEN_ATTR_NAME;

            } else {

                $word .= $char;
                $type //= TOKEN_ATTR_NAME;
            }

        } else {

            $word .= $char;
        }

        $escaped = false if $escaped;
        last if ( $pos > $length );
    }

    if( length( $word ) > 0 ) {

        push( @tokens, {
            type => TOKEN_DATA,
            word => $word,
        } );
    }

    return \@tokens;
}

__END__
