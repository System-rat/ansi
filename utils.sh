#!/usr/bin/env bash
# A Simple shell script for some common tasks like configuring, testing and tidying

error() {
    if [[ -t 1 ]]
    then
        echo -ne "\e[31m";
    fi
    echo $@;
    if [[ -t 1 ]]
    then
        echo -ne "\e[0m";
    fi
}

case $1 in
    configure)
        FLAGS=()

        if [[ -n $2 ]]
        then
            case $2 in
                debug)
                    FLAGS+=("-DCMAKE_BUILD_TYPE=Debug");;
                release)
                    FLAGS+=("-DCMAKE_BUILD_TYPE=Release");;
                *)
                    error "unkown build type:" $2;
                    exit 1;
            esac
        fi

        if [[ -d build ]]
        then
            rm -rf build/
        fi

        cmake -B build -S . -G Ninja $FLAGS;;

    test)
        if [[ -n $2 ]]
        then
            if [[ $2 == "--force" ]]
            then
                if [[ -d build ]]
                then
                    rm -rf build
                fi
            else
                error "unkown option:" $2;
                exit 1;
            fi
        fi

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
        echo -e \
"USAGE utils.sh <command>

Simple util shell script for building and linting
  configure [mode] \tRun cmake with an optional build type
                   \tSupported build types: debug, release

  tidy             \tRun clang-tidy

  test [--force]   \tConfigure and run tests
                   \tIf --force is specified, the tests
                   \twill be forced to rebuild";
        exit 1;
        ;;
esac
