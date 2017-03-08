CXX = g++
CXXFLAGS = -std=c++14 -Wall -Werror -I../src -I.. -g
LDFLAGS =
BOOST_TEST_LOG_LEVEL = message
BOOST_TEST_COLOR_OUTPUT = yes
BOOST_TEST_REPORT_LEVEL = detailed
BOOST_PARAMS = BOOST_TEST_REPORT_LEVEL=$(BOOST_TEST_REPORT_LEVEL) BOOST_TEST_LOG_LEVEL=$(BOOST_TEST_LOG_LEVEL) BOOST_TEST_COLOR_OUTPUT=$(BOOST_TEST_COLOR_OUTPUT)

src = $(wildcard *.cpp)
obj = $(src:.cpp=)
ok  = $(src:.cpp=.ok)

.PHONY: test
test: $(ok)

.PHONY: build
build: $(obj)

zusi_offizielle_strecken_test: zusi_offizielle_strecken_test.cpp
	$(CXX) $(CXXFLAGS) -O3 -flto -o $@ $^

callgrind: zusi_offizielle_strecken_test
	ZUSI2_STRECKEN_PFAD=/mnt/zusi/Zusi/Strecken/ ZUSI3_STRECKEN_PFAD=/mnt/zusi/Zusi3/OmegaDatenFull/Routes/ $(BOOST_PARAMS) valgrind --tool=callgrind ./zusi_offizielle_strecken_test

$(ok): %.ok: %
	ZUSI2_STRECKEN_PFAD=/mnt/zusi/Zusi/Strecken/ ZUSI3_STRECKEN_PFAD=/mnt/zusi/Zusi3/OmegaDatenFull/Routes/ ZUSI3_DATAPATH=./eingabe/zusi3 $(BOOST_PARAMS) $(GDB_CMD) ./$(@:.ok=)
	touch $@

pfade_unix_test.ok: pfade_unix_test
	ZUSI3_DATAPATH=/mnt/zusi/Zusi3/Daten $(BOOST_PARAMS) $(GDB_CMD) ./pfade_unix_test
	ZUSI3_DATAPATH=/mnt/zusi/Zusi3/Daten/ $(BOOST_PARAMS) $(GDB_CMD) ./pfade_unix_test
	touch pfade_unix_test.ok

.PHONY: clean
clean:
	rm -f $(obj) $(ok)
