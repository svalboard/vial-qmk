// Functions that pertain to the QMK Settings tab in vial.
#include "svalboard.h"

#define STATIC_ASSERT(x) do { typedef char Assert[(x) > 0? 1: -1] __attribute__((unused)); } while (0)

/**
 * QS ids. You need a matching;
 * https://github.com/vial-kb/vial-gui/blob/main/src/main/resources/base/qmk_settings.json
 * We don't use the "bit" value. Just one value per setting.
 */
#define SVALBOARD_BASE_QSID 5000 // uint16_t. We use a decimal value for readability in the json.

// switch is optimized for contiguous cases, use contiguous values if possible.
enum QsidMap {
  SVAL_QSID_ACHORDION_MODE = 0,
  SVAL_QSID_NUM_VALUES = 1,  // Update this to the highest index if you add some value.
};

#define SVAL_SZ_QSID_ACHORDION_MODE 1

/**
 * Called after eepom_settings_load, this just sets up the pointers and calls notify
 * if required. It invoked at the end of qmk_settings_reset.
 */
void qmk_settings_init_user(void) {
  // Nothing to do: Already done with read_eeprom.
}

/**
 * Reset to default values.
 * Called before eeprom_save and clear_keyboard.
 */
void qmk_settings_reset_user(void) {
}

/**
 * If we find a qsid that's greater that qsdig_gt.
 *
 * It is called by the vial command: vial_qmk_settings_query. It is called by:
 * vial-gui/src/main/python/protocol/keyboard_comm.py
 * to introspect what settings are available.
 *
 * buffer: the output buffer than qsid_gt, push the value in the buffer.
 * sz: available buffer len for output.
 */
void qmk_settings_query_user(uint16_t qsid_gt, void *buffer, size_t sz) {
  char* cbuf = (char*)buffer;
  const char* end = cbuf + sz;
  for (uint16_t qsid = qsid_gt; qsid < SVALBOARD_BASE_QSID + SVAL_QSID_NUM_VALUES; ++qsid) {
    char *next = buffer + sizeof(qsid);
    if (next > end) {
      // Buffer is full.
      return;
    }
    (void)memcpy(cbuf, &qsid, sizeof(qsid));
    cbuf = next;
  }
}

/**
 * Returns 0 on success? -1 if not found or failure.
 */
int qmk_settings_get_user(uint16_t qsid, void *setting, size_t maxsz) {
  if (qsid < SVALBOARD_BASE_QSID)
    return -1;
  switch ((enum QsidMap)(qsid - SVALBOARD_BASE_QSID)) {
    case SVAL_QSID_ACHORDION_MODE: {
      const char val = global_saved_values.disable_achordion;
      STATIC_ASSERT(sizeof(val) == SVAL_SZ_QSID_ACHORDION_MODE);
      if (maxsz < SVAL_SZ_QSID_ACHORDION_MODE)
        return -1;
      (void)memcpy(setting, &val, SVAL_SZ_QSID_ACHORDION_MODE);
      return 0;
    }
    default:
      return -1;
  }
}

/**
 * Sets the value at the qsid slot, and invokes any notification callbacks.
 * Returns -1 on error or noop, otherwise 0.
 */
int qmk_settings_set_notify_user(uint16_t qsid, const void *setting, size_t maxsz) {
  if (qsid < SVALBOARD_BASE_QSID)
    return -1;
  switch ((enum QsidMap)(qsid - SVALBOARD_BASE_QSID)) {
    case SVAL_QSID_ACHORDION_MODE: {
      char val;
      STATIC_ASSERT(sizeof(val) == SVAL_SZ_QSID_ACHORDION_MODE);
      if (maxsz < SVAL_SZ_QSID_ACHORDION_MODE)
        return -1;
      (void)memcpy(&val, setting, SVAL_SZ_QSID_ACHORDION_MODE);
      global_saved_values.disable_achordion = val;
      return 0;
    }
    default:
      return -1;
  }
}
