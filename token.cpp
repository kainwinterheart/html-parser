#ifndef _TOKEN_CPP_
#define _TOKEN_CPP_

#include "token.h"

namespace Salvation {

    namespace HTMLLike {

        Token::Token( short type, std::string * word ) {

            this -> _type = type;
            this -> _word = word;
        }

        Token::~Token() {

            delete this -> _word;
        }

        short Token::get_type() {

            return this -> _type;
        }

        std::string * Token::get_word() {

            return this -> _word;
        }
    }
}

#endif /* end of include guard: _TOKEN_CPP_ */
