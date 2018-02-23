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

#ifndef INCLUDED_DEVSYNC_RMS_NORMALIZATION_IMPL_H
#define INCLUDED_DEVSYNC_RMS_NORMALIZATION_IMPL_H

#include <devsync/rms_normalization.h>

namespace gr
{
  namespace devsync
  {

    class rms_normalization_impl : public rms_normalization
    {
    private:
      size_t d_vlen;
      size_t d_num_streams;

    public:
      rms_normalization_impl (size_t num_streams, size_t vlen);
      ~rms_normalization_impl ();

      int
      work (int noutput_items, gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items);
    };

  } // namespace devsync
} // namespace gr

#endif /* INCLUDED_DEVSYNC_RMS_NORMALIZATION_IMPL_H */

