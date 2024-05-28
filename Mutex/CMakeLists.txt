
add_executable(mutex
				main.cpp
				Mutex.h
				Mutex.cpp
)

add_executable(mutex_tests TestMutex/test.cpp)

target_link_libraries(mutex_tests 				 
				GTest::GTest						
)
add_test(mutex_tests mutex_tests)

