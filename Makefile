BUILD := debug
BUILD_DIR := build/${BUILD}/
.DEFAULT_GOAL := ${BUILD_DIR}checksums

cxxflags.common :=  -I. `pkg-config --cflags gtkmm-3.0 libcurl libsecret-1` -std=c++17
cxxflags.debug := -g -O0
cxxflags.release := -O2
cxxflags.sanitize_address:= -g -O0 -fsanitize=address
cxxflags.sanitize_thread:= -g -O0 -fsanitize=thread
CXXFLAGS := ${cxxflags.${BUILD}} ${cxxflags.common}

ldflags.common :=
ldflags.debug :=
ldflags.release :=
ldflags.sanitize_address := -fsanitize=address
ldflags.sanitize_thread := -fsanitize=thread
LDFLAGS := ${ldflags.${BUILD}} ${ldflags.common}

VPATH=$(BUILD_DIR)third_party/json_parser/
CC=g++

_OBJ := token_window.o main_window.o utils.o resources.o api.o third_party/json_parser/json_parser.o
_TEST_OBJ := utils_unittest.o api_unittest.o
OBJ := ${patsubst %,${BUILD_DIR}%,${_OBJ}}
TEST_OBJ := ${patsubst %,${BUILD_DIR}%,${_TEST_OBJ}}
$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(BUILD_DIR)third_party/json_parser/)

${BUILD_DIR}third_party/json_parser/%.o: %.cc
	$(CC) -c -o $@ $< $(CXXFLAGS)

${BUILD_DIR}%.o: %.cc
	$(CC) -c -o $@ $< $(CXXFLAGS)

${BUILD_DIR}checksums: $(OBJ)
	$(CC) ${LDFLAGS} -o ${BUILD_DIR}checksums shasums.cc $^ `pkg-config --cflags --libs gtkmm-3.0 libcurl libcrypto libsecret-1`

${BUILD_DIR}run_tests: $(OBJ) $(TEST_OBJ)
	$(CC) ${LDFLAGS} -o ${BUILD_DIR}run_tests run_tests.cc /usr/lib/x86_64-linux-gnu/libgtest.a $^ `pkg-config --cflags --libs gtkmm-3.0 libcurl libcrypto libsecret-1`

.PHONY: tests
tests: ${BUILD_DIR}run_tests

install:
	mkdir -p $(DESTDIR)/bin
	cp ${BUILD_DIR}checksums $(DESTDIR)/bin/

.PHONY: clean
clean:
	echo "Removing build files"
	rm ${BUILD_DIR}*.o 
	rm ${BUILD_DIR}third_party/json_parser/*.o
	rm ${BUILD_DIR}checksums
