#ifndef _TAG_TOKEN_H_
#define _TAG_TOKEN_H_

#include "token.h"
#include "tag_attr.h"
#include <string>
#include <vector>

namespace Salvation {

    namespace HTMLLike {

        class TagToken : public Salvation::HTMLLike::Token {

            public:
                TagToken( short type, std::string word, std::vector<Salvation::HTMLLike::TagAttr*> * attrs );
                ~TagToken();

                std::vector<Salvation::HTMLLike::TagAttr*> * get_attrs();

            private:
                std::vector<Salvation::HTMLLike::TagAttr*> * _attrs;
        };
    }
}

#endif /* end of include guard: _TAG_TOKEN_H_ */
