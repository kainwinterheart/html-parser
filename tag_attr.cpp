#ifndef _TAG_ATTR_CPP_
#define _TAG_ATTR_CPP_

#include "tag_attr.h"

namespace Salvation {

    namespace HTMLLike {

        TagAttr::TagAttr( std::string * name, std::string * value ) {

            this -> _name = name;
            this -> _value = value;
        }

        TagAttr::~TagAttr() {

            delete this -> _name;
            delete this -> _value;
        }

        std::string * TagAttr::get_name() {

            return this -> _name;
        }

        std::string * TagAttr::get_value() {

            return this -> _value;
        }
    }
}

#endif /* end of include guard: _TAG_ATTR_CPP_ */
