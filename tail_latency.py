
import matplotlib.pyplot as plt
# import matplotlib.ticker as mticker

data_raw = [
    { 'latency' : 80, 'pkts' : 13 },
    { 'latency' : 30, 'pkts' : 53 },
    { 'latency' : 35, 'pkts' : 1 },
    { 'latency' : 36, 'pkts' : 50 },
    { 'latency' : 38, 'pkts' : 10 },
    { 'latency' : 40, 'pkts' : 31 },
    { 'latency' : 47, 'pkts' : 8 },
    { 'latency' : 50, 'pkts' : 7 },
    { 'latency' : 60, 'pkts' : 9 },
    { 'latency' : 70, 'pkts' : 11 },
]

data_sorted = sorted(data_raw, key=lambda x: x['latency'], reverse=False)

# p - percentil = 0, 0.1, 0.5...
# data - data set
# plt - matplot
def set_latency_percentil (p, data, plt, text, color):
    pkts = total_pkts * p

    sum_pkts = 0
    for i, obj in enumerate(data):
        sum_pkts += obj['pkts']
        if sum_pkts >= pkts:
            plt.axvline( x=i, label=text, color=color )
            return

total_pkts = sum(obj['pkts'] for obj in data_sorted)

# axie x this array of latencys
x = [obj['latency'] for obj in data_sorted]
# pkts = [obj['pkts'] for obj in data_sorted]

fig, ax = plt.subplots()
ax.plot(x, marker='o')
ax.set(xlabel=
      'Data point numbers show packets frequency',
       ylabel='Latency (µs)',
       title='Tail latency')

for i, obj in enumerate(data_sorted):
    ax.annotate( str(obj['pkts']),
                 ( i, obj['latency'] ),
                 xytext= (i + 0.2, obj['latency'] - 1.2))

set_latency_percentil(0.5, data_sorted, plt, '50th', 'b')
set_latency_percentil(0.9, data_sorted, plt, '90th', 'r')
set_latency_percentil(0.99, data_sorted, plt, '99th', 'g')
set_latency_percentil(0.999, data_sorted, plt, '99.9th', 'm')

plt.legend()
plt.tick_params(
    axis='x',          # changes apply to the x-axis
    which='both',      # both major and minor ticks are affected
    bottom=False,      # ticks along the bottom edge are off
    top=False,         # ticks along the top edge are off
    labelbottom=False) # labels along the bottom edge are off
# ax.grid( visible=False, axis='y')
ax.grid()
# fig.savefig("test.png")
plt.show()
