import os
import numpy as np
import sys

def read_received():
	calibration_file = 'calibration.csv'
	receiver_file = 'received.csv'
	if not os.path.isfile(calibration_file):
		print("Missing calibration timing input. Please run calibration: ./calibration")
		exit(0)
	if not os.path.isfile(receiver_file):
		print("Missing receiver timing file")
	no_CoW = [int(x.strip()) for x in open(calibration_file).readlines()][4:]
	CoW = [int(x.strip()) for x in open(receiver_file).readlines()]
	nc_median = np.median(no_CoW)
	nc_maximum = max(no_CoW)
	nc_min = min(no_CoW)
	print('Calibration Timing:\n min: %i\tmedian: %i\tmax: %i' % (nc_min, nc_median, nc_maximum))
	pages_written = {}
	l = int(len(CoW)/8)
	s = ''
	counter = 0
	c = 0
	for i in range(l-1,0, -1):
		start = i*8
		end = (i + 1)*8
		c_median = np.median(CoW[start:end])
		c_min = min(CoW[start:end])
		if (nc_median*2 > c_median):
			#print('Location %i written: ' % (l - i - 1))
			#print('Timing:\n min: %i\tmedian: %i' % (c_min, c_median))
			bit = 1	
		else:
			bit = 0
		c += bit*(2**(counter%8));
		if counter%8 == 7:
			s += chr(c)
			c = 0
		counter += 1
	print(s)

def get_offset(infile):
	if not os.path.isfile(infile):
		print("Missing %s" % (infile))
		exit(0)
	os.system('nm %s | grep %s > %s' % (infile, bufname, tmpfile))
	map_loc = None
	with open(tmpfile) as reader:
		for line in reader:
			l = line.strip().split(' ')
			if l[-1] == bufname:
				map_loc = int(l[0], 16)
	offset = pagesize - map_loc%pagesize
	start = map_loc + offset
	print("Map location: %i\tOffset: %i\tStarting Address %i\tPage Offest (should be 0): %i" % (map_loc, offset, start, start%pagesize))
	os.system('rm %s' % (tmpfile))
	return offset

if __name__ == '__main__':
	if len(sys.argv) < 2:
		print("Provide one of the following options: s (sender), r (receiver)")
		exit(0)
	options = {'s': {'infile': 'sender', 'options': 'secret.txt'}, \
		'r': {'infile': 'receiver', 'options': ''}}
	option = sys.argv[1]
	tmpfile = 'tmp'
	bufname = 'map'
	pagesize = 4096
	seed = 100
	offset = get_offset(options[option]['infile'])
	#os.system('./%s %i %i %s' % (options[option]['infile'], offset/4, seed, options[option]['options']))
	if option == 'r':
		read_received()
