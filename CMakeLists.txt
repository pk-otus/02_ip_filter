cmake_minimum_required(VERSION 3.2)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER})
   set(PROJECT_VERSION_PATCH $ENV{TRAVIS_BUILD_NUMBER})
else()
   set(PROJECT_VERSION_PATCH 1)
endif()

project(ip_filter VERSION 0.0.${PROJECT_VERSION_PATCH})

configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

add_executable(ip_filter ip_filter.cpp)

if (MSVC)
	set_target_properties(ip_filter PROPERTIES
		CXX_STANDARD 14
		CXX_STANDARD_REQUIRED ON
		COMPILE_OPTIONS -W3
		)
else()
	set_target_properties(ip_filter PROPERTIES
		CXX_STANDARD 14
		CXX_STANDARD_REQUIRED ON
		COMPILE_OPTIONS -Wpedantic -Wall -Wextra
		)
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)
install(FILES ip.tsv DESTINATION bin)
install(FILES out.tsv DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT vvz.otus@gmail.com)

include (CPack)

