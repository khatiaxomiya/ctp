file(GLOB_RECURSE VOTCA_MOO_SHARE INDOParameters.xml)
install(FILES ${VOTCA_MOO_SHARE} DESTINATION ${CMAKE_INSTALL_DATADIR}/votca/moo/xml)
