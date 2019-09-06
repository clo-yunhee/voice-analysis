TARGET = lpcAnalyser.js

DEPS = bindings.cc LPCAnalyser.cc 

build: $(DEPS)
	@emcc --bind \
	    -I/usr/local/include/eigen3 \
	    -I../lib \
        -std=c++17 \
        -s SINGLE_FILE=1 \
        -s MODULARIZE_INSTANCE=1 \
        -s EXPORT_NAME='AnalyserModule' \
        -s WASM=1 \
		-s BINARYEN_ASYNC_COMPILATION=0 \
		-s ASSERTIONS=0 \
		-O0 \
		-g4 --source-map-base --export-all \
		$(DEPS) \
		-o ../../build/$(TARGET) \
		--pre-js ../lib/wasm-audio-helper.js \
		--post-js ./processor.js

clean:
	@rm -f ../../build/$(TARGET)
