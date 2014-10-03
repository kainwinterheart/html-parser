#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <string>
#include <vector>
#include "tokenizer.h"

// int main( int argc, char * argv[] ) {
//
//     std::string input ( "" );
//
//     for( int i = 1; i < argc; ++i ) {
//
//         input.append( argv[ i ] );
//     }
//
//     std::vector<Salvation::HTMLLike::Token*> * tokens;
//
//     try {
//         tokens = Salvation::HTMLLike::Tokenizer::tokenize( &input );
//
//     } catch( const char * e ) {
//
//         printf( "error: %s\n", e );
//         tokens = new std::vector<Salvation::HTMLLike::Token*> ();
//     }
//

//
//
//
//     return 0;
// }


MODULE = Salvation::HTMLLike::XS PACKAGE = Salvation::HTMLLike::XS

PROTOTYPES: DISABLED

SV * get_str_sv( std::string * str ) {

    SV * sv = newSVpvn( str -> c_str(), str -> length() );

    sv_2mortal( sv );

    return sv;
}

void hash_set( HV * hv, std::string * key, int value ) {

    hash_set( hv, key, &( std::to_string( i ) ) );
}

void hash_set( HV * hv, std::string * key, std::string * value ) {

    hv_store( hv, key -> c_str(), key -> length(), get_str_sv( value ), 0 );
}

FooContext*
Foo::new()
    PREINIT:
        FooContext* ctx;
    CODE:
        Newx(ctx, 1, FooContext);
        ctx->val = 0;
        RETVAL=ctx;
    OUTPUT:
        RETVAL

AV*
tokenize(str)
        char * str
    CODE:
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

        RETVAL = newAV();
        sv_2mortal( (SV*)RETVAL );

        std::string * key = new std::string ( "" );

        for( std::vector<Salvation::HTMLLike::Token*>::iterator it = tokens -> begin(); it != tokens -> end(); ++it ) {

            HV * perl_token = newHV();
            sv_2mortal( (SV*)perl_token );

            key -> assign( "type" );
            hash_set( perl_token, key, (*it) -> get_type() );

            key -> assign( "word" );
            hash_set( perl_token, key, (*it) -> get_word() );

            switch ( (*it) -> get_type() ) {

                case 1:
                case 2:
                case 8:
                    std::vector<Salvation::HTMLLike::TagAttr*> * attrs = ((Salvation::HTMLLike::TagToken*)*it) -> get_attrs();
                    AV * perl_attrs = newAV();
                    sv_2mortal( (SV*)perl_attrs );

                    for( std::vector<Salvation::HTMLLike::TagAttr*>::iterator it = attrs -> begin(); it != attrs -> end(); ++it ) {

                        HV * perl_attr = newHV();
                        sv_2mortal( (SV*)perl_attr );

                        key -> assign( "name" );
                        hash_set( perl_attr, key, (*it) -> get_name() );

                        key -> assign( "value" );
                        hash_set( perl_attr, key, (*it) -> get_value() );

                        av_push( perl_attrs, (SV*)perl_attr );
                    }

                    key -> assign( "attrs" );
                    hash_set( perl_token, key, (SV*)perl_attrs );
                    break;
                default:
                    break;
            }

            av_push( RETVAL, (SV*)perl_token );
        }

        delete key;
        Salvation::HTMLLike::Tokenizer::erase( tokens );

    OUTPUT:
        RETVAL
