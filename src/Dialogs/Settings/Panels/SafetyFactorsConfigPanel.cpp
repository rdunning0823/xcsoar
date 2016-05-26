/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
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

#include "SafetyFactorsConfigPanel.hpp"
#include "Profile/ProfileKeys.hpp"
#include "Profile/Profile.hpp"
#include "Widget/RowFormWidget.hpp"
#include "Form/DataField/Float.hpp"
#include "Form/DataField/Enum.hpp"
#include "Interface.hpp"
#include "Components.hpp"
#include "Task/ProtectedTaskManager.hpp"
#include "Language/Language.hpp"
#include "Units/Units.hpp"
#include "Formatter/UserUnits.hpp"
#include "UIGlobals.hpp"

enum ControlIndex {
  ArrivalHeight,
  ArrivalHeightGR,
  TerrainHeight,
  AlternateMode,
  PolarDegradation,
  RiskFactor,
};

class SafetyFactorsConfigPanel final : public RowFormWidget {
public:
  SafetyFactorsConfigPanel()
    :RowFormWidget(UIGlobals::GetDialogLook()) {}

  virtual void Prepare(ContainerWindow &parent, const PixelRect &rc) override;
  virtual bool Save(bool &changed) override;
};

void
SafetyFactorsConfigPanel::Prepare(ContainerWindow &parent, const PixelRect &rc)
{
  RowFormWidget::Prepare(parent, rc);

  const ComputerSettings &settings_computer = CommonInterface::GetComputerSettings();
  const TaskBehaviour &task_behaviour = settings_computer.task;

  AddFloat(_("Arrival height"),
           _("The height above terrain that the glider should arrive at for a safe landing.  This is also added to height to complete a task."),
           _T("%.0f %s"), _T("%.0f"),
           fixed(0), fixed(10000), fixed(100), false,
           UnitGroup::ALTITUDE, task_behaviour.safety_height_arrival);

  AddFloat(_("Arrival height for GR"),
           _("For gradient (GR) calculations. The safety height above terrain at the destination used to calculated the gradient (slope) to arrive there."),
           _T("%.0f %s"), _T("%.0f"),
           fixed(0), fixed(10000), fixed(100), false,
           UnitGroup::ALTITUDE, task_behaviour.safety_height_arrival_gr);

  AddFloat(_("Terrain height"),
           _("The height above terrain that the glider must clear during final glide.  This does not affect arrival height, but displays warnings on the screen where a mountain will be hit or an X in the final glide bar."),
           _T("%.0f %s"), _T("%.0f"),
           fixed(0), fixed(10000), fixed(100), false,
           UnitGroup::ALTITUDE, task_behaviour.route_planner.safety_height_terrain);

  static constexpr StaticEnumChoice abort_task_mode_list[] = {
    { (unsigned)AbortTaskMode::SIMPLE, N_("Simple"),
      N_("The alternates will only be sorted by waypoint type (airport/outlanding field) and arrival height.") },
    { (unsigned)AbortTaskMode::TASK, N_("Task"),
      N_("The sorting will also take the current task direction into account.") },
    { (unsigned)AbortTaskMode::HOME, N_("Home"),
      N_("The sorting will try to find landing options in the current direction to the configured home waypoint.") },
    { 0 }
  };

  AddEnum(_("Alternates mode"),
          _("Determines sorting of alternates in the alternates dialog and in abort mode."),
          abort_task_mode_list, (unsigned)task_behaviour.abort_task_mode);
  SetExpertRow(AlternateMode);

  AddFloat(_("Polar degradation"), /* xgettext:no-c-format */
           _("A permanent polar degradation. "
             "0% means no degradation, "
             "50% indicates the glider's sink rate is doubled."),
           _T("%.0f %%"), _T("%.0f"),
           fixed(0), fixed(50), fixed(1), false,
           (fixed(1) - settings_computer.polar.degradation_factor) * 100);
  SetExpertRow(PolarDegradation);

  AddFloat(_("STF risk factor"),
           _("The STF risk factor reduces the MacCready setting used to calculate speed to fly as the glider gets low, in order to compensate for risk. Set to 0.0 for no compensation, 1.0 scales MC linearly with current height (with reference to height of the maximum climb). If considered, 0.3 is recommended."),
           _T("%.1f %s"), _T("%.1f"),
           fixed(0), fixed(1), fixed(0.1), false,
           task_behaviour.risk_gamma);
  SetExpertRow(RiskFactor);
}

bool
SafetyFactorsConfigPanel::Save(bool &_changed)
{
  bool changed = false;

  ComputerSettings &settings_computer = CommonInterface::SetComputerSettings();
  TaskBehaviour &task_behaviour = settings_computer.task;

  changed |= SaveValue(ArrivalHeight, UnitGroup::ALTITUDE,
                       ProfileKeys::SafetyAltitudeArrival,
                       task_behaviour.safety_height_arrival);

  changed |= SaveValue(ArrivalHeightGR, UnitGroup::ALTITUDE,
                       ProfileKeys::SafetyAltitudeArrivalGR,
                       task_behaviour.safety_height_arrival_gr);

  changed |= SaveValue(TerrainHeight, UnitGroup::ALTITUDE,
                       ProfileKeys::SafetyAltitudeTerrain,
                       task_behaviour.route_planner.safety_height_terrain);

  changed |= SaveValueEnum(AlternateMode, ProfileKeys::AbortTaskMode,
                           task_behaviour.abort_task_mode);

  fixed degradation = (fixed(1) - settings_computer.polar.degradation_factor) * 100;
  if (SaveValue(PolarDegradation, degradation)) {
    settings_computer.polar.SetDegradationFactor(fixed(1) - degradation / 100);
    Profile::Set(ProfileKeys::PolarDegradation,
                 settings_computer.polar.degradation_factor);
    if (protected_task_manager != nullptr)
      protected_task_manager->SetGlidePolar(settings_computer.polar.glide_polar_task);
    changed = true;
  }

  if (SaveValue(RiskFactor, task_behaviour.risk_gamma)) {
    Profile::Set(ProfileKeys::RiskGamma,
                 iround(task_behaviour.risk_gamma * 10));
    changed = true;
  }

  _changed |= changed;

  return true;
}

Widget *
CreateSafetyFactorsConfigPanel()
{
  return new SafetyFactorsConfigPanel();
}
