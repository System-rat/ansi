#!/usr/bin/env bash
# A Simple shell script for some common tasks like configuring, testing and tidying

case $1 in
    configure)
        FLAGS=()

        if [[ -n $2 ]]
        then
            case $2 in
                debug)
                    FLAGS+=("-DCMAKE_BUILD_TYPE=Debug");;
                release)
                    FLAGS+=("-DCMAKE_BUILD_TYPE=Debug");;
                *)
                    echo "unkown build type";
                    exit 1;
            esac
        fi

        if [[ -d build ]]
        then
            rm -rf build/
        fi

        cmake -B build -S . -G Ninja $FLAGS;;

    test)
        if [[ -d build ]]
        then
            cd build && ninja tests && ctest
        else
            cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug &&
            cd build && ninja tests && ctest
        fi;;

    tidy)
        clang-tidy src/**/*.cpp src/**/*.h src/*.cpp include/*.h -p build/;;

    *)
        echo -e "USAGE utils.sh <command>";
        echo;
        echo "Simple util shell script for building and linting";
        echo -e "  configure [mode] \tRun cmake in [mode]";
        echo -e "  tidy             \tRun clang-tidy";
        echo -e "  test             \tConfigure and run tests";
        exit 1;
        ;;
esac
