import math

def median(x):
        s = sorted(x)
        l = len(x)
        i = math.floor(l/2)
        if l == 1:
                return s[0]
        if l%2 == 0:
                return (s[i] + s[i+1])/2
        return s[i]

if __name__ == '__main__':
	calibration_file = 'calibration.csv'
	receiver_file = 'received.csv'	
	sender = {}
	sender_start = None
	errors = {1: set([]), 0: set([])}
	received = []
	no_CoW = [int(x.strip()) for x in open(calibration_file).readlines()][8:]
	CoW = [int(x.strip()) for x in open(receiver_file).readlines()]
	nc_median = median(no_CoW)
	l = int(len(CoW)/8)
	for i in range(l-1,0, -1):
		start = i*8
		end = (i + 1)*8
		c_median = median(CoW[start:end])
		if (nc_median*2 > c_median):
			received.append(1)
		else:
			received.append(0)
	print(len(received))
	with open('debug_sender.txt') as reader:
		for line in reader:
			l = line.strip().split('\t')
			if sender_start is None:
				sender_start = int(l[0], 16)
			sender[int(l[0], 16) - sender_start] = int(l[1])
	receiver_start = None
	with open('debug_receiver.txt') as reader:
		for line in reader:
			l = line.strip().split('\t')
			if receiver_start is None:
				receiver_start = int(l[0], 16)
			index = int(l[0], 16) - receiver_start
			if index not in sender:
				print("Missing index %i\n" % index)
				exit(0)	
			if sender[index] != int(l[1]):
				j = ((index)/4096)%8
				page = int(((index)/4096 - j)/8)
				e = received[page]
				errors[e].add(page)
				#print("Page different: %i" % (page + j))
	for e in errors:
		print(e, len(errors[e]))

