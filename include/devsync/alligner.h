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


#ifndef INCLUDED_DEVSYNC_ALLIGNER_H
#define INCLUDED_DEVSYNC_ALLIGNER_H

#include <devsync/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace devsync {

    /*!
     * \brief Sample alligner block
     * \ingroup devsync
     *
     * \details
     * This block alligns the samples of the receiving streams
     * according to the information provided by the peak detector
     * block. One stream is taken as the reference stream, and
     * an offset is applied to the rest of the streams so that
     * they become alligned in phase.
     */
    class DEVSYNC_API alligner : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<alligner> sptr;

      /*!
       * Build the alligner block
       *
       * \param num_streams (size_t) Total number of receiving streams
       */
      static sptr make(size_t num_streams);
    };

  } // namespace devsync
} // namespace gr

#endif /* INCLUDED_DEVSYNC_ALLIGNER_H */

