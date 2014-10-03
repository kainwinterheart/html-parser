#include "helper.cpp"

MODULE = Salvation::HTMLLike::XS PACKAGE = Salvation::HTMLLike::XS

PROTOTYPES: DISABLED

AV*
tokenize(str)
        char * str
    CODE:
        RETVAL = tokenize_main( str );

    OUTPUT:
        RETVAL
