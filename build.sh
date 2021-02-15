[ ! -d build ] && mkdir build
[ ! -d tmp ] && mkdir tmp
cd tmp
if [ $# -eq 0 ]; then
    cmake -DCMAKE_BUILD_TYPE=Release ..
else
    if [ "$1" == "0" ]; then
        cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
    fi
    if [ "$1" == "1" ]; then
        cmake -DCMAKE_BUILD_TYPE=Release ..
    fi
fi
make
