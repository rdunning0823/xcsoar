/* Copyright_License {

  Top Hat Soaring Glide Computer - http://www.tophatsoaring.org/
  Copyright (C) 2000-2016 The Top Hat Soaring Project
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
#ifndef FAI_TASK_FACTORY_HPP
#define FAI_TASK_FACTORY_HPP

#include "AbstractTaskFactory.hpp"

/**
 * Factory for construction of legal FAI tasks
 * Currently the validate() method will check 4-point tasks as to whether they
 * satisfy short and long-distance FAI triangle rules.
 */
class FAITaskFactory: 
  public AbstractTaskFactory 
{
protected:
  FAITaskFactory(const TaskFactoryConstraints &_constraints,
                 OrderedTask& _task,
                 const TaskBehaviour &tb);

public:
/** 
 * Constructor
 * 
 * @param _task Ordered task to be managed by this factory
 * @param tb Behaviour (options)
 */  
  FAITaskFactory(OrderedTask& _task,
                 const TaskBehaviour &tb);

  virtual ~FAITaskFactory() {};

  void UpdateOrderedTaskSettings(OrderedTaskSettings& to) override;

  /**
   * Check whether task is complete and valid according to factory rules
   * Adds error types to validation_errors
   *
   * @return True if task is valid according to factory rules
   */
  bool Validate() override;

  /**
   * Is the boundary of the start cylinder scored
   * instead of the center with an adjustment?
   * Start line not affected
   */
  bool IsStartBoundaryScored(bool is_american_task) const override;

  /**
   * swaps non FAI OZs with either FAI OZs
   * based on the shape of the input point
   * @param tp
   * @return: point type compatible with current factory, most
   * similar to type of tp
   */
  TaskPointFactoryType GetMutatedPointType(const OrderedTaskPoint &tp) const override;

  /**
   * @param start_radius: either fixed(-1) or a valid value
   * @param turnpoint_radius: either fixed(-1) or a valid value
   * @param finish_radius: either fixed(-1) or a valid value
   *
   * only affects start cylinder/line, finish cylinder/line and
   * turnpoint sector
   * Does not affects FAI sectors which have their own class types
   *
   * sets radiuses FAI defaults
   */
  void GetPointDefaultSizes(const TaskPointFactoryType type,
                            fixed &start_radius,
                            fixed &turnpoint_radius,
                            fixed &finish_radius) const override;
};

#endif
