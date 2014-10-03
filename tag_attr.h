#ifndef _TAG_ATTR_H_
#define _TAG_ATTR_H_

#include <string>

namespace Salvation {

    namespace HTMLLike {

        class TagAttr {

            public:
                TagAttr( std::string name, std::string value );
                ~TagAttr();

                std::string get_name();
                std::string get_value();

            private:
                std::string _name;
                std::string _value;
        };
    }
}

#endif /* end of include guard: _TAG_ATTR_H_ */
