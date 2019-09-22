message(STATUS "Aegisub Build: Loading dependencies using Conan...")
include(cmake/conan.cmake)

conan_add_remote(NAME bincrafters URL https://api.bintray.com/conan/bincrafters/public-conan)
conan_add_remote(NAME charliejiang URL https://api.bintray.com/conan/charliejiang/conan)
conan_add_remote(NAME h4mster URL https://api.bintray.com/conan/h4mster/conan)

conan_cmake_run(CONANFILE dependencies/conanfile.py
                BASIC_SETUP
                BUILD missing)

message(STATUS "Aegisub Build: External dependencies loaded.")