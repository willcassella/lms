
add_library(lmssom STATIC
	impl/DataNormalizer.cpp
	impl/Network.cpp
	)

target_include_directories(lmssom INTERFACE
	include
	)

target_include_directories(lmssom PRIVATE
	include
	)

target_link_libraries(lmssom PUBLIC
	lmsutils
	)

set_property(TARGET lmssom PROPERTY POSITION_INDEPENDENT_CODE ON)

