if __name__ == '__main__':
	sender = {}
	sender_start = None
	with open('debug_sender.txt') as reader:
		for line in reader:
			l = line.strip().split('\t')
			if sender_start is None:
				sender_start = int(l[0], 16)
			sender[int(l[0], 16) - sender_start] = l[1]
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
			if sender[index] != l[1]:
				j = ((index)/4096)%4
				page = ((index)/4096 - j)/4
				print("Page different: %i" % (page + j))

