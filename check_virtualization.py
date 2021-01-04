import math
import os
import sys

def median(x):
	s = sorted(x)
	l = len(x)
	i = math.floor(l/2)
	if l == 1:
		return s[0]
	if l%2 == 0:
		return (s[i] + s[i+1])/2
	return s[i]


def get_offset(infile, bufname, tmpfile):
	if not os.path.isfile(infile):
		print("Missing %s" % (infile))
		exit(0)
	os.system('nm %s | grep %s > %s' % (infile, bufname, tmpfile))
	map_loc = None
	with open(tmpfile) as reader:
		for line in reader:
			l = line.strip().split(' ')
			if l[-1] in bufnames:
				bufnames[l[-1]] = int(l[0], 16)
	offsets = []
	for bufname in ['map0', 'map1']:
		if bufnames[bufname] is None:
			print("Offset for %s not fount" % (bufname))
			os.system('rm %s' % (tmpfile))
			return None
		offsets.append(pagesize - bufnames[bufname]%pagesize)
		start = bufnames[bufname] + offsets[-1]
		print("Map %s location: %i\tOffset: %i\tStarting Address %i\tPage Offest (should be 0): %i"\
			 % (bufname, bufnames[bufname], offsets[-1], start, start%pagesize))
	os.system('rm %s' % (tmpfile))
	return offsets

def check_timings():
	calibration_file = 'calibration.csv'
	virtualization_file = 'check_virtualization.csv'
	if not os.path.isfile(calibration_file):
		print("Missing calibration timing input. Please run calibration: ./calibration")
		exit(0)
	if not os.path.isfile(virtualization_file):
		print("Missing virtualization check timing file. Please run check: ./check_virtualization")
		exit(0)
	no_CoW = [int(x.strip()) for x in open(calibration_file).readlines()][1:-1]
	CoW = [int(x.strip()) for x in open(virtualization_file).readlines()][1:-1]
	nc_median = median(no_CoW)
	nc_maximum = max(no_CoW)
	nc_min = min(no_CoW)
	c_median = median(CoW)
	c_maximum = max(CoW)
	c_min = min(CoW)
	print('No COW:\n min: %i\tmedian: %i\tmax: %i' % (nc_min, nc_median, nc_maximum))
	print('With COW:\n min: %i\tmedian: %i\tmax: %i' % (c_min, c_median, c_maximum))
	if (c_median > nc_median*10):
		print("Virtualization detected!")
	else:
		print("Virtualization not detected!")

if __name__ == '__main__':
	tmpfile = 'tmp'
	bufname = 'map'
	bufnames = {'map0': None, 'map1': None}
	pagesize = 4096
	seed = 100
	to_run = 'check_virtualization'
	offsets = get_offset(to_run, bufname, tmpfile)
	if offsets is None:
		exit(0)
	os.system('./%s %i %i %i' % (to_run, offsets[0]/4, offsets[1]/4, seed))
	check_timings()



