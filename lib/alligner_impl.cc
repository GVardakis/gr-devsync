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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "alligner_impl.h"

#include <volk/volk.h>
#define MODPI(a) (((a)<-M_PI) ? a+2*M_PI : ((a)>M_PI ? a-2*M_PI : a))

namespace gr
{
  namespace devsync
  {

    alligner::sptr
    alligner::make (size_t num_streams)
    {
      return gnuradio::get_initial_sptr (new alligner_impl (num_streams));
    }

    alligner_impl::alligner_impl (size_t num_streams) :
            gr::sync_block (
                "alligner",
                gr::io_signature::make (num_streams, num_streams,
                                        sizeof(gr_complex)),
                gr::io_signature::make (num_streams, num_streams,
                                        sizeof(gr_complex))),
            d_num_streams (num_streams),
            d_sample_counter (0),
            d_phase_init (pmt::mp ("phase_init")),
            ov_state (SYNCING),
            d_init_filled_streams (0),
            d_num_delayed_streams (0)
    {
      message_port_register_in (d_phase_init);
      d_delayed_samples_buf = NULL;
      d_sample_difference = new size_t[num_streams];
      d_cyc_buf_next_idx = new size_t[num_streams];
      d_filled_buf_rem = new size_t[num_streams];
      d_fine_phase_diffs = new double[num_streams];
      d_fine_complex_shifts = new std::complex<float>[num_streams];
      for (int i = 0; i < num_streams; i++) {
        d_sample_difference[i] = 0;
        d_cyc_buf_next_idx[i] = 0;
        d_filled_buf_rem[i] = 0;
        d_fine_phase_diffs[i] = 0;
      }
      set_msg_handler (
          d_phase_init,
          boost::bind (&alligner_impl::recv_phase_shifts, this, _1));
    }

    alligner_impl::~alligner_impl ()
    {
    }

    int
    alligner_impl::work (int noutput_items,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star &output_items)
    {
      const gr_complex *in;
      gr_complex *out;
      if (ov_state == SYNCING) {
        for (int i = 0; i < d_num_streams; i++) {
          in = (const gr_complex *) input_items[i];
          out = (gr_complex *) output_items[i];
          memcpy (out, in, noutput_items * sizeof(gr_complex));
        }
        return noutput_items;
      }
      else if (ov_state == NO_PHASE_DIFF) {
        for (int i = 0; i < d_num_streams; i++) {
          in = (const gr_complex *) input_items[i];
          out = (gr_complex *) output_items[i];
          memcpy (out, in, noutput_items * sizeof(gr_complex));
        }
        return noutput_items;
      }
      if (ov_state == FIRST_BUFFER_FILL) {
        initial_buff_fill (input_items, output_items, noutput_items);
        if (ov_state == FIRST_BUFFER_FILL) {
          return noutput_items;
        }
      }
      if (ov_state == SYNCED) {
        delay_samples (input_items, output_items, noutput_items);
        return noutput_items;
      }
      return noutput_items;
    }

    void
    alligner_impl::recv_phase_shifts (pmt::pmt_t msg)
    {
      size_t* diffs = (size_t*) pmt::blob_data (pmt::tuple_ref (msg, 1));
      memcpy (d_sample_difference, diffs, d_num_streams * sizeof(size_t));
      d_ref_stream = pmt::to_uint64 (pmt::tuple_ref (msg, 0));
      gr_complex* fine_shifts = (gr_complex*) pmt::blob_data (
          pmt::tuple_ref (msg, 2));
      float angle;
      for (int i = 0; i < d_num_streams; i++) {
        angle = std::atan2 (std::imag (fine_shifts[i]),
                            std::real (fine_shifts[i]));
        std::complex<float> c (std::cos (-angle), std::sin (-angle));
        d_fine_complex_shifts[i] = c;
        if (d_sample_difference[i] != 0) {
          ov_state = FIRST_BUFFER_FILL;
        }
      }
      if (ov_state == FIRST_BUFFER_FILL) {
        setup_delays ();
      }
      else {
        ov_state = NO_PHASE_DIFF;
      }
      return;
    }

    void
    alligner_impl::setup_delays ()
    {
      d_delayed_samples_buf = (gr_complex**) malloc (
          d_num_streams * sizeof(gr_complex*));
      size_t temp_diffs[d_num_streams];
      memcpy (temp_diffs, d_sample_difference, d_num_streams * sizeof(size_t));
      for (int i = 0; i < d_num_streams; i++) {
        d_sample_difference[i] = temp_diffs[d_ref_stream] - temp_diffs[i];
        d_filled_buf_rem[i] = d_sample_difference[i];
        d_delayed_samples_buf[i] = (gr_complex*) malloc (
            d_sample_difference[i] * sizeof(gr_complex));
        if (d_sample_difference[i] > 0)
          d_num_delayed_streams++;
      }
      ov_state = FIRST_BUFFER_FILL;
      return;

    }

    void
    alligner_impl::initial_buff_fill (gr_vector_const_void_star &input_items,
                                      gr_vector_void_star &out,
                                      int noutput_items)
    {
      const gr_complex* in;
      for (int i = 0; i < d_num_streams; i++) {
        in = (const gr_complex *) input_items[i];
        if (d_filled_buf_rem[i] > 0) {
          if (noutput_items >= d_filled_buf_rem[i]) {
            if (d_cyc_buf_next_idx[i] == 0) {
              memcpy (&d_delayed_samples_buf[i][d_cyc_buf_next_idx[i]],
                      &in[noutput_items - d_filled_buf_rem[i]],
                      d_filled_buf_rem[i] * sizeof(gr_complex));
            }
            else {
              memcpy (&d_delayed_samples_buf[i][d_cyc_buf_next_idx[i]], in,
                      d_filled_buf_rem[i] * sizeof(gr_complex));
            }
            d_init_filled_streams++;
            d_cyc_buf_next_idx[i] = 0;
            d_filled_buf_rem[i] = 0;
          }
          else {
            memcpy (&d_delayed_samples_buf[i][d_cyc_buf_next_idx[i]], &in[0],
                    noutput_items * sizeof(gr_complex));
            d_cyc_buf_next_idx[i] += noutput_items;
            d_filled_buf_rem[i] -= noutput_items;
          }
        }
      }
      if (d_init_filled_streams == d_num_delayed_streams) {
        ov_state = SYNCED;
      }
      return;
    }

    void
    alligner_impl::delay_samples (gr_vector_const_void_star &input_items,
                                  gr_vector_void_star &output_items,
                                  int noutput_items)
    {
      const gr_complex* in;
      gr_complex* out;
      size_t num_first_copy = 0;
      size_t num_sec_copy = 0;
      for (int i = 0; i < d_num_streams; i++) {
        in = (const gr_complex *) input_items[i];
        out = (gr_complex *) output_items[i];
        if (d_sample_difference[i] == 0) {
          memcpy (out, in, noutput_items * sizeof(gr_complex));
          volk_32fc_s32fc_multiply_32fc (out, out, d_fine_complex_shifts[i],
                                         noutput_items);
          continue;
        }
        size_t len;
        if (noutput_items >= d_sample_difference[i]) {
          len = d_sample_difference[i];
        }
        else {
          len = noutput_items;
        }
        if (d_cyc_buf_next_idx[i] + len <= d_sample_difference[i]) {
          memcpy (out, &d_delayed_samples_buf[i][d_cyc_buf_next_idx[i]],
                  len * sizeof(gr_complex));
          memcpy (&d_delayed_samples_buf[i][d_cyc_buf_next_idx[i]],
                  &in[noutput_items - len], len * sizeof(gr_complex));
          if (noutput_items > len) {
            memcpy (out + len, in, (noutput_items - len) * sizeof(gr_complex));

          }
          d_cyc_buf_next_idx[i] = (d_cyc_buf_next_idx[i] + len)
              % d_sample_difference[i];
        }
        else {
          num_first_copy = d_sample_difference[i] - d_cyc_buf_next_idx[i];
          num_sec_copy = len - num_first_copy;
          memcpy (out, &d_delayed_samples_buf[i][d_cyc_buf_next_idx[i]],
                  num_first_copy * sizeof(gr_complex));
          memcpy (out + num_first_copy, d_delayed_samples_buf[i],
                  num_sec_copy * sizeof(gr_complex));
          memcpy (&d_delayed_samples_buf[i][d_cyc_buf_next_idx[i]],
                  &in[noutput_items - len],
                  num_first_copy * sizeof(gr_complex));
          memcpy (&d_delayed_samples_buf[i][0],
                  &in[noutput_items - num_sec_copy],
                  num_sec_copy * sizeof(gr_complex));
          d_cyc_buf_next_idx[i] = num_sec_copy % d_sample_difference[i];
          if (noutput_items > len) {
            memcpy (out + len, in, (noutput_items - len) * sizeof(gr_complex));
          }
        }
        volk_32fc_s32fc_multiply_32fc (out, out, d_fine_complex_shifts[i],
                                       noutput_items);
      }
    }
  } /* namespace devsync */
} /* namespace gr */

