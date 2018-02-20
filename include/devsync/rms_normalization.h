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


#ifndef INCLUDED_DEVSYNC_RMS_NORMALIZATION_H
#define INCLUDED_DEVSYNC_RMS_NORMALIZATION_H

#include <devsync/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace devsync {

    /*!
     * \brief RMS normalization block
     * \ingroup devsync
     *
     *\details
     * This block performs a simple RMS normalization method to bring
     * the amplitudes of all receiving channels at the same level for
     * effective peak detection.
     */
    class DEVSYNC_API rms_normalization : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<rms_normalization> sptr;

      /*!
       * Build the RMS normalization block
       *
       * \param num_streams (size_t) Total number of receiving streams
       * \param vlen (size_t) Length of window over which the normalization
       * will take place.
       *
       */
      static sptr make(size_t num_streams, size_t vlen);
    };

  } // namespace devsync
} // namespace gr

#endif /* INCLUDED_DEVSYNC_RMS_NORMALIZATION_H */

