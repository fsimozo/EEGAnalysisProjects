TEMPLATE      = lib
 CONFIG       += plugin
 INCLUDEPATH  += "../OpenFile"
 HEADERS       = HFO_detector1.h
 SOURCES       = HFO_detector1.cpp
 TARGET        = $$qtLibraryTarget(method_HFOdetection1)
 DESTDIR       = "../OpenFile/plugins/methods"