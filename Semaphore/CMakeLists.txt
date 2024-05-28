
add_library(semaphore_lib
				Semaphore.h
				Semaphore.cpp				
				BufferedChannel.h
)

target_include_directories(semaphore_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(semaphore_tests TestSemaphore/test.cpp)

target_link_libraries(semaphore_tests PRIVATE
				GTest::GTest
				semaphore_lib 								
)

add_test(semaphore_tests semaphore_tests)

