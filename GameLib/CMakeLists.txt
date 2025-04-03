project(GameLib)

set(SOURCE_FILES
        pch.h
        MainFrame.cpp
        MainFrame.h
        GameView.cpp
        GameView.h
        Game.cpp
        Game.h
        Item.cpp
        Item.h
        ids.h
        Conveyor.cpp
        Conveyor.h
        Product.cpp
        Product.h
        Sparty.cpp
        Sparty.h
        Beam.cpp
        Beam.h
        Sensor.cpp
        Sensor.h
        Pins.cpp
        Pins.h
        InputPin.cpp
        InputPin.h
        OutputPin.cpp
        OutputPin.h
        Scoreboard.cpp
        Scoreboard.h
        Gates.cpp
        Gates.h
        AndGate.cpp
        AndGate.h
        OrGate.cpp
        OrGate.h
        NotGate.cpp
        NotGate.h
        SrFlipFlopGate.cpp
        SrFlipFlopGate.h
        DFlipFlopGate.cpp
        DFlipFlopGate.h
        ItemVisitor.cpp
        ItemVisitor.h
        Score.cpp
        Score.h
        Timer.cpp
        Timer.h
        LevelLoader.cpp
        LevelLoader.h
        IDraggable.h
        SensorPanel.cpp
        SensorPanel.h
        ProductVisitors.cpp
        ProductVisitors.h
)

set(wxBUILD_PRECOMP OFF)
find_package(wxWidgets COMPONENTS core base xrc html xml REQUIRED)
include(${wxWidgets_USE_FILE})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)