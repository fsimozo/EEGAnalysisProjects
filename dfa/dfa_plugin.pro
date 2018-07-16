TEMPLATE      = lib
 CONFIG       += plugin
 INCLUDEPATH  += "../eeg_analysis"
 HEADERS       = dfa_plugin.h simulatedHfoGenerator.h dfaAnalysis.h methodResult.h resultElement.h FIRfilter.h writeMarkersText.h
 SOURCES       = dfa_plugin.cpp simulatedHfoGenerator.cpp dfaAnalysis.cpp methodResult.cpp resultElement.cpp FIRfilter.cpp writeMarkersText.cpp
 TARGET        = $$qtLibraryTarget(method_dfa)
 DESTDIR       = "../eeg_analysis/plugins/methods"