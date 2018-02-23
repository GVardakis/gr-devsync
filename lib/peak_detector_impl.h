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

#ifndef INCLUDED_DEVSYNC_PEAK_DETECTOR_IMPL_H
#define INCLUDED_DEVSYNC_PEAK_DETECTOR_IMPL_H

#include <devsync/peak_detector.h>
#include <vector>
namespace gr
{
  namespace devsync
  {

    class peak_detector_impl : public peak_detector
    {
    private:
      size_t d_num_streams;
      size_t d_window;
      float d_scale;
      float d_influence;
      size_t d_sample_counter;
      overall_state ov_state;
      size_t d_synced_streams;
      size_t d_ref_stream;
      size_t *d_sample_difference;
      state *d_state_vector;
      size_t d_cyc_buf_next_idx;
      size_t *d_zero_counter;
      size_t d_discarted_samples_counter;
      size_t d_discarted_samples;
      size_t d_ninput_items;
      size_t d_noutput_items;
      float *d_amp_thres;
      std::vector<std::vector<float>> filteredY;
      float *avgFilter;
      float *stdFilter;
      float *d_max_abs_sample;
      gr_complex *d_max_sample;
      float *d_sum;
      float *d_sumsqr;
      pmt::pmt_t d_phase_shifts_out;

    public:
      peak_detector_impl (size_t num_streams, size_t window, float scale,
                          float influence, size_t discarted_samples);
      ~peak_detector_impl ();
      void
      mean_std (int stream, float discarded_val);

      int
      work (int noutput_items, gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items);
    };

  } // namespace devsync
} // namespace gr

#endif /* INCLUDED_DEVSYNC_PEAK_DETECTOR_IMPL_H */

