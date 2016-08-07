cmake_minimum_required (VERSION 3.0)

project ( dcp2p )

find_package( Boost COMPONENTS thread system date_time regex asio array signals )

set( EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build" )
set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build" )

set( ROOT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set  ( ROOT_HEADERS      
       "include/p2p_host.hpp"
       "include/p2p_listener.hpp"
       "include/p2p_connection.hpp"
       "include/p2p_manager.hpp"
       "include/p2p_packet.hpp"
     )

set  ( ROOT_SRC
       "src/p2p_host.cpp"
       "src/p2p_listener.cpp"
       "src/p2p_connection.cpp"
       "src/p2p_manager.cpp"
       "src/p2p_packet.cpp"
     )

source_group( "include" FILES ${ROOT_HEADERS} )
source_group( "src" FILES ${ROOT_SRC} )

include_directories( ${ROOT_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} )

link_directories( 
	${Boost_LIBRARY_DIRS}
)

ADD_LIBRARY( dcp2p STATIC ${ROOT_HEADERS} ${ROOT_SRC} )

SET_TARGET_PROPERTIES( dcp2p PROPERTIES LIBRARY_OUTPUT_NAME dcp2p ARCHIVE_OUTPUT_NAME dcp2p )

if ( MSVC )
	ADD_DEFINITIONS( 
		-D_WIN32_WINNT=0x0501
	)
    set_target_properties( dcp2p PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/libs)
    set_target_properties( dcp2p PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/libs)
    set_target_properties( dcp2p PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/libs)
endif ( MSVC )