<?php

$excludes = [           
    // "src/utils/str_printf.h",
    // "src/utils/KeyRange.h",
    // "tests/test_utils.h",
    "libs/vcpkg/installed/x64-linux/include/json/value.h",
    "libs/nlopt/build/nlopt.hpp",
];

$extras = [
    // install libwebsockets: 
    // https://libwebsockets.org/
    // https://github.com/warmcat/libwebsockets/blob/v4.3-stable/READMEs/README.build.md
    "-I" . realpath("libs/libwebsockets/build/include"),
    "-L" . realpath("libs/libwebsockets/build/lib"),

    // intall jsoncpp:
    // https://github.com/open-source-parsers/jsoncpp/blob/master/README.md#jsoncpp
    "-I" . realpath("libs/vcpkg/installed/x64-linux/include"),
    "-L" . realpath("libs/vcpkg/installed/x64-linux/lib"),

    // install nlopt: 
    // https://nlopt.readthedocs.io/en/latest/#download-and-installation
    // https://github.com/stevengj/nlopt/blob/master/README.md
    "-I" . realpath("libs/nlopt/build"),
    "-I" . realpath("libs/nlopt/src/api"),
    "-L" . realpath("libs/nlopt/build"),

    // install eigen: 
    // https://eigen.tuxfamily.org/dox/GettingStarted.html
    "-I" . realpath("libs/eigen"),

    // SDL
    "-I" . realpath("/usr/include/SDL2"),
    "-I" . realpath("/usr/include/SDL2_ttf"),

    "-lwebsockets",
    "-ljsoncpp",
    "-lnlopt",
    "-lSDL2 -lSDL2_ttf"
];