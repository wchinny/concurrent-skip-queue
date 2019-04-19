import subprocess
import time
import matplotlib.pyplot as plt

plt.style.use('ggplot')

def generate_bar_plot(x_data, y_data, _color) :
	plt.bar(x_data, y_data, color=_color)
	plt.xlabel("# threads")
	plt.ylabel("time in ms")
	plt.show()

def bench(num_threads, num_inserts) :
	start = time.time() * 1000
	args = ["./klsm", str(num_threads), str(num_inserts)]
	subprocess.call(args)
	end = time.time() * 1000

	print(end - start)
	return(end - start)

def bench_wrapper(num_inserts) :
	j = 1
	times = []
	for i in range(0, 4) :
		times.append(bench(j, num_inserts))
		j = j*2
	return times

x = ['1T', '2T', '4T', '8T']

## 10000 inserts
# y = bench_wrapper(10000)
# generate_bar_plot(x, y, 'blue')

## 50000 inserts
y = bench_wrapper(100000)
generate_bar_plot(x, y, 'blue')




