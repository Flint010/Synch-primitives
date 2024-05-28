
add_executable(condVar
				main.cpp
				ConditionVariable.h
				ConditionVariable.cpp			
)


add_executable(condVar_tests TestCondVar/test.cpp)

target_link_libraries(condVar_tests 
				GTest::GTest						
)

add_test(condVar_tests condVar_tests)

