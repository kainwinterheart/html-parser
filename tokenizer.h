#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include "tokenizer.h"

#include "token.h"
#include "tag_token.h"
#include <vector>
#include <string>
#include <pcrecpp.h>

namespace Salvation {

    namespace HTMLLike {

        class Tokenizer {

            public:
                Tokenizer();
                ~Tokenizer();

                static std::vector<Salvation::HTMLLike::Token*> * tokenize( std::string * s );
                static void erase( std::vector<Salvation::HTMLLike::Token*> * tokens );
        };
    }
}

#endif /* end of include guard: _TOKENIZER_H_ */
