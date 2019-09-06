#include <emscripten/bind.h>
#include "LPCAnalyser.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(lpcAnalyser) {
    class_<LPCAnalyser>("LPCAnalyser")
        .constructor()
        .function("getData", &LPCAnalyser::getData)
        .function("process", &LPCAnalyser::process, allow_raw_pointers());
}