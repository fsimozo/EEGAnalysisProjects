TEMPLATE      = lib
 CONFIG       += plugin
 INCLUDEPATH  += "../OpenFile"
 HEADERS       = mirroring.h
 SOURCES       = mirroring.cpp
 TARGET        = $$qtLibraryTarget(method_mirror)
 DESTDIR       = "../OpenFile/plugins/methods"
