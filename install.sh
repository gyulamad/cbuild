#!/bin/bash

# run: yes | ./install.sh


# note: libwebsockets: cmake .. -DLWS_WITH_MINIMAL_EXAMPLES=1 -DCMAKE_BUILD_TYPE=DEBUG -DLWS_WITH_SSL=1 -DLWS_WITH_HTTP2=1 -DLWS_WITHOUT_EXTENSIONS=0  && make && sudo make install
# nlopt: 
#   https://nlopt.readthedocs.io/en/latest/#download-and-installation
#   https://github.com/stevengj/nlopt


libwebsockets_branch="v4.3-stable"
libwebsockets_cmake="-DLWS_ROLE_WS=1 -DLWS_WITHOUT_EXTENSIONS=0"
jsoncpp_branch="1.9.5"
jsoncpp_cmake=""
nlopt_branch="v2.7.1"
nlopt_cmake="-DBUILD_SHARED_LIBS=OFF"
eigen_branch="3.4"
eigen_cmake=""
build_options="-c"

function confirm_input {
    local message=$1
    local default=$2

    if [[ "$default" == "y" ]]; then
        options="Y/n"
    else
        options="y/N"
    fi

    while true; do
        read -p "$message [$options] "  -n 1 -r 
        echo
        case $REPLY in
            [Yy] ) return 0;;
            [Nn] ) return 1;;
            "" ) if [[ "$default" == "y" ]]; then return 0; else return 1; fi;;
            * ) echo "Invalid input, please enter y or n";;
        esac
    done
}


# ---------- create libs if not exsists

mkdir -p libs
cd libs

# ---------- install libwebsockets ------------------

echo "Installing libwebsockets..."

if [ -d libwebsockets ]; then
    if confirm_input "libwebsockets is already exists, do you want to re-install?" y; then
        rm -rf libwebsockets 
    fi
fi

# download if not exists
if [ ! -d libwebsockets ]; then
    git clone https://libwebsockets.org/repo/libwebsockets
    cd libwebsockets
    git checkout $libwebsockets_branch
    cd ..
fi

# --------- build/install libwebsockets ----------------

cd libwebsockets
if [ -d build ]; then
    if confirm_input "libwebsockets is already built, do you want to re-build?" y; then
        rm -rf build 
    fi
fi

# build if not already built
if [ ! -d build ]; then 
    mkdir build
    cd build
    cmake .. $libwebsockets_cmake
    make
    cd ..
fi
cd ..

# ---------- install jsoncpp ------------------

echo "Installing jsoncpp..."

if [ -d vcpkg ]; then
    if confirm_input "vcpkg is already exists, do you want to re-install?" y; then
        rm -rf vcpkg 
    fi
fi

# download if not exists
if [ ! -d vcpkg ]; then
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    ./vcpkg install jsoncpp
    cd ..
fi

# ---------- install nlopt ------------------

echo "Installing nlopt..."

if [ -d nlopt ]; then
    if confirm_input "nlopt is already exists, do you want to re-install?" y; then
        rm -rf nlopt 
    fi
fi

# download if not exists
if [ ! -d nlopt ]; then
    git clone https://github.com/stevengj/nlopt.git
    cd nlopt
    git checkout $nlopt_branch
    cd ..
fi

# --------- build/install nlopt ----------------

cd nlopt
if [ -d build ]; then
    if confirm_input "nlopt is already built, do you want to re-build?" y; then
        rm -rf build 
    fi
fi

# build if not already built
if [ ! -d build ]; then 
    mkdir build
    cd build
    cmake .. $nlopt_cmake
    make
    cd ..
fi
cd ..

# ---------- install eigen ------------------

echo "Installing eigen..."

if [ -d eigen ]; then
    if confirm_input "eigen is already exists, do you want to re-install?" y; then
        rm -rf eigen 
    fi
fi

# download if not exists
if [ ! -d eigen ]; then
    git clone https://gitlab.com/libeigen/eigen.git
    cd eigen
    git checkout $eigen_branch
    cd ..
fi

# ---------- libs created ----------
cd ..

# -------------- build project -------------

echo "Building main project..."

if [ -d build ]; then
    if confirm_input "main project is already built, do you want to clean up?" y; then
        php build.php $build_options
    fi
fi


# ---------------- finished ----------------
echo "\033[36m"
echo "------------------------------------------"
echo "       Install and build finished."
echo "------------------------------------------"
echo "\033[0;0;0m"
make help