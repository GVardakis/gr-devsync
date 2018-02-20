/* -*- c++ -*- */
/*
    gr-devsync a phase synchronizer for mutliple SDR devices
    Copyright (C) 2018  George Vardakis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef INCLUDED_DEVSYNC_API_H
#define INCLUDED_DEVSYNC_API_H

#include <gnuradio/attributes.h>

#ifdef gnuradio_devsync_EXPORTS
#  define DEVSYNC_API __GR_ATTR_EXPORT
#else
#  define DEVSYNC_API __GR_ATTR_IMPORT
#endif

typedef enum{
  NO_SYNC=0,
  PREAMBLE=1,
  POST_PREAMBLE=2,
  SYNC=3,
  IN_SYNC=4,
}state;

typedef enum{
  SYNCING=0,
  FIRST_SYNC_NOW=1,
  FIRST_SYNC_FOUND=2,
  FIRST_BUFFER_FILL=3,
  SYNCED=4,
  NO_PHASE_DIFF=5,
  FINE_SYNC=6,
  WINDOW_FILL=7,
  FIRST_SYNCING=8
}overall_state;

#define ENABLE_DEBUG_MSG = 1
#endif /* INCLUDED_DEVSYNC_API_H */
