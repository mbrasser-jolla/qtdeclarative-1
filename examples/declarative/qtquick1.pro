TEMPLATE = subdirs

# These examples contain some C++ and need to be built
SUBDIRS = \
   cppextensions \
   modelviews \
   tutorials

# OpenGL shader examples requires opengl and they contain some C++ and need to be built
contains(QT_CONFIG, opengl): SUBDIRS += shadereffects

# These examples contain no C++ and can simply be copied
sources.files = \
   animation \
   cppextensions \
   i18n \
   imageelements \
   keyinteraction \
   positioners \
   sqllocalstorage \
   text \
   threading \
   touchinteraction \
   toys \
   ui-components \
   xml

sources.path = $$[QT_INSTALL_EXAMPLES]/declarative
INSTALLS += sources
