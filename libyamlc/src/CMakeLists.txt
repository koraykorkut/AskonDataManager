cmake_minimum_required (VERSION 3.4.1)
project (libyamlc)
add_library(libyamlc api.c dumper.c emitter.c loader.c parser.c reader.c scanner.c writer.c yaml_private.h)
