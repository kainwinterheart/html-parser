#ifndef _TOKENIZER_CPP_
#define _TOKENIZER_CPP_

#include "tokenizer.h"

#define TOKEN_TAG_OPEN 1
#define TOKEN_TAG_CLOSE 2
#define TOKEN_STRING 3
#define TOKEN_ATTR_NAME 4
#define TOKEN_ATTR_VALUE 5
#define TOKEN_TAG 6
#define TOKEN_DATA 7
#define TOKEN_TAG_NOCONTENT 8

namespace Salvation {

    namespace HTMLLike {

        Tokenizer::Tokenizer() {


        }

        Tokenizer::~Tokenizer() {


        }
/*
my $pos = 0;
my $length = length( $s );

my @tokens = ();

my $tag = undef;
my @attrs = ();
my $prev_word = undef;

my $word = '\0';
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
*/
        std::vector<Salvation::HTMLLike::Token*> * Tokenizer::tokenize( std::string * s ) {

            std::vector<Salvation::HTMLLike::Token*> * tokens = new std::vector<Salvation::HTMLLike::Token*> ();

            int pos = 0;
            int length = s -> length();

            std::string * tag = NULL;
            std::vector<Salvation::HTMLLike::TagAttr*> * attrs = new std::vector<Salvation::HTMLLike::TagAttr*> ();
            std::string * prev_word = NULL;

            std::string * word = new std::string( "" );
            short type = 0;
            short tag_type = 0;

            bool in_tag = false;
            bool quoted = false;
            bool escaped = false;
            bool space_in_attr_name = false;

            char prev_quote = '\0';

            while( pos < length ) {

                char chr = s -> at( pos ++ );

                if( in_tag && ( chr == '\\' ) ) {

                    if( quoted ) {

                        // die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );

                        escaped = ! escaped;

                        if( escaped ) continue;

                        word -> push_back( chr );

                    } else {

                        // die( 'Unexpected character' );
                    }

                } else if( in_tag && ( ( chr == '"' ) || ( chr == '\'' ) ) && ( ( prev_quote == '\0' ) || ( prev_quote == chr ) ) ) {

                    // die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );

                    if( quoted ) {

                        if( escaped ) {

                            word -> push_back( chr );

                        } else {

                            quoted = false;

                            // die( 'Unexpected token' ) unless defined $prev_word;

                            attrs -> push_back( new Salvation::HTMLLike::TagAttr( *prev_word, *word ) );

                            word = new std::string ( "" );
                            prev_word = NULL;
                            type = 0;
                            prev_quote = '\0';
                        }

                    } else {

                        // die( 'Unexpected character' ) if defined $type && ( $type != TOKEN_ATTR_VALUE );

                        quoted = true;
                        type = TOKEN_STRING;
                        prev_quote = chr;
                    }

                } else if( quoted ) {

                    word -> push_back( chr );

                } else if( chr == '<' ) {

                    // die( 'Unexpected character' ) if defined $tag_type;

                    if( word -> length() > 0 ) {

                        tokens -> push_back( new Salvation::HTMLLike::Token( TOKEN_DATA, *word ) );

                        word = new std::string ( "" );
                    }

                    type = TOKEN_TAG;
                    tag_type = TOKEN_TAG_OPEN;
                    in_tag = true;
                    tag = new std::string ( "" );

                } else if( in_tag && ( chr == '/' ) ) {

                    // die( 'Unexpected character' ) if ! defined $tag_type || ( $tag_type != TOKEN_TAG_OPEN );

                    if( tag -> length() > 0 ) {

                        tag_type = TOKEN_TAG_NOCONTENT;

                    } else {

                        tag_type = TOKEN_TAG_CLOSE;
                    }

                } else if( in_tag && ( chr == '>' ) ) {

                    // die( 'Unexpected character' ) if ! defined $tag_type;

                    if( tag -> length() > 0 ) {

                        if( word -> length() > 0 ) {

                            if( prev_word == NULL ) {

                                prev_word = word;

                            } else {

                                attrs -> push_back( new Salvation::HTMLLike::TagAttr( *prev_word, *word ) );

                                prev_word = NULL;
                            }
                        }

                    } else {

                        if( word -> length() > 0 ) {

                            tag = word;

                        } else {

                            // die( 'No tag name' );
                        }
                    }

                    if( prev_word != NULL ) {

                        attrs -> push_back( new Salvation::HTMLLike::TagAttr( *prev_word, std::string ( "" ) ) );

                        prev_word = NULL;
                    }

                    if( ( tag_type == TOKEN_TAG_CLOSE ) && ! attrs -> empty() ) {

                        // die( 'Closing tag can\'t contain attributes' );
                    }

                    tokens -> push_back( new Salvation::HTMLLike::TagToken( tag_type, *tag, attrs ) );

                    word = new std::string ( "" );
                    type = 0;
                    tag_type = 0;
                    in_tag = false;
                    tag = NULL;
                    attrs = new std::vector<Salvation::HTMLLike::TagAttr*> ();

                } else if( in_tag && ( chr == ':' ) ) {

                    // die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );
                    // die( 'Unexpected character' ) if ( length( $word ) == 0 ) || ( length( $tag ) > 0 );

                    tag = word;
                    word = new std::string ( "" );
                    type = TOKEN_ATTR_NAME;

                } else if( in_tag && ( chr == '=' ) ) {

                    // die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );
                    // die( 'Unexpected character' ) if ! defined $type || ( $type != TOKEN_ATTR_NAME );
                    // die( 'Unexpected character' ) if defined $prev_word;

                    prev_word = word;
                    word = new std::string ( "" );
                    type = TOKEN_ATTR_VALUE;
                    space_in_attr_name = false;

                } else if( in_tag && ( chr == ' ' ) ) {

                    if( ( type != 0 ) && ( word -> length() > 0 ) ) {

                        // die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );

                        if( type == TOKEN_TAG ) {

                            // die( 'Unexpected character' ) if ( length( $tag ) > 0 );

                            tag = word;
                            word = new std::string ( "" );
                            type = 0;

                        } else if( type == TOKEN_ATTR_VALUE ) {

                            // die( 'Unexpected token' ) unless defined $prev_word;

                            attrs -> push_back( new Salvation::HTMLLike::TagAttr( *prev_word, *word ) );

                            word = new std::string ( "" );
                            prev_word = NULL;
                            type = 0;

                        } else if( type == TOKEN_ATTR_NAME ) {

                            space_in_attr_name = true;
                        }
                    }

                } else if( in_tag ) {

                    // die( 'Unexpected token' ) if( $char !~ $word_re );
                    // die( 'Unexpected character' ) if defined $tag_type && ( $tag_type == TOKEN_TAG_NOCONTENT );

                    if( space_in_attr_name ) {

                        space_in_attr_name = false;

                        if( word -> length() > 0 ) {

                            attrs -> push_back( new Salvation::HTMLLike::TagAttr( *word, std::string ( "" ) ) );
                        }

                        word = new std::string ( "" );
                        word -> push_back( chr );
                        type = TOKEN_ATTR_NAME;

                    } else {

                        word -> push_back( chr );
                        if( type == 0 ) type = TOKEN_ATTR_NAME;
                    }

                } else {

                    word -> push_back( chr );
                }

                if( escaped ) escaped = false;
            }

            if( word -> length() > 0 ) {

                tokens -> push_back( new Salvation::HTMLLike::Token( TOKEN_DATA, *word ) );
            }

            return tokens;
        }
    }
}

#endif /* end of include guard: _TOKENIZER_CPP_ */
