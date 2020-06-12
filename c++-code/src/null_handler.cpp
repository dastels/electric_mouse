// Serial logging handler (mainly for debug mode)
//
// Copyright (c) 2020 Dave Astels

#include "null_handler.h"


NullHandler::NullHandler(): LoggingHandler()
{
}


void NullHandler::emit(const char *level_name, const char *msg)
{
}
