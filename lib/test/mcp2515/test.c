// Unit tests for some functions.

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define TXRXBUF_HEADER_LEN 5

void expect_int32(char* name, uint32_t expected, uint32_t real) {
  if (expected == real)
    printf("%s: OK (%d)\n", name, expected);
  else 
    printf("%s: BAD - expected (%d) but got (%d)\n", name, expected, real);
}

void expect_bool(char* name, bool expected, bool real) {
  if (expected == real)
    printf("%s: OK (%s)\n", name, expected ? "true" : "false");
  else
    printf("%s: BAD - expected (%s) but got (%s)\n", name, expected ? "true" : "false", real ? "true" : "false");
}

void test_id() {
  printf("\nTEST_ID\n");

  const uint32_t SID = 234;
  const uint32_t EID = 130912;
  const uint32_t ID = (SID << 18) | (EID);

  uint32_t sid, eid, id;
  __split_unified_can_id(ID, &sid, &eid);
  __make_unified_can_id(SID, EID, &id);

  expect_int32("Split SID", SID, sid);
  expect_int32("Split EID", EID, eid);
  expect_int32("Merge ID", ID, id);
}

void test_txbuf_eid() {
  printf("\nTEST_TXBUF_EID\n");

  const uint32_t SID = 234;
  const uint32_t EID = 130912;
  const uint32_t ID = (SID << 18) | (EID);
  const uint8_t LEN = 4;

  uint8_t buf[TXRXBUF_HEADER_LEN];
  __mmr_mcp2515ll_make_txbuf_header(ID, false, true, LEN, buf);

  uint32_t id;
  bool isStandardId;
  bool rtr;
  uint8_t len;

  __mmr_mcp2515ll_parse_rxbuf_header(buf, &id, &isStandardId, &rtr, &len);

  expect_bool("Parsed isStandardId", false, isStandardId);
  expect_int32("Parsed len", LEN, len);

  uint32_t sid;
  uint32_t eid;
  __split_unified_can_id(id, &sid, &eid);

  expect_int32("SID", SID, sid);
  expect_int32("EID", EID, eid);
}

void test_txbuf_sid() {
  printf("\nTEST_TXBUF_SID\n");

  const uint32_t SID = 234;
  const uint8_t LEN = 4;

  uint8_t buf[TXRXBUF_HEADER_LEN];
  __mmr_mcp2515ll_make_txbuf_header(SID, true, true, LEN, buf);

  uint32_t id;
  bool isStandardId;
  bool rtr;
  uint8_t len;

  __mmr_mcp2515ll_parse_rxbuf_header(buf, &id, &isStandardId, &rtr, &len); // can't test RTR as it doesn't exist in TXBUF

  expect_int32("SID", SID, id);
  expect_bool("Parsed isStandardId", true, isStandardId);
  expect_int32("Parsed len", LEN, len);
}


int main() {
  test_id();
  test_txbuf_eid();
  test_txbuf_sid();
}