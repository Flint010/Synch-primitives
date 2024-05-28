
add_executable(cyclicBarrier
				main.cpp
				CyclicBarrier.h
				CyclicBarrier.cpp
)

add_executable(barrier_tests TestBarrier/test.cpp)

target_link_libraries(barrier_tests 				 
				GTest::GTest						
)
add_test(barrier_tests barrier_tests)

