#ifndef _TOKENIZER_CPP_
#define _TOKENIZER_CPP_

#include "tokenizer.h"
#include <stdio.h>

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

            pcrecpp::RE_Options * reopt = new pcrecpp::RE_Options ();

            reopt -> set_utf8( true );

            pcrecpp::RE word_re ( "[\\w-]", *reopt );
            pcrecpp::RE space_re ( "\\s", *reopt );

            const char * err = NULL;

            try {

                while( pos < length ) {

                    char chr = s -> at( pos ++ );

                    if( in_tag && ( chr == '\\' ) ) {

                        if( quoted ) {

                            if( tag_type == TOKEN_TAG_NOCONTENT ) throw "Unexpected character";

                            escaped = ! escaped;

                            if( escaped ) continue;

                            word -> push_back( chr );

                        } else {

                            throw "Unexpected character";
                        }

                    } else if( in_tag && ( ( chr == '"' ) || ( chr == '\'' ) ) && ( ( prev_quote == '\0' ) || ( prev_quote == chr ) ) ) {

                        if( tag_type == TOKEN_TAG_NOCONTENT ) throw "Unexpected character";

                        if( quoted ) {

                            if( escaped ) {

                                word -> push_back( chr );

                            } else {

                                quoted = false;

                                if( prev_word == NULL ) throw "Unexpected character";

                                attrs -> push_back( new Salvation::HTMLLike::TagAttr( prev_word, word ) );

                                word = new std::string ( "" );
                                prev_word = NULL;
                                type = 0;
                                prev_quote = '\0';
                            }

                        } else {

                            if( type != TOKEN_ATTR_VALUE ) throw "Unexpected character";

                            quoted = true;
                            type = TOKEN_STRING;
                            prev_quote = chr;
                        }

                    } else if( quoted ) {

                        word -> push_back( chr );

                    } else if( chr == '<' ) {

                        if( tag_type != 0 ) throw "Unexpected character";

                        if( word -> length() > 0 ) {

                            tokens -> push_back( new Salvation::HTMLLike::Token( TOKEN_DATA, word ) );

                            word = new std::string ( "" );
                        }

                        type = TOKEN_TAG;
                        tag_type = TOKEN_TAG_OPEN;
                        in_tag = true;
                        tag = new std::string ( "" );

                    } else if( in_tag && ( chr == '/' ) ) {

                        if( tag_type != TOKEN_TAG_OPEN ) throw "Unexpected character";

                        if( tag -> length() > 0 ) {

                            tag_type = TOKEN_TAG_NOCONTENT;

                        } else {

                            tag_type = TOKEN_TAG_CLOSE;
                        }

                    } else if( in_tag && ( chr == '>' ) ) {

                        if( tag_type == 0 ) throw "Unexpected character";

                        if( tag -> length() > 0 ) {

                            if( word -> length() > 0 ) {

                                if( prev_word == NULL ) {

                                    prev_word = word;

                                } else {

                                    attrs -> push_back( new Salvation::HTMLLike::TagAttr( prev_word, word ) );

                                    prev_word = NULL;
                                }
                            }

                        } else {

                            if( word -> length() > 0 ) {

                                tag = word;

                            } else {

                                throw "No tag name";
                            }
                        }

                        if( prev_word != NULL ) {

                            attrs -> push_back( new Salvation::HTMLLike::TagAttr( prev_word, new std::string ( "" ) ) );

                            prev_word = NULL;
                        }

                        if( ( tag_type == TOKEN_TAG_CLOSE ) && ! attrs -> empty() ) {

                            throw "Closing tag can\'t contain attributes";
                        }

                        tokens -> push_back( new Salvation::HTMLLike::TagToken( tag_type, tag, attrs ) );

                        word = new std::string ( "" );
                        type = 0;
                        tag_type = 0;
                        in_tag = false;
                        tag = NULL;
                        attrs = new std::vector<Salvation::HTMLLike::TagAttr*> ();

                    } else if( in_tag && ( chr == ':' ) ) {

                        if( tag_type == TOKEN_TAG_NOCONTENT ) throw "Unexpected character";
                        if( ( word -> length() == 0 ) || ( tag -> length() > 0 ) ) throw "Unexpected character";

                        tag = word;
                        word = new std::string ( "" );
                        type = TOKEN_ATTR_NAME;

                    } else if( in_tag && ( chr == '=' ) ) {

                        if( tag_type == TOKEN_TAG_NOCONTENT ) throw "Unexpected character";
                        if( type != TOKEN_ATTR_NAME ) throw "Unexpected character";
                        if( prev_word != NULL ) throw "Unexpected character";

                        prev_word = word;
                        word = new std::string ( "" );
                        type = TOKEN_ATTR_VALUE;
                        space_in_attr_name = false;

                    } else if( in_tag && space_re.FullMatch( std::string ( 1, chr ) ) ) {

                        if( ( type != 0 ) && ( word -> length() > 0 ) ) {

                            if( tag_type == TOKEN_TAG_NOCONTENT ) throw "Unexpected character";

                            if( type == TOKEN_TAG ) {

                                if( tag -> length() > 0 ) throw "Unexpected character";

                                tag = word;
                                word = new std::string ( "" );
                                type = 0;

                            } else if( type == TOKEN_ATTR_VALUE ) {

                                if( prev_word == NULL ) throw "Unexpected token";

                                attrs -> push_back( new Salvation::HTMLLike::TagAttr( prev_word, word ) );

                                word = new std::string ( "" );
                                prev_word = NULL;
                                type = 0;

                            } else if( type == TOKEN_ATTR_NAME ) {

                                space_in_attr_name = true;
                            }
                        }

                    } else if( in_tag ) {

                        if( ! word_re.FullMatch( std::string ( 1, chr ) ) ) throw "Unexpected token";
                        if( tag_type == TOKEN_TAG_NOCONTENT ) throw "Unexpected character";

                        if( space_in_attr_name ) {

                            space_in_attr_name = false;

                            if( word -> length() > 0 ) {

                                attrs -> push_back( new Salvation::HTMLLike::TagAttr( word, new std::string ( "" ) ) );
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

                    tokens -> push_back( new Salvation::HTMLLike::Token( TOKEN_DATA, word ) );
                }

            } catch( const char * e ) {

                err = e;
                Tokenizer::erase( tokens );
            }

            delete reopt;
            delete attrs;

            if( err != NULL ) throw err;

            return tokens;
        }

        void Tokenizer::erase( std::vector<Salvation::HTMLLike::Token*> * tokens ) {

            while( ! tokens -> empty() ) {

                Salvation::HTMLLike::Token * token = tokens -> back();

                short type = token -> get_type();

                if(
                    ( type == TOKEN_TAG_OPEN ) || ( type == TOKEN_TAG_CLOSE )
                    || ( type == TOKEN_TAG_NOCONTENT )
                ) {

                    delete (Salvation::HTMLLike::TagToken*)token;

                } else {

                    delete token;
                }

                tokens -> pop_back();
            }

            delete tokens;
        }
    }
}

#endif /* end of include guard: _TOKENIZER_CPP_ */
