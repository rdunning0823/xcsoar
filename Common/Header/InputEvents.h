#ifndef INPUTEVENTS_H
#define INPUTEVENTS_H

#include "XCSoar.h"
#include "externs.h"

class InputEvents {
 public:
  static void readFile();
  static int mode2int(TCHAR *mode, bool create);
  static void setMode(TCHAR *mode);
  static TCHAR* getMode();
  static bool processKey(int key);
  static bool processNmea(TCHAR *data);
  static void eventScreenModes(TCHAR *misc);
  static void eventSnailTrail(TCHAR *misc);
  static void eventSounds(TCHAR *misc);
  static void eventMarkLocation(TCHAR *misc);
  static void eventFullScreen(TCHAR *misc);
  static void eventAutoZoom(TCHAR *misc);
  static void eventScaleZoom(TCHAR *misc);
  static void eventPan(TCHAR *misc);
  static void eventClearWarningsAndTerrain(TCHAR *misc);
  static void eventSelectInfoBox(TCHAR *misc);
  static void eventChangeInfoBoxType(TCHAR *misc);
  static void eventDoInfoKey(TCHAR *misc);
  static void eventPanCursor(TCHAR *misc);
  static void eventMainMenu(TCHAR *misc);

};


#endif
