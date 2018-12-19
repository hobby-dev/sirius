# add sources
set(SIRIUS_SOURCES
        # implementations:
        first.hpp
        first.cpp
        second.hpp
        second.cpp
        third.hpp
        third.cpp
        # testing:
        cfg.cpp
        catch.hpp
        main.cpp)

# add main executable
add_executable(sirius ${SIRIUS_SOURCES})
