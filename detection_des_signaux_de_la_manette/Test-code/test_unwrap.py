"""
CE CODE NE MARCHE PAS
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!


version modifie de http://aaronscher.com/GNU_Radio_Companion_Collection/Embedded_Python_Example1.html
"""

import numpy as np
from gnuradio import gr


class blk(gr.sync_block):  # other base classes are basic_block, decim_block, interp_block
    """Embedded Python Block example - a simple multiply const"""

    def __init__(self, example_param=1):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.sync_block.__init__(
            self,
            name='Embedded Python Block',   # will show up in GRC
            in_sig=[np.float32],
            out_sig=[np.float32]
        )
        # if an attribute with the same name as a parameter is found,
        # a callback is registered (properties work, too).
        self.example_param = example_param

    def work(self, input_items, output_items):
        """example: multiply with constant"""
		phase = np.linspace(0, np.pi, num=2)
		#phase[1]=input_items[0]
		phase = np.unwrap(phase)
		#output_items=np.unwrap(input_items)
		#output_items[0][:]=input_items[0]
		output_items[0][:]=phase[0]#<- cette ligne pose probleme
        return (output_items[0])