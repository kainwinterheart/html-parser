use ExtUtils::MakeMaker;

WriteMakefile (
    NAME => 'Salvation::HTMLLike::XS',
    VERSION => '0.01',
    OBJECT => 'tag_attr.o tag_token.o token.o tokenizer.o XS.o',
    CC => 'g++',
    CCCDLFLAGS => '-std=c++0x -fPIC',
    LIBS => '-lpcrecpp',
);

exit 0;

__END__
