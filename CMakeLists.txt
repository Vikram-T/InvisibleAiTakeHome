cmake_minimum_required(VERSION 2.8)
project( ChallengeInvis )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( ChallengeInvis ChallengeInvis.cpp )
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries( ChallengeInvis ${OpenCV_LIBS} Threads::Threads)


