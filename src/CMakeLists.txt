set(lib_rasende_src
    Board.cpp
    Game.cpp
    Moves.cpp
    Robot.cpp
    )

add_library(LibRasende ${lib_rasende_src})

ADD_EXECUTABLE(Rasende rasende_roboter.cpp)
TARGET_LINK_LIBRARIES(Rasende
    LibRasende
    )
