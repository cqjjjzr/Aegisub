find_package(GTest REQUIRED)
find_package(Boost REQUIRED chrono filesystem locale regex system thread)
find_package(ICU REQUIRED uc dt in)

target_include_directories(test-aegisub PRIVATE ${Boost_INCLUDE_DIRS} ${Iconv_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})
target_link_libraries(test-aegisub ${Boost_LIBRARIES} ${ICU_LIBRARIES} ${GTEST_LIBRARIES})
