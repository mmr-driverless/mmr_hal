#include "__inc/__mcp2515_errmgmt.h"

static MmrMcp2515Error __error = MMR_MCP2515_ERROR_OK;

void __mmr_mcp2515_set_error(MmrMcp2515Error error) {
  __error = error;
}

/**
 * @returns Returns the last error, which is updated when a function of the MCP2515 library returns false. (It is never reset to OK!)
*/
MmrMcp2515Error MMR_MCP2515_GetLastError() {
  return __error;
}