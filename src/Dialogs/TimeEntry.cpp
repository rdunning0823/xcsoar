/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2011 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Dialogs/TimeEntry.hpp"
#include "Form/Form.hpp"
#include "Form/ButtonPanel.hpp"
#include "Form/DigitEntry.hpp"
#include "Screen/SingleWindow.hpp"
#include "Language/Language.hpp"
#include "UIGlobals.hpp"
#include "Time/RoughTime.hpp"
#include "Time/BrokenDateTime.hpp"

enum {
  CLEAR = 100,
};

bool
TimeEntryDialog(const TCHAR *caption, RoughTime &value, bool nullable)
{
  /* create the dialog */

  const DialogLook &look = UIGlobals::GetDialogLook();

  WindowStyle dialog_style;
  dialog_style.Hide();
  dialog_style.ControlParent();

  WndForm dialog(look);
  dialog.Create(UIGlobals::GetMainWindow(), caption, dialog_style);

  ContainerWindow &client_area = dialog.GetClientAreaWindow();

  /* create button panel */

  ButtonPanel buttons(client_area, look);
  const PixelRect rc = buttons.UpdateLayout();

  /* create the command buttons */

  WindowStyle control_style;
  control_style.TabStop();

  DigitEntry entry(look);
  entry.CreateTime(client_area, rc, control_style);
  entry.SetValue(value);
  entry.SetActionListener(dialog, mrOK);

  /* create buttons */

  buttons.Add(_("OK"), dialog, mrOK);
  buttons.Add(_("Cancel"), dialog, mrCancel);

  buttons.Add(_("Now"), [&entry](){
      const BrokenTime bt = BrokenDateTime::NowUTC();
      entry.SetValue(RoughTime(bt.hour, bt.minute));
    });

  if (nullable)
    buttons.Add(_("Clear"), [&entry](){ entry.SetInvalid(); });

  entry.Move(buttons.UpdateLayout());

  /* run it */

  bool result = dialog.ShowModal() == mrOK;
  if (!result)
    return false;

  value = entry.GetTimeValue();
  return true;
}
