cmake_minimum_required(VERSION 3.21.3)

set(SOURCES_COMMON
    ti_log_config.c
    tracer_control.c
)

set(SOURCES_CC23X0R5
    ${SOURCES_COMMON}
)

set(SOURCES_CC23X0R2
    ${SOURCES_COMMON}
)
