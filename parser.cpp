#include "tokenizer.h"
#include <stdio.h>

int main( int argc, char ** argv ) {

    std::string input ( "a <b> c</b> d <strong color=red>e</strong>" );

    std::vector<Salvation::HTMLLike::Token*> * tokens = Salvation::HTMLLike::Tokenizer::tokenize( &input );

    for( std::vector<Salvation::HTMLLike::Token*>::iterator it = tokens -> begin(); it != tokens -> end(); ++it ) {

        printf( "type: %d, word: %s\n", (*it) -> get_type(), (*it) -> get_word().c_str() );

        if( (*it) -> get_type() == 1 ) {

            std::vector<Salvation::HTMLLike::TagAttr*> * attrs = ((Salvation::HTMLLike::TagToken*)*it) -> get_attrs();

            for( std::vector<Salvation::HTMLLike::TagAttr*>::iterator it = attrs -> begin(); it != attrs -> end(); ++it ) {

                printf( "attrs name: %s, value: %s\n", (*it) -> get_name().c_str(), (*it) -> get_value().c_str() );
            }
        }
    }

    return 0;
}
