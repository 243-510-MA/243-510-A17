"""
Embedded Python Blocks:

Each this file is saved, GRC will instantiate the first class it finds to get
ports and parameters of your block. The arguments to __init__  will be the
parameters. All of them are required to have default values!
"""
import numpy as np
from gnuradio import gr

class blk(gr.sync_block):
    def __init__(self, factor=1.0):  # only default arguments here
        gr.sync_block.__init__(
            self,
            name='Embedded Python Block',
            in_sig=[np.float32],
            out_sig=[np.float32]
        )
        self.factor = factor

    def work(self, input_items, output_items):
        output_items[0][:] = (((input_items[0][:]+np.pi)%(2*np.pi))-np.pi)
        return len(output_items[0])
