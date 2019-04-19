import subprocess
import time
from time import sleep
import matplotlib.pyplot as plt
import sys

plt.style.use('ggplot')

def generate_bar_plot(x_data, y_data, _color) :
	plt.bar(x_data, y_data, color=_color)
	plt.xlabel("# threads")
	plt.ylabel("time in ms")
	if str(sys.argv[3]) == '-rm' :
		plt.title(str(sys.argv[1]) + " insert operations + " + str(sys.argv[1]) + " delete operations")
	else :
		plt.title(str(sys.argv[1]) + " insert operations")
	plt.show()

def bench(num_threads, num_inserts) :
	start = time.time() * 1000
	args = ["./klsm", str(num_threads), str(num_inserts)]
	subprocess.call(args)
	end = time.time() * 1000
	print(end - start)
	return(end - start)

def bench_rm(num_threads, num_inserts) :
	start = time.time() * 1000
	args = ["./klsm", str(num_threads), str(num_inserts), "-rm"]
	subprocess.call(args)
	end = time.time() * 1000
	print(end - start)
	return(end - start)

def bench_wrapper(num_inserts) :
	j = 1
	times = []
	for i in range(0, 6) :
		times.append(bench(j, num_inserts))
		j = j*2
	return times

def bench_wrapper_rm(num_inserts) :
	j = 1
	times = []
	for i in range(0, 6) :
		times.append(bench_rm(j, num_inserts))
		j = j*2
	return times

x = ['1T', '2T', '4T', '8T', '16T', '32T']

if sys.argv[3] == "-rm" :
	y = bench_wrapper_rm(int(sys.argv[1]))

else :
	y = bench_wrapper(int(sys.argv[1]))

generate_bar_plot(x, y, str(sys.argv[2]))




