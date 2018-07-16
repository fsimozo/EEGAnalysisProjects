TEMPLATE      = lib
 CONFIG       += plugin
 INCLUDEPATH  += "../eeg_analysis"
 HEADERS       = hfoDetector.h methodResult.h resultElement.h FIRfilter.h writeMarkersText.h
 SOURCES       = hfoDetector.cpp methodResult.cpp resultElement.cpp FIRfilter.cpp writeMarkersText.cpp
 TARGET        = $$qtLibraryTarget(method_hfoDetector)
 DESTDIR       = "../eeg_analysis/plugins/methods"