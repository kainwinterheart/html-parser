use ExtUtils::MakeMaker;

WriteMakefile (
    NAME => 'Salvation::HTMLLike::XS',
    VERSION => '0.01',
    OBJECT => 'tag_attr.o tag_token.o token.o tokenizer.o XS.o',
    CC => 'g++',
    CCCDLFLAGS => '-std=c++0x -fPIC',
    LDFLAGS => '-fstack-protector -L/usr/local/lib -lpcrecpp',
    LDDLFLAGS => '-shared -O2 -g -L/usr/local/lib -fstack-protector -lpcrecpp',
);

exit 0;

__END__
