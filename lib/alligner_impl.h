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

#ifndef INCLUDED_DEVSYNC_ALLIGNER_IMPL_H
#define INCLUDED_DEVSYNC_ALLIGNER_IMPL_H

#include <devsync/alligner.h>

namespace gr
{
  namespace devsync
  {

    class alligner_impl : public alligner
    {
    private:
      size_t d_num_streams;
      size_t d_sample_counter;
      pmt::pmt_t d_phase_init;
      overall_state ov_state;
      size_t d_init_filled_streams;
      size_t d_num_delayed_streams;
      gr_complex** d_delayed_samples_buf;
      size_t *d_sample_difference;
      size_t *d_cyc_buf_next_idx; /*Index of available next sample to be copied to the output stream*/
      size_t *d_filled_buf_rem; /*How many bytes remaining for the buffer of the stream
       to fill initially*/
      double *d_fine_phase_diffs;
      std::complex<float>* d_fine_complex_shifts;
      boost::shared_ptr<boost::thread> d_init_thread;
      size_t d_ref_stream;

    public:
      alligner_impl (size_t num_streams);
      ~alligner_impl ();

      void
      recv_phase_shifts (pmt::pmt_t msg);
      void
      setup_delays ();
      void
      initial_buff_fill (gr_vector_const_void_star &input_items,
                         gr_vector_void_star &out, int noutput_items);
      void
      delay_samples (gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items, int noutput_items);

      int
      work (int noutput_items, gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items);
    };

  } // namespace devsync
} // namespace gr

#endif /* INCLUDED_DEVSYNC_ALLIGNER_IMPL_H */

