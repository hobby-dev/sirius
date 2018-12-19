#include <iostream>
#include <cstring>

#include "catch.hpp"

#include "first.hpp"
#include "second.hpp"
#include "third.hpp"

using namespace sirius;

TEST_CASE("Binary representations calculated correctly", "[first]") {
  char buf[BUF_SIZE];
  SECTION("1") {
    ConvertToBinStr(1, buf);
    REQUIRE(!strcmp(buf, "0000000000000000000000000000000000000000000000000000000000000001"));
  };
  SECTION("-1") {
    ConvertToBinStr(-1, buf);
    REQUIRE(!strcmp(buf, "1111111111111111111111111111111111111111111111111111111111111111"));
  };
  SECTION("0") {
    ConvertToBinStr(0, buf);
    REQUIRE(!strcmp(buf, "0000000000000000000000000000000000000000000000000000000000000000"));
  };
}

TEST_CASE("ConvertToBinStr(100500, nullptr) does not crash)", "[first]") {
  char *buf = nullptr;
  REQUIRE_NOTHROW(ConvertToBinStr(100500, buf));
  REQUIRE(buf == nullptr);
};

TEST_CASE("Consecutive duplicates removed", "[second]") {
  char buf[BUF_SIZE];
  SECTION("\"AAA BBB CCC\" to \"A B C\"") {
    strcpy(buf, "AAA BBB CCC");
    RemoveDups(buf);
    REQUIRE(!strcmp(buf, "A B C"));
  };
  SECTION("\"AAA\" to \"A\"") {
    strcpy(buf, "AAA");
    RemoveDups(buf);
    REQUIRE(!strcmp(buf, "A"));
  };
}

TEST_CASE("RemoveDups(nullptr) does not crash", "[second]") {
  char *myPtr = nullptr;
  REQUIRE_NOTHROW(RemoveDups(myPtr));
  REQUIRE(myPtr == nullptr);
}

SCENARIO("List can be created and Serialized", "[third]") {
  GIVEN("A list with some items") {
    List list;
    list.PushBack(std::string("The first virtue is curiosity"));
    list.PushBack(std::string("The second virtue is relinquishment"));
    REQUIRE(list.Size() == 2);
    AND_GIVEN("A file opened with fopen(\"write_only_list.bin\", \"wb\"))") {
      FILE *file = fopen("write_only_list.bin", "wb");
      REQUIRE(file != nullptr);
      WHEN("Serialization to file requested") {
        list.Serialize(file);
        AND_WHEN("Another item added") {
          list.PushBack(std::string("The third virtue is lightness"));
          THEN("List can still be Serialize-d") {
            list.Serialize(file);
          }
        }
      }
      fflush(file);
      fclose(file);
    }
  }
}

SCENARIO("List can be Deserialized", "[third]") {
  GIVEN("A list with some items") {
    List list;
    list.PushBack(std::string("The first virtue is curiosity"));
    list.PushBack(std::string("The second virtue is relinquishment"));
    list.PushBack(std::string("The third virtue is lightness"));
    list.PushBack(std::string("The fourth virtue is evenness"));

    REQUIRE(list.Size() == 4);
    AND_GIVEN("A file opened with fopen(\"write_read_list.bin\", \"wb\"))") {
      FILE *writeFile = fopen("write_read_list.bin", "wb");
      REQUIRE(writeFile != nullptr);
      AND_GIVEN("List completely serialized and written to file") {
        list.Serialize(writeFile);
        fflush(writeFile);
        fclose(writeFile);
        AND_GIVEN("A file opened with fopen(\"write_read_list.bin\", \"rb\"))") {
          FILE *readFile = fopen("write_read_list.bin", "rb");
          REQUIRE(readFile != nullptr);
          WHEN("Deserialization from file to the same instance of List requested") {
            list.Deserialize(readFile);
            THEN("List has correct size") {
              REQUIRE(list.Size() == 4);
            }
          }

          AND_GIVEN("Clean instance of list") {
            List cleanList;
            WHEN("Deserialization from file to the clean instance of List requested") {
              cleanList.Deserialize(readFile);

              THEN("List has correct size") {
                REQUIRE(cleanList.Size() == 4);
              }

              THEN("List item's strings data is deserialized correctly") {
                REQUIRE(cleanList.accessNode(0)->data == "The first virtue is curiosity");
                REQUIRE(cleanList.accessNode(1)->data == "The second virtue is relinquishment");
                REQUIRE(cleanList.accessNode(2)->data == "The third virtue is lightness");
                REQUIRE(cleanList.accessNode(3)->data == "The fourth virtue is evenness");
              }

              THEN("Don't crash when trying to access incorrect index") {
                REQUIRE_NOTHROW(cleanList.accessNode(42) == nullptr);
              }
            }
          }
        }
      }
    }
  }
}

SCENARIO("Random pointers in List Serialized and Deserialized correctly", "[third]") {
  GIVEN("A list with some items pointing to each other") {
    List list;
    list.PushBack(std::string("0th"));
    list.PushBack(std::string("1st"));
    list.PushBack(std::string("2nd"));
    list.PushBack(std::string("3rd"));
    list.PushBack(std::string("4th"));
    list.PushBack(std::string("5th"));
    list.PushBack(std::string("6th"));
    list.PushBack(std::string("7th"));
    list.PushBack(std::string("8th"));
    list.PushBack(std::string("9th"));
    list.PushBack(std::string("10th"));
    list.PushBack(std::string("11th"));
    list.PushBack(std::string("12th"));
    list.PushBack(std::string("13th"));
    list.PushBack(std::string("14th"));

    list.accessNode(7)->rand = list.accessNode(12);
    list.accessNode(13)->rand = list.accessNode(0);
    list.accessNode(14)->rand = list.accessNode(14);
    list.accessNode(0)->rand = list.accessNode(14);

    REQUIRE(list.Size() == 15);
    AND_GIVEN("A file opened with fopen(\"write_read_list.bin\", \"wb\"))") {
      FILE *writeFile = fopen("write_read_list.bin", "wb");
      REQUIRE(writeFile != nullptr);
      AND_GIVEN("List completely serialized and written to file") {
        list.Serialize(writeFile);
        fflush(writeFile);
        fclose(writeFile);
        AND_GIVEN("A file opened with fopen(\"write_read_list.bin\", \"rb\"))") {
          FILE *readFile = fopen("write_read_list.bin", "rb");
          REQUIRE(readFile != nullptr);
          AND_GIVEN("Clean instance of list") {
            List cleanList;
            WHEN("Deserialization from file to the same instance of List requested") {
              cleanList.Deserialize(readFile);
              THEN("List has correct size") {
                REQUIRE(cleanList.Size() == 15);
              }THEN("Random pointers are pointing correctly") {
                REQUIRE(cleanList.accessNode(7)->rand == cleanList.accessNode(12));
                REQUIRE(cleanList.accessNode(13)->rand == cleanList.accessNode(0));
                REQUIRE(cleanList.accessNode(14)->rand == cleanList.accessNode(14));
                REQUIRE(cleanList.accessNode(0)->rand == cleanList.accessNode(14));
              }
            }
          }
        }
      }
    }
  }
}

SCENARIO("Empty List is Serialized and Deserialized correctly", "[third]") {
  GIVEN("An empty list") {
    List list;

    REQUIRE(list.Size() == 0);
    AND_GIVEN("A file opened with fopen(\"write_read_list.bin\", \"wb\"))") {
      FILE *writeFile = fopen("write_read_list.bin", "wb");
      REQUIRE(writeFile != nullptr);
      AND_GIVEN("List completely serialized and written to file") {
        list.Serialize(writeFile);
        fflush(writeFile);
        fclose(writeFile);
        AND_GIVEN("A file opened with fopen(\"write_read_list.bin\", \"rb\"))") {
          FILE *readFile = fopen("write_read_list.bin", "rb");
          REQUIRE(readFile != nullptr);
          AND_GIVEN("Clean instance of list") {
            List cleanList;
            WHEN("Deserialization from file to the same instance of List requested") {
              cleanList.Deserialize(readFile);
              THEN("List has correct size") {
                REQUIRE(cleanList.Size() == 0);
              }
            }
          }
        }
      }
    }
  }
}
