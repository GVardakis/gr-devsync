#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Tue Feb 20 15:54:13 2018
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import cmath
import devsync
import numpy
import sys
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())


        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 32000
        self.num_streams = num_streams = 4
        self.num_elements = num_elements = 1024
        self.long_seq = long_seq = [ 0.1562 - 0.0j    , -0.0051 + 0.1203j, 0.0397 + 0.1112j, 0.0968 - 0.0828j,
             0.0211 - 0.0279j, 0.0598 + 0.0877j, -0.1151 + 0.0552j, -0.0383 + 0.1062j,
             0.0975 + 0.0259j, 0.0533 - 0.0041j, 0.0010 + 0.115j , -0.1368 + 0.0474j,
             0.0245 + 0.0585j, 0.0587 + 0.0149j, -0.0225 - 0.1607j, 0.1192 + 0.0041j,
             0.0625 + 0.0625j, 0.0369 - 0.0983j, -0.0572 - 0.0393j, -0.1313 - 0.0652j,
             0.0822 - 0.0924j, 0.0696 - 0.0141j, -0.0603 - 0.0813j, -0.0565 + 0.0218j,
             - 0.0350 + 0.1509j, -0.1219 + 0.0166j, -0.1273 + 0.0205j, 0.0751 + 0.074j ,
             - 0.0028 - 0.0538j, -0.0919 - 0.1151j, 0.0917 - 0.1059j, 0.0123 - 0.0976j,
             - 0.1562 - 0.0j    , 0.0123 + 0.0976j, 0.0917 + 0.1059j, -0.0919 + 0.1151j,
             - 0.0028 + 0.0538j, 0.0751 - 0.074j , -0.1273 - 0.0205j, -0.1219 - 0.0166j,
             - 0.0350 - 0.1509j, -0.0565 - 0.0218j, -0.0603 + 0.0813j, 0.0696 + 0.0141j,
             0.0822 + 0.0924j, -0.1313 + 0.0652j, -0.0572 + 0.0393j, 0.0369 + 0.0983j,
             0.0625 - 0.0625j, 0.1192 - 0.0041j, -0.0225 + 0.1607j, 0.0587 - 0.0149j,
             0.0245 - 0.0585j, -0.1368 - 0.0474j, 0.0010 - 0.115j , 0.0533 + 0.0041j,
             0.0975 - 0.0259j, -0.0383 - 0.1062j, -0.1151 - 0.0552j, 0.0598 - 0.0877j,
             0.0211 + 0.0279j, 0.0968 + 0.0828j, 0.0397 - 0.1112j, -0.0051 - 0.1203j] + 1000*[0]

        ##################################################
        # Blocks
        ##################################################
        self.fft_filter_xxx_0_4_0_0_0 = filter.fft_filter_ccc(1, (long_seq[::-1]), 1)
        self.fft_filter_xxx_0_4_0_0_0.declare_sample_delay(0)
        self.fft_filter_xxx_0_4_0_0 = filter.fft_filter_ccc(1, (long_seq[::-1]), 1)
        self.fft_filter_xxx_0_4_0_0.declare_sample_delay(0)
        self.fft_filter_xxx_0_4_0 = filter.fft_filter_ccc(1, (long_seq[::-1]), 1)
        self.fft_filter_xxx_0_4_0.declare_sample_delay(0)
        self.fft_filter_xxx_0_4 = filter.fft_filter_ccc(1, (long_seq[::-1]), 1)
        self.fft_filter_xxx_0_4.declare_sample_delay(0)
        self.devsync_rms_normalization_0 = devsync.rms_normalization(num_streams, num_elements)
        self.devsync_peak_detector_0 = devsync.peak_detector(num_streams, 100, 1, 0, 100)
        self.devsync_alligner_0 = devsync.alligner(num_streams)
        self.blocks_vector_to_stream_1_2 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, num_elements)
        self.blocks_vector_to_stream_1_1 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, num_elements)
        self.blocks_vector_to_stream_1_0 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, num_elements)
        self.blocks_vector_to_stream_1 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, num_elements)
        self.blocks_vector_source_x_1 = blocks.vector_source_c(numpy.conj(long_seq) , True, 1, [])
        self.blocks_stream_to_vector_0_3_1 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.blocks_stream_to_vector_0_0_0_1 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.blocks_stream_to_vector_0_0_0_0_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.blocks_stream_to_vector_0_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.blocks_null_source_0 = blocks.null_source(gr.sizeof_gr_complex*1)
        self.blocks_null_sink_1 = blocks.null_sink(gr.sizeof_gr_complex*1)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_gr_complex*1)



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.devsync_peak_detector_0, 'shifts_out'), (self.devsync_alligner_0, 'phase_init'))
        self.connect((self.blocks_null_source_0, 0), (self.devsync_alligner_0, 0))
        self.connect((self.blocks_null_source_0, 1), (self.devsync_alligner_0, 1))
        self.connect((self.blocks_null_source_0, 2), (self.devsync_alligner_0, 2))
        self.connect((self.blocks_null_source_0, 3), (self.devsync_alligner_0, 3))
        self.connect((self.blocks_null_source_0, 0), (self.fft_filter_xxx_0_4, 0))
        self.connect((self.blocks_null_source_0, 2), (self.fft_filter_xxx_0_4_0, 0))
        self.connect((self.blocks_null_source_0, 1), (self.fft_filter_xxx_0_4_0_0, 0))
        self.connect((self.blocks_null_source_0, 3), (self.fft_filter_xxx_0_4_0_0_0, 0))
        self.connect((self.blocks_stream_to_vector_0_0, 0), (self.devsync_rms_normalization_0, 1))
        self.connect((self.blocks_stream_to_vector_0_0_0_0_0, 0), (self.devsync_rms_normalization_0, 3))
        self.connect((self.blocks_stream_to_vector_0_0_0_1, 0), (self.devsync_rms_normalization_0, 2))
        self.connect((self.blocks_stream_to_vector_0_3_1, 0), (self.devsync_rms_normalization_0, 0))
        self.connect((self.blocks_vector_source_x_1, 0), (self.blocks_null_sink_0, 0))
        self.connect((self.blocks_vector_to_stream_1, 0), (self.devsync_peak_detector_0, 0))
        self.connect((self.blocks_vector_to_stream_1_0, 0), (self.devsync_peak_detector_0, 2))
        self.connect((self.blocks_vector_to_stream_1_1, 0), (self.devsync_peak_detector_0, 3))
        self.connect((self.blocks_vector_to_stream_1_2, 0), (self.devsync_peak_detector_0, 1))
        self.connect((self.devsync_alligner_0, 0), (self.blocks_null_sink_1, 0))
        self.connect((self.devsync_alligner_0, 1), (self.blocks_null_sink_1, 1))
        self.connect((self.devsync_alligner_0, 2), (self.blocks_null_sink_1, 2))
        self.connect((self.devsync_alligner_0, 3), (self.blocks_null_sink_1, 3))
        self.connect((self.devsync_rms_normalization_0, 0), (self.blocks_vector_to_stream_1, 0))
        self.connect((self.devsync_rms_normalization_0, 2), (self.blocks_vector_to_stream_1_0, 0))
        self.connect((self.devsync_rms_normalization_0, 3), (self.blocks_vector_to_stream_1_1, 0))
        self.connect((self.devsync_rms_normalization_0, 1), (self.blocks_vector_to_stream_1_2, 0))
        self.connect((self.fft_filter_xxx_0_4, 0), (self.blocks_stream_to_vector_0_3_1, 0))
        self.connect((self.fft_filter_xxx_0_4_0, 0), (self.blocks_stream_to_vector_0_0_0_1, 0))
        self.connect((self.fft_filter_xxx_0_4_0_0, 0), (self.blocks_stream_to_vector_0_0, 0))
        self.connect((self.fft_filter_xxx_0_4_0_0_0, 0), (self.blocks_stream_to_vector_0_0_0_0_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

    def get_num_streams(self):
        return self.num_streams

    def set_num_streams(self, num_streams):
        self.num_streams = num_streams

    def get_num_elements(self):
        return self.num_elements

    def set_num_elements(self, num_elements):
        self.num_elements = num_elements

    def get_long_seq(self):
        return self.long_seq

    def set_long_seq(self, long_seq):
        self.long_seq = long_seq
        self.fft_filter_xxx_0_4_0_0_0.set_taps((self.long_seq[::-1]))
        self.fft_filter_xxx_0_4_0_0.set_taps((self.long_seq[::-1]))
        self.fft_filter_xxx_0_4_0.set_taps((self.long_seq[::-1]))
        self.fft_filter_xxx_0_4.set_taps((self.long_seq[::-1]))
        self.blocks_vector_source_x_1.set_data(numpy.conj(self.long_seq) , [])


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
