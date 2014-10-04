#ifndef _HELPER_CPP_
#define _HELPER_CPP_

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <string>
#include <vector>
#include "tokenizer.h"

SV * get_str_sv( std::string * str ) {

    SV * sv = newSVpvn( str -> c_str(), str -> length() );

    return sv;
}

void hash_set( HV * hv, std::string * key, SV * value ) {

    hv_store( hv, key -> c_str(), key -> length(), value, 0 );
}

void hash_set( HV * hv, std::string * key, std::string * value ) {

    hash_set( hv, key, get_str_sv( value ) );
}

void hash_set( HV * hv, std::string * key, int value ) {

    std::string str = std::to_string( value );

    hash_set( hv, key, &str );
}

AV * tokenize_main( char * str ) {

    std::string input ( "" );
    std::vector<Salvation::HTMLLike::Token*> * tokens;

    input.append( str );

    try {
        tokens = Salvation::HTMLLike::Tokenizer::tokenize( &input );

    } catch( const char * e ) {

        printf( "error: %s\n", e );
        delete e;
        tokens = new std::vector<Salvation::HTMLLike::Token*> ();
    }

    AV * perl_tokens = newAV();
    sv_2mortal( (SV*)perl_tokens );

    std::string * key = new std::string ( "" );

    for( std::vector<Salvation::HTMLLike::Token*>::iterator it = tokens -> begin(); it != tokens -> end(); ++it ) {

        HV * perl_token = newHV();

        short type = (*it) -> get_type();

        key -> assign( "type" );
        hash_set( perl_token, key, type );

        key -> assign( "word" );
        hash_set( perl_token, key, (*it) -> get_word() );

        if( ( type == 1 ) || ( type == 2 ) || ( type == 8 ) ) {

            std::vector<Salvation::HTMLLike::TagAttr*> * attrs = ((Salvation::HTMLLike::TagToken*)*it) -> get_attrs();
            AV * perl_attrs = newAV();

            for( std::vector<Salvation::HTMLLike::TagAttr*>::iterator it = attrs -> begin(); it != attrs -> end(); ++it ) {

                HV * perl_attr = newHV();

                key -> assign( "name" );
                hash_set( perl_attr, key, (*it) -> get_name() );

                key -> assign( "value" );
                hash_set( perl_attr, key, (*it) -> get_value() );

                av_push( perl_attrs, newRV_noinc( (SV*)perl_attr ) );
            }

            key -> assign( "attrs" );
            hash_set( perl_token, key, newRV_noinc( (SV*)perl_attrs ) );
            delete attrs;
        }

        av_push( perl_tokens, newRV_noinc( (SV*)perl_token ) );
    }

    delete key;
    Salvation::HTMLLike::Tokenizer::erase( tokens );

    return perl_tokens;
}

#endif /* end of include guard: _HELPER_CPP_ */
