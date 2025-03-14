import os
import matplotlib.pyplot as plt
import numpy as np
import matplotlib
matplotlib.rcParams['figure.dpi'] = 150
	
BITS = 64
BYTES = int(BITS / 8)

RUNS = 2**10
C = 0xbea225f9eb34556d

#inline uint64_t mix(uint64_t x) {
#	x ^= x >> 32;
#	x *= C;
#	x ^= x >> 29;
#	x *= C;
#	x ^= x >> 32;
#	x *= C;
#	x ^= x >> 29;
#	return x;
#}

def mix(x):
	x = x ^ (x >> 32)
	x = x * C
	x = x ^ (x >> 29)
	x = x * C
	x = x ^ (x >> 32)
	x = x * C
	x = x ^ (x >> 29)
	x = x * C
	return x
	
def urandom64(x):
	return int.from_bytes(os.urandom(8), 'big')
	
def setbits(n):
	global BITS
	global BYTES
	BITS = n
	BYTES = int(BITS / 8)

cm = matplotlib.colors.LinearSegmentedColormap.from_list('test', [(0,0,0), (0.1,1,0.1)])

def plot_hash(fn, title=None, bits=64):
	setbits(bits)

	vals = np.zeros((BITS, BITS))

	ideal_table = np.full((BITS, BITS), 0.5)

	for _ in range(RUNS):
		val = urandom64(0)
		h = fn(val)

		for i in range(BITS):
			new_val = val ^ (1 << i)
			new_h = fn(new_val)

			diff = h ^ new_h
			for j in range(BITS):
				if diff & 1:
					vals[i][j] += 1
				diff >>= 1
	vals /= RUNS
	plt.imshow(vals, cmap=cm, vmin=0, vmax=1, origin="lower")
	#plt.imshow(vals, cmap="hot", origin="lower")
	plt.colorbar()
	plt.xlabel("Input bits")
	plt.ylabel("Output bits")
	if not title:
		title = fn.__name__
	title = f"Diffusion diagram of {title}\n{BITS}-bit input, {BITS}-bit output"
	plt.title(title)


	# Distribution stats
	sse = np.square(vals - 0.5).sum()
	bias = abs(((ideal_table-vals)/ideal_table).mean()) * 100
	plt.figtext(0.99, 0.08, f"{min(100 - bias, 100):.4f}% diffusion",horizontalalignment ="right", fontsize=8)
	plt.figtext(0.99, 0.04, f"{bias:.4f}% bias", horizontalalignment ="right", fontsize=8)
	plt.figtext(0.99, 0, f"{sse:.6f} SSE", horizontalalignment ="right", fontsize=8)
	plt.tight_layout()
	plt.show()
	
if __name__ == "__main__":
	#plot_hash(urandom64)
	plot_hash(mix)