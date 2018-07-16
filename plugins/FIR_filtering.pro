TEMPLATE      = lib
 CONFIG       += plugin
 INCLUDEPATH  += "../OpenFile"
 HEADERS       = FIR_filtering.h
 SOURCES       = FIR_filtering.cpp
 TARGET        = $$qtLibraryTarget(method_FIRfiltering)
 DESTDIR       = "../OpenFile/plugins/methods"