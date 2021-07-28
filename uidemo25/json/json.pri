HEADERS += \
    $$PWD/autolink.h \
    $$PWD/config.h \
    $$PWD/features.h \
    $$PWD/forwards.h \
    $$PWD/json.h \
    $$PWD/reader.h \
    $$PWD/value.h \
    $$PWD/writer.h \
    $$PWD/json_src/json_batchallocator.h

DISTFILES += \
    $$PWD/json_src/sconscript

SOURCES += \
    $$PWD/json_src/json_internalarray.inl \
    $$PWD/json_src/json_internalmap.inl \
    $$PWD/json_src/json_reader.cpp \
    $$PWD/json_src/json_value.cpp \
    $$PWD/json_src/json_valueiterator.inl \
    $$PWD/json_src/json_writer.cpp
