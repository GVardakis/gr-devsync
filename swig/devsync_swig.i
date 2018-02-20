/* -*- c++ -*- */

#define DEVSYNC_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "devsync_swig_doc.i"

%{
#include "devsync/alligner.h"
#include "devsync/peak_detector.h"
#include "devsync/rms_normalization.h"
%}


%include "devsync/alligner.h"
GR_SWIG_BLOCK_MAGIC2(devsync, alligner);
%include "devsync/peak_detector.h"
GR_SWIG_BLOCK_MAGIC2(devsync, peak_detector);
%include "devsync/rms_normalization.h"
GR_SWIG_BLOCK_MAGIC2(devsync, rms_normalization);
