#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>

namespace Salvation {

    namespace HTMLLike {

        class Token {

            public:
                Token( short type, std::string word );
                ~Token();

                short get_type();
                std::string get_word();

            private:
                short _type;
                std::string _word;
        };
    }
}

#endif /* end of include guard: _TOKEN_H_ */
