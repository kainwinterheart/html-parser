#ifndef _TAG_TOKEN_CPP_
#define _TAG_TOKEN_CPP_

#include "tag_token.h"

namespace Salvation {

    namespace HTMLLike {

        TagToken::TagToken( short type, std::string * word, std::vector<Salvation::HTMLLike::TagAttr*> * attrs )
            : Salvation::HTMLLike::Token::Token( type, word ) {

            this -> _attrs = attrs;
        }

        TagToken::~TagToken() {

            std::vector<Salvation::HTMLLike::TagAttr*> * attrs = this -> _attrs;

            while( ! attrs -> empty() ) {

                Salvation::HTMLLike::TagAttr * attr = attrs -> back();

                delete attr;

                attrs -> pop_back();
            }

            delete attrs;
        }

        std::vector<Salvation::HTMLLike::TagAttr*> * TagToken::get_attrs() {

            return this -> _attrs;
        }
    }
}

#endif /* end of include guard: _TAG_TOKEN_CPP_ */
