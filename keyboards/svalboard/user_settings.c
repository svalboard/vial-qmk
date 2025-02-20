// Functions that pertain to the QMK Settings tab in vial.
// See doc/qmk_user_settings.md.
#include "svalboard.h"

#define STATIC_ASSERT(x) do { typedef char Assert[(x) > 0? 1: -1] __attribute__((unused)); } while (0)

/**
 * QS ids. You need a matching;
 * https://github.com/vial-kb/vial-gui/blob/main/src/main/resources/base/qmk_settings.json
 */
#define SVALBOARD_BASE_QSID 500  // uint16_t. We use a decimal value for readability in the json.

// switch is optimized for contiguous cases, use contiguous values if possible.
enum QsidMap {
  SVAL_QSID_ACHORDION_MODE = 0,
  SVAL_QSID_AUTO_MOUSE = 1,
  SVAL_QSID_MOUSE_SCROLLS = 2,  // Bit 0: left, bit 1: right
  SVAL_QSID_MH_TIMER_INDEX = 3,
  SVAL_QSID_MH_TIMER_SLOT0_MS = 4,
  SVAL_QSID_MH_TIMER_SLOT1_MS = 5,
  SVAL_QSID_MH_TIMER_SLOT2_MS = 6,
  SVAL_QSID_MH_TIMER_SLOT3_MS = 7,
  SVAL_QSID_NUM_VALUES = 8,  // Update this to the highest index if you add some entry.
};

// Size in bytes.
#define SVAL_SZ_QSID_ACHORDION_MODE 1
#define SVAL_SZ_QSID_AUTO_MOUSE 1
#define SVAL_SZ_QSID_MOUSE_SCROLLS 1
#define SVAL_SZ_QSID_MH_TIMER_INDEX 1
#define SVAL_SZ_QSID_MH_TIMER_SLOT_VALUES 2

/**
 * Called after eepom_settings_load, this just sets up the pointers and calls notify
 * if required. It is invoked at the end of qmk_settings_reset.
 */
void qmk_settings_init_user(void) {
  // This is where we would call notify callbacks that need to be called when
  // global_saved_values changes.
}

/**
 * Reset to default values.
 * Called before clear_keyboard.
 */
void qmk_settings_reset_user(void) {
  global_saved_values.auto_mouse = SVAL_SETTINGS_DEFAULT_AUTO_MOUSE;
  global_saved_values.disable_achordion = SVAL_SETTINGS_DEFAULT_DISABLE_ACHORDION;
  global_saved_values.mh_timer_index = SVAL_SETTINGS_DEFAULT_MH_TIMER_INDEX;
  (void)memcpy(global_saved_values.mh_timer_choices,
               sval_settings_default_mh_timer_choices,
               sizeof(global_saved_values.mh_timer_choices));
  // We just have 4 slots hard-wired in the enum.
  STATIC_ASSERT(sizeof(global_saved_values.mh_timer_choices) / sizeof(uint16_t) == 4);
  write_eeprom_kb();
}

/**
 * If we find a qsid that's greater that qsdig_gt that we can handle, return
 * it. This is where we can deprecate QSIDs.
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
 * Returns 0 on success and -1 if not found or failure.
 */
int qmk_settings_get_user(uint16_t qsid, void *setting, size_t maxsz) {
  if (qsid < SVALBOARD_BASE_QSID)
    return -1;
  switch ((enum QsidMap)(qsid - SVALBOARD_BASE_QSID)) {
    case SVAL_QSID_ACHORDION_MODE: {
      const char cur_is_on = global_saved_values.disable_achordion == 0;
      STATIC_ASSERT(sizeof(cur_is_on) == SVAL_SZ_QSID_ACHORDION_MODE);
      if (maxsz < SVAL_SZ_QSID_ACHORDION_MODE)
        return -1;
      (void)memcpy(setting, &cur_is_on, SVAL_SZ_QSID_ACHORDION_MODE);
      return 0;
    }
    case SVAL_QSID_AUTO_MOUSE: {
      const char cur_is_on = global_saved_values.auto_mouse;
      STATIC_ASSERT(sizeof(cur_is_on) == SVAL_SZ_QSID_AUTO_MOUSE);
      if (maxsz < SVAL_SZ_QSID_AUTO_MOUSE)
        return -1;
      (void)memcpy(setting, &cur_is_on, SVAL_SZ_QSID_AUTO_MOUSE);
      return 0;
    }
    case SVAL_QSID_MOUSE_SCROLLS: {
      const unsigned char cur = global_saved_values.left_scroll | (global_saved_values.right_scroll << 1);
      STATIC_ASSERT(sizeof(cur) == SVAL_SZ_QSID_MOUSE_SCROLLS);
      if (maxsz < SVAL_SZ_QSID_MOUSE_SCROLLS)
        return -1;
      (void)memcpy(setting, &cur, SVAL_SZ_QSID_MOUSE_SCROLLS);
      return 0;
    }
    case SVAL_QSID_MH_TIMER_INDEX: {
      const unsigned char cur = global_saved_values.mh_timer_index;
      STATIC_ASSERT(sizeof(cur) == SVAL_SZ_QSID_MH_TIMER_INDEX);
      if (maxsz < SVAL_SZ_QSID_MH_TIMER_INDEX)
        return -1;
      (void)memcpy(setting, &cur, SVAL_SZ_QSID_MH_TIMER_INDEX);
      return 0;
    }
    case SVAL_QSID_MH_TIMER_SLOT0_MS:
    case SVAL_QSID_MH_TIMER_SLOT1_MS:
    case SVAL_QSID_MH_TIMER_SLOT2_MS:
    case SVAL_QSID_MH_TIMER_SLOT3_MS: {
      const int idx = qsid - SVALBOARD_BASE_QSID - SVAL_QSID_MH_TIMER_SLOT0_MS;
      const int16_t cur = global_saved_values.mh_timer_choices[idx];
      STATIC_ASSERT(sizeof(cur) == SVAL_SZ_QSID_MH_TIMER_SLOT_VALUES);
      if (maxsz < SVAL_SZ_QSID_MH_TIMER_SLOT_VALUES)
        return -1;
      (void)memcpy(setting, &cur, SVAL_SZ_QSID_MH_TIMER_SLOT_VALUES);
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
  int ret = -1;
  switch ((enum QsidMap)(qsid - SVALBOARD_BASE_QSID)) {
    case SVAL_QSID_ACHORDION_MODE: {
      char want_on;
      STATIC_ASSERT(sizeof(want_on) == SVAL_SZ_QSID_ACHORDION_MODE);
      if (maxsz < SVAL_SZ_QSID_ACHORDION_MODE)
        return -1;
      (void)memcpy(&want_on, setting, SVAL_SZ_QSID_ACHORDION_MODE);
      global_saved_values.disable_achordion = !want_on;
      ret = 0;
    } break;
    case SVAL_QSID_AUTO_MOUSE: {
      char want_on;
      STATIC_ASSERT(sizeof(want_on) == SVAL_SZ_QSID_AUTO_MOUSE);
      if (maxsz < SVAL_SZ_QSID_AUTO_MOUSE)
        return -1;
      (void)memcpy(&want_on, setting, SVAL_SZ_QSID_AUTO_MOUSE);
      global_saved_values.auto_mouse = want_on;
      ret = 0;
    } break;
    case SVAL_QSID_MOUSE_SCROLLS: {
      unsigned char wanted;
      STATIC_ASSERT(sizeof(wanted) == SVAL_SZ_QSID_MOUSE_SCROLLS);
      if (maxsz < SVAL_SZ_QSID_MOUSE_SCROLLS)
        return -1;
      (void)memcpy(&wanted, setting, SVAL_SZ_QSID_MOUSE_SCROLLS);
      const bool left = wanted & 0x1;
      const bool right = (wanted >> 1) & 0x01;
      global_saved_values.left_scroll = left;
      global_saved_values.right_scroll = right;
      ret = 0;
    } break;
    case SVAL_QSID_MH_TIMER_INDEX: {
      uint8_t wanted;
      STATIC_ASSERT(sizeof(wanted) == SVAL_SZ_QSID_MH_TIMER_INDEX);
      if (maxsz < SVAL_SZ_QSID_MH_TIMER_INDEX)
        return -1;
      (void)memcpy(&wanted, setting, SVAL_SZ_QSID_MH_TIMER_INDEX);
      const uint8_t len_valid = sizeof(global_saved_values.mh_timer_choices) / sizeof(uint16_t);
      if (wanted >= len_valid) {
        wanted = len_valid - 1;
      }
      global_saved_values.mh_timer_index = wanted;
      ret = 0;
    } break;
    case SVAL_QSID_MH_TIMER_SLOT0_MS:
    case SVAL_QSID_MH_TIMER_SLOT1_MS:
    case SVAL_QSID_MH_TIMER_SLOT2_MS:
    case SVAL_QSID_MH_TIMER_SLOT3_MS: {
      const int idx = qsid - SVALBOARD_BASE_QSID - SVAL_QSID_MH_TIMER_SLOT0_MS;
      int16_t wanted;
      STATIC_ASSERT(sizeof(wanted) == SVAL_SZ_QSID_MH_TIMER_SLOT_VALUES);
      if (maxsz < SVAL_SZ_QSID_MH_TIMER_SLOT_VALUES)
        return -1;
      (void)memcpy(&wanted, setting, SVAL_SZ_QSID_MH_TIMER_SLOT_VALUES);
      global_saved_values.mh_timer_choices[idx] = wanted;
      ret = 0;
    }
    default:
      return -1;
  }
  if (ret != -1) {
    write_eeprom_kb();
  }
  return ret;
}
