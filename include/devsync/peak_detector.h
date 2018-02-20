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


#ifndef INCLUDED_DEVSYNC_PEAK_DETECTOR_H
#define INCLUDED_DEVSYNC_PEAK_DETECTOR_H

#include <devsync/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace devsync {

    /*!
     * \brief The peak detector block
     * \ingroup devsync
     *
     * \details This block performs a simple peak detection algorithm
     * on the output of the FFT filter blocks in order to detect the
     * peaks that correspond to the reception of the training sequence
     * by the receiving channels.
     *
     * The peak detection algorithm first makes a rough noise floor
     * estimation. Next, it attempts to detect peak values by keeping
     * a moving average of previous energy values. If the difference
     * in energy levels between the last and the new sample exceeds the
     * last sample's energy, scaled by a value, then a peak
     * is detected. The user also has the option, through the influence
     * parameter to dictate if the current sample's energy (influence = 1)
     * , or the moving average of the previous samples in the window
     * (influence = 0), will have the most effect on the calculations.
     *
     * The last stream on which a peak is detected, is taken as the
     * reference stream : all remaining streams will allign their samples
     * according to it. Apart from the coarse synchronization, in
     * sample level, between the streams, a fine synchronization also
     * takes place to account for phase differences smaller than a
     * sample. This is done by estimating the angle between the peak
     * value of the reference stream, and the rest of the streams.
     * All the information is passed through the message output to
     * the alligner block.
     *
     */
    class DEVSYNC_API peak_detector : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<peak_detector> sptr;

      /*!
       * Build the peak detection block
       *
       * \param num_streams (size_t) The total number of receiving streams
       * \param window (size_t) The length in samples of the window over which
       * the noise floor estimation at the beginning will occur
       * \param scale (float) Scaling value of the previous sample's energy
       * used in the calculations of the peak detection
       * \param influence (float) The influence of the new sample's energy
       * in the peak detection algorithm calculations. 1 for total trust
       * on the new sample's value in the algorithm, 0 for total trust on
       * the moving average calculated by previous samples
       * \param discarted_samples (size_t) Number of samples to discard in
       * the beginning of execution
       */
      static sptr make(size_t num_streams, size_t window, float scale, float influence, size_t discarted_samples);
    };

  } // namespace devsync
} // namespace gr

#endif /* INCLUDED_DEVSYNC_PEAK_DETECTOR_H */

