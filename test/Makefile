CXX = g++
CXXFLAGS = -std=c++14 -Wall -Werror -I../src -I..
LDFLAGS =
BOOST_TEST_LOG_LEVEL = message
BOOST_TEST_COLOR_OUTPUT = yes
BOOST_TEST_REPORT_LEVEL = detailed

src = $(wildcard *.cpp)
obj = $(src:.cpp=)
ok  = $(src:.cpp=.ok)

.PHONY: test
test: $(ok)

build: $(obj)

$(ok): %.ok: %
	ZUSI2_STRECKEN_PFAD=/mnt/zusi/Zusi/Strecken/ ZUSI3_STRECKEN_PFAD=/mnt/zusi/Zusi3/OmegaDatenFull/Routes/ BOOST_TEST_REPORT_LEVEL=$(BOOST_TEST_REPORT_LEVEL) BOOST_TEST_LOG_LEVEL=$(BOOST_TEST_LOG_LEVEL) BOOST_TEST_COLOR_OUTPUT=$(BOOST_TEST_COLOR_OUTPUT) ./$(@:.ok=)
	touch $@

pfade_unix_test.ok: pfade_unix_test
	ZUSI3_DATAPATH=/mnt/zusi/Zusi3/Daten BOOST_TEST_REPORT_LEVEL=$(BOOST_TEST_REPORT_LEVEL) BOOST_TEST_LOG_LEVEL=$(BOOST_TEST_LOG_LEVEL) ./pfade_unix_test
	ZUSI3_DATAPATH=/mnt/zusi/Zusi3/Daten/ BOOST_TEST_REPORT_LEVEL=$(BOOST_TEST_REPORT_LEVEL) BOOST_TEST_LOG_LEVEL=$(BOOST_TEST_LOG_LEVEL) ./pfade_unix_test
	touch pfade_unix_test.ok

.PHONY: clean
clean:
	rm -f $(obj) $(ok)
