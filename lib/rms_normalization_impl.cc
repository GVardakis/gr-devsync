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
#include "rms_normalization_impl.h"

#include <armadillo>
#include <volk/volk.h>
using namespace arma;
namespace gr
{
  namespace devsync
  {

    rms_normalization::sptr
    rms_normalization::make (size_t num_streams, size_t vlen)
    {
      return gnuradio::get_initial_sptr (
          new rms_normalization_impl (num_streams, vlen));
    }

    rms_normalization_impl::rms_normalization_impl (size_t num_streams,
                                                    size_t vlen) :
            gr::sync_block (
                "rms_normalization",
                gr::io_signature::make (num_streams, num_streams,
                                        sizeof(std::complex<float>) * vlen),
                gr::io_signature::make (num_streams, num_streams,
                                        sizeof(std::complex<float>) * vlen)),
            d_vlen (vlen),
            d_num_streams (num_streams)
    {
    }

    rms_normalization_impl::~rms_normalization_impl ()
    {
    }

    int
    rms_normalization_impl::work (int noutput_items,
                                  gr_vector_const_void_star &input_items,
                                  gr_vector_void_star &output_items)
    {
      std::complex<float> **in = (std::complex<float> **) &input_items[0];

      std::complex<float> **out = (std::complex<float> **) &output_items[0];
      for (int i = 0; i < noutput_items; i++) {
        cx_fmat batch (&in[0][i * d_vlen], 1, d_vlen, false, true);
        float ref = std::sqrt (sum (sum (pow (abs (batch), 2))) / d_vlen);
        memcpy (&out[0][i * d_vlen], &in[0][i * d_vlen],
                sizeof(std::complex<float>) * d_vlen);
        for (int j = 1; j < d_num_streams; j++) {
          cx_fmat batch (&in[j][i * d_vlen], 1, d_vlen, false, true);
          float rms = std::sqrt (sum (sum (pow (abs (batch), 2))) / d_vlen);
          batch = batch * (ref / rms);
          memcpy (&out[j][i * d_vlen], batch.memptr (),
                  sizeof(std::complex<float>) * d_vlen);
        }
      }
      return noutput_items;
    }

  } /* namespace devsync */
} /* namespace gr */

