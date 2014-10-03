#ifndef _TAG_TOKEN_CPP_
#define _TAG_TOKEN_CPP_

#include "tag_token.h"

namespace Salvation {

    namespace HTMLLike {

        TagToken::TagToken( short type, std::string * word, std::vector<Salvation::HTMLLike::TagAttr*> * attrs )
            : Salvation::HTMLLike::Token::Token( type, word ), _attrs( attrs ) {

            this -> _attrs = attrs;
        }

        TagToken::~TagToken() {

            std::vector<Salvation::HTMLLike::TagAttr*> * attrs = this -> _attrs;

            for( std::vector<Salvation::HTMLLike::TagAttr*>::iterator it = attrs -> begin(); it != attrs -> end(); ++it ) {

                delete *it;
            }

            delete attrs;
        }

        std::vector<Salvation::HTMLLike::TagAttr*> * TagToken::get_attrs() {

            return this -> _attrs;
        }
    }
}

#endif /* end of include guard: _TAG_TOKEN_CPP_ */
