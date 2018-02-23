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
#include "peak_detector_impl.h"
#include <devsync/log.h>
#include <numeric>
#include <volk/volk.h>
namespace gr
{
  namespace devsync
  {

    peak_detector::sptr
    peak_detector::make (size_t num_streams, size_t window, float scale,
                         float influence, size_t discarted_samples)
    {
      return gnuradio::get_initial_sptr (
          new peak_detector_impl (num_streams, window, scale, influence,
                                  discarted_samples));
    }

    peak_detector_impl::peak_detector_impl (size_t num_streams, size_t window,
                                            float scale, float influence,
                                            size_t discarted_samples) :
            gr::sync_block (
                "peak_detector",
                gr::io_signature::make (num_streams, num_streams,
                                        sizeof(gr_complex)),
                gr::io_signature::make (0, 0,0)),
            d_num_streams (num_streams),
            d_window (window),
            d_scale (scale),
            d_influence (influence),
            d_discarted_samples (discarted_samples),
            d_sample_counter (0),
            ov_state (WINDOW_FILL),
            d_synced_streams (0),
            d_ref_stream (0),
            d_cyc_buf_next_idx (0),
            d_discarted_samples_counter (0),
            d_ninput_items (0),
            d_noutput_items (0),
            d_phase_shifts_out (pmt::mp ("shifts_out"))
    {
      d_state_vector = new state[num_streams];
      d_sample_difference = new size_t[num_streams];
      d_zero_counter = new size_t[num_streams];
      avgFilter = new float[num_streams];
      stdFilter = new float[num_streams];
      d_sum = new float[num_streams];
      d_sumsqr = new float[num_streams];
      d_amp_thres = new float[num_streams];
      filteredY.resize (num_streams);
      d_max_sample = new gr_complex[num_streams];
      d_max_abs_sample = new float[num_streams];
      for (int i = 0; i < num_streams; i++) {
        d_state_vector[i] = NO_SYNC;
        d_sample_difference[i] = 0;
        d_max_sample[i] = 0;
        d_max_abs_sample[i] = 0;
        d_zero_counter[i] = 0;
        filteredY[i].resize (d_window);
      }
      message_port_register_out (d_phase_shifts_out);
      LOG_INFO("Discarding initial samples");
    }

    peak_detector_impl::~peak_detector_impl ()
    {
    }

    int
    peak_detector_impl::work (int noutput_items,
                              gr_vector_const_void_star &input_items,
                              gr_vector_void_star &output_items)
    {
      const gr_complex *in;
      float temp;
      d_ninput_items += noutput_items;

      /* At the beginning we discard some samples */
      if (d_discarted_samples_counter < d_discarted_samples) {
        d_discarted_samples_counter += noutput_items;
        d_noutput_items += noutput_items;
        return noutput_items;
      }
      /* Next we estimate the noise floor */
      if (ov_state == WINDOW_FILL) {
        if (d_sample_counter + noutput_items <= d_window) {
          for (int i = 0; i < d_num_streams; i++) {
            in = (const gr_complex *) input_items[i];
            volk_32fc_magnitude_squared_32f (
                filteredY[i].data () + d_sample_counter, in, noutput_items);
          }
          d_sample_counter += noutput_items;
          if (d_sample_counter + noutput_items == d_window) {
            ov_state = SYNCING;
            d_sample_counter = 0;
            mean_std (-1, 0);
            d_cyc_buf_next_idx++;
          }
          d_noutput_items += noutput_items;
          return noutput_items;
        }
        else {
          for (int i = 0; i < d_num_streams; i++) {
            in = (const gr_complex *) input_items[i];
            volk_32fc_magnitude_squared_32f (
                filteredY[i].data () + d_sample_counter, in,
                (d_window - d_sample_counter));
          }
          d_sample_counter += (d_window - d_sample_counter);
          d_noutput_items += (d_window - d_sample_counter);
          ov_state = FIRST_SYNCING;
          mean_std (-1, 0);
          d_cyc_buf_next_idx++;
          LOG_INFO("Transmit the training sequence!");
        }
      }
      if (ov_state == SYNCED) {
        d_noutput_items += noutput_items;
        return noutput_items;
      }
      int c;
      if (ov_state == FIRST_SYNCING) {
        c = (d_window - d_sample_counter);
        d_sample_counter = 0;
      }
      else {
        c = 0;
      }
      size_t prev_index;
      if (d_cyc_buf_next_idx == 0) {
        prev_index = (d_window - 1);
      }
      else {
        prev_index = (d_cyc_buf_next_idx - 1);
      }
      for (int i = c; i < noutput_items; i++) {

        if (ov_state == FIRST_SYNC_FOUND) {
          d_sample_counter++;
        }
        else if (ov_state == SYNCED) {
          d_noutput_items += (noutput_items - i);
          break;
        }
        d_noutput_items++;
        float curr_sample;
        for (int stream = 0; stream < d_num_streams; stream++) {
          in = (const gr_complex *) input_items[stream];
          curr_sample = std::pow (std::real (in[i]), 2)
              + std::pow (std::imag (in[i]), 2);
          if (d_state_vector[stream] == SYNC) {
            continue;
          }

          /* The following three conditions will determine the existence
           * of a peak */
          if ((curr_sample > 0.0001)
              && std::abs (curr_sample - avgFilter[stream])
                  > d_scale * filteredY[stream][prev_index]) {
             LOG_DEBUG("Abs value %f thresh %f \n",
                    std::abs (curr_sample - avgFilter[stream]),
                    d_threshold * filteredY[stream][prev_index]);
            if (curr_sample > avgFilter[stream]) {
              if (curr_sample > d_max_abs_sample[stream]) {
                d_max_sample[stream] = in[i];
                d_max_abs_sample[stream] = curr_sample;
                d_zero_counter[stream] = 0;
              }
              else {
                d_zero_counter[stream]++;
              }
            }
            temp = filteredY[stream][d_cyc_buf_next_idx];
            filteredY[stream][d_cyc_buf_next_idx] = d_influence * curr_sample
                + (1 - d_influence) * filteredY[stream][prev_index];
            mean_std (stream, temp);
          }
          else {
            temp = filteredY[stream][d_cyc_buf_next_idx];
            filteredY[stream][d_cyc_buf_next_idx] = curr_sample;
            mean_std (stream, temp);
          }
          if (d_zero_counter[stream] > 100) {
            LOG_DEBUG ("Last max for stream %d is %f\n", stream,
                    d_max_abs_sample[stream]);
            d_state_vector[stream] = SYNC;
            LOG_INFO("Stream %d synced", stream);
            d_synced_streams++;
            if (!(ov_state == FIRST_SYNC_FOUND)) {
              d_sample_counter = d_zero_counter[stream];
              d_sample_difference[stream] = 0;
              ov_state = FIRST_SYNC_FOUND;
            }
            else {
              d_sample_difference[stream] = d_sample_counter
                  - d_zero_counter[stream];
            }
            if (d_synced_streams == d_num_streams) {
              d_ref_stream = stream;
              ov_state = SYNCED;
              LOG_INFO("All streams synced");
              pmt::pmt_t tup = pmt::make_tuple (
                  pmt::from_uint64 (d_ref_stream),
                  pmt::make_blob (d_sample_difference,
                                  d_num_streams * sizeof(size_t)),
                  pmt::make_blob (d_max_sample,
                                  d_num_streams * sizeof(gr_complex)));
              message_port_pub (d_phase_shifts_out, tup);
              for (int l = 0; l < d_num_streams; l++) {
                LOG_INFO("Stream %d, sample offset %d\n", l, d_sample_difference[l]);
              }
            }
          }
          if (d_max_abs_sample[stream] > 0) {
            if (curr_sample > d_max_abs_sample[stream]) {
              d_zero_counter[stream] = 0;
            }
            else {
              d_zero_counter[stream]++;
            }
          }
          d_cyc_buf_next_idx = d_cyc_buf_next_idx % d_window;
        }
      }
      if (ov_state == FIRST_SYNCING)
        ov_state = SYNCING;

      return noutput_items;
    }
    void
    peak_detector_impl::mean_std (int stream, float discarded_val)
    {
      if (stream == -1) {
        for (int i = 0; i < d_num_streams; i++) {
          if (d_state_vector[i] == NO_SYNC) {
            float sum = std::accumulate (std::begin (filteredY[i]),
                                         std::end (filteredY[i]), 0.0);
            avgFilter[i] = sum / filteredY[i].size ();
            d_sum[i] = sum;

            float accum = 0.0;
            std::for_each (std::begin (filteredY[i]), std::end (filteredY[i]),
                           [&](const float d) {
                             accum += (d - avgFilter[i]) * (d - avgFilter[i]);
                           });
            stdFilter[i] = sqrt (accum / (filteredY[i].size () - 1));

            std::for_each (std::begin (filteredY[i]), std::end (filteredY[i]),
                           [&](const float d) {
                             accum += (d * d);
                           });
            d_sumsqr[i] = accum;
            d_amp_thres[i] = avgFilter[i];
          }
        }
      }
      else {
        if (d_state_vector[stream] == NO_SYNC) {
          double mean = 0, M2 = 0, variance = 0;
          d_sum[stream] = d_sum[stream] - discarded_val
              + filteredY[stream][d_cyc_buf_next_idx];
          d_sumsqr[stream] = d_sumsqr[stream] - discarded_val * discarded_val
              + filteredY[stream][d_cyc_buf_next_idx]
                  * filteredY[stream][d_cyc_buf_next_idx];
          avgFilter[stream] = d_sum[stream] / d_window;
          stdFilter[stream] = std::sqrt (
              (d_sumsqr[stream] - 2 * avgFilter[stream] * d_sum[stream]
                  + d_window * avgFilter[stream] * avgFilter[stream])
                  / (d_window - 1));

        }
      }
    }

  } /* namespace devsync */
} /* namespace gr */

