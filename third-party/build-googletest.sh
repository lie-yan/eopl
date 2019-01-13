#! /bin/bash

# obtain the script directory
SCRIPT_DIR=$(dirname "$0")
cd ${SCRIPT_DIR}

# check googletest directory
GTEST_DIR="googletest"
if [ -d ${GTEST_DIR} ]
then
    echo "Found directory ${GTEST_DIR}."
else
    echo "Failed to find directory ${GTEST_DIR}."
    exit -1
fi

cd ${GTEST_DIR}

# check cmake-build-debug directory
CMAKE_DIR="cmake-build-debug"
if [ -d ${CMAKE_DIR} ]
then
    echo "Found directory ${CMAKE_DIR}. Will delete it."
    rm -rf ${CMAKE_DIR}
fi

mkdir ${CMAKE_DIR}
cd ${CMAKE_DIR}
echo "Created directory ${CMAKE_DIR}."

cmake ..
make