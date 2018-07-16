TEMPLATE      = lib
 CONFIG       += plugin
 INCLUDEPATH  += "../OpenFile"
 HEADERS       = HFO_detector.h
 SOURCES       = HFO_detector.cpp
 TARGET        = $$qtLibraryTarget(method_HFOdetection)
 DESTDIR       = "../OpenFile/plugins/methods"
