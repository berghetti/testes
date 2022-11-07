
import sys
import matplotlib.pyplot as plt
from matplotlib.ticker import (MultipleLocator, AutoMinorLocator)

cycles_per_us = None

# x é o timestamp ordenado
# y é a latencia daquele timestamp
def windower(x, y, cycles_per_us, ms_window=20, percentile=0.999):
    newx, newy, newz = [], [], []
    i = 0
    start = None
    wsize = 0
    wcnts = 0
    cycles_per_window = ms_window * 1000 * cycles_per_us
    curWinEnd = x[0] + cycles_per_window

    print(x[0])
    print(curWinEnd)
    print(cycles_per_us)

    for i in range(len(x)):
        while x[i] > curWinEnd:
            if start is not None:
                # print(curWinEnd)
                # print(x[i])
                # print(y[start:i])
                newx.append((curWinEnd - x[0]) / (cycles_per_us * 1e6))
                ys = sorted(y[start:i])
                # print('len ys: {}'.format(len(ys)))
                #wsize += i - start
                #wcnts += 1
                #if ( ys[-1] > 200 ):
                #   print(y[start:i])
                #   print("{} \n".format(ys[int(len(ys) * percentile)]))

                if percentile == 'max':
                   newy.append(ys[-1])
                elif percentile == 'avg':
                   newy.append(sum(ys) / len(ys) if ys else 0)
                else:
                   newy.append(ys[int(len(ys) * percentile)])

                newz.append(len(ys) * (1e3 / ms_window))

            start = None
            curWinEnd += cycles_per_window

        if start is None:
            start = i
    return newx, newy, newz

def read_trace(f):
    WARMUP = 0
    dat = []
    with open(f) as ff:
        d = ff.read()

    is_sorted = True
    last_i = 0
    print("Reading full request trace... (may take awhile)")
    for line in d.splitlines():
        if "ticks / us" in line:
            global cycles_per_us
            cycles_per_us = int(line.split("detected")[1].split()[0])
            print('detected cyples_per_us = {}'.format(cycles_per_us))
            continue

        line = line.split("Trace: ")
        if len(line) < 2:
            continue

        for tup in line[1].split()[:-1]:
            # print(tup)
            a, b, c = tup.split(":", 2)
            if ':' in c:
                c, d = c.split(":")
            else:
                d = 0
            a, b, c = (int(a), float(b), float(c))
            if a <= 1000000 + WARMUP or b == -1:
                continue
            if c <= 0:
                c = float("inf")

            d = int(d)
            is_sorted = is_sorted and last_i <= d
            # assert is_sorted, "{} {} {}".format(last_i, d, f)
            last_i = d

            dat.append((d, a, c // 1000))

    print(len(dat))
    if is_sorted:
        return dat
    return sorted(dat)

# file - log memory
def read_mem(f, first_tsc, last_tsc, cycles_per_us, percentile='avg'):
    print('Reading memory file')
    x, y = [], []
    with open("{}".format(f) ) as f:
        for line in f:
            try:
                _m, mbps, tsc = line.split()
                tsc = int(tsc)
                if tsc < first_tsc or tsc > last_tsc: continue
                x.append(tsc - first_tsc)
                y.append(float(mbps))
            except:
                continue

    x, y, _z = windower(x, y, cycles_per_us, percentile = percentile)

    return x, y

#read garbage collector
def read_gc(f, first_tsc: int, last_tsc: int, cycles_per_us: int):
    print('Reading GC')
    
    data = []
    gc_cycle = {}
    with open(f) as f:
        for line in f:
            if "GC_EVENT" not in line: continue
   
            line2 = line.split() 
            event = line2[-1].strip()
            tsc = int( line2[-2][:-1] )
    
            if tsc < first_tsc or tsc > last_tsc:
                continue

            #if tsc > last_tsc and event == 'GC_EVENT_START':
            #    break
           
            gc_cycle[event] = ( tsc - first_tsc ) / ( cycles_per_us * 1e6 ) 
            if ( event == 'GC_EVENT_END' ):
                data.append( gc_cycle.copy() )
                #gc_cycle.clear()

    return data

def set_gc_mark(data, ax):
    #miny, maxy = ax.get_ylim()
    for cycle in data:
        try:
            ax.axvspan(cycle['GC_EVENT_START'], cycle['GC_EVENT_END'], facecolor='#cccccc')
        except:
            continue
        #ax.vlines(cycle['GC_EVENT_END'], miny, maxy, label='gc_end', linestyles ="dashed")


# file_trace - output from option '-o' synthetic application
# file_mem - output from ~/caladan/apps/netbench$ sudo numactl -N1 ./stress_shm_query membw:1000
# file_swaptions - output with G.C log from swaptions
# out - name to save chart.png
def main(file_trace, file_mem, file_swaptions, out):

    latencytrace = read_trace(file_trace)
    if not latencytrace:
        print('error read file')
        sys.exit(1)

    tm_tsc, tm, lat = zip(*latencytrace)

    gc_data = read_gc(file_swaptions, tm_tsc[0], tm_tsc[-1], cycles_per_us)
    #print(gc_data)

    # tm_tsc: é o momento que o pacote chegou no servidor, para todos os pacotes
    # tm: é o que o pacote saiu, para todos os pacotes
    # lat: é a latencia de todos os pacotes

    # print('tm_tsc:\n {}'.format(tm_tsc))
    # print('tm:\n {}'.format(tm))
    # print('lat:\n {}'.format(lat))

    # x é a janela
    # y é a latencia percentil da janela
    # z é a quantidade pacotes enviados em uma janea de 20ms * 1000 / tempo da janela
    # mas oq isso significa?

    fig, axs = plt.subplots(3)

    x, y, z = windower( tm_tsc, lat, cycles_per_us )
    axs[0].plot(x, y, 'o', markersize=2)
    axs[0].set_ylabel("Latency 99.9% (us)")
    #axs[0].set_ylim([0,500])
    #axs[0].set_yscale('log')

    axs[1].plot(x, z, '-')
    axs[1].set_ylabel("LC Throughput (pps)")
    axs[1].set_ylim(0, 1.25 * max(z))

    x, y = read_mem( file_mem, tm_tsc[0], tm_tsc[-1], cycles_per_us )
    axs[2].plot(x, y, '-')
    axs[2].set_ylabel("Mem avg BW (MB/s)")
    
    #x, y = read_mem( file_mem, tm_tsc[0], tm_tsc[-1], cycles_per_us, percentile='max' )
    #axs[3].plot(x, y, '-')
    #axs[3].set_ylabel("Mem max BW (MB/s)")

    plt.xlabel("Time (s)")

    # set common options subplots
    for i in range(len(axs)):
        set_gc_mark(gc_data, axs[i])
        axs[i].yaxis.set_minor_locator(AutoMinorLocator())
        axs[i].xaxis.set_major_locator(MultipleLocator(5))
        axs[i].xaxis.set_minor_locator(MultipleLocator(1))
        axs[i].set_xlim(left=0)
        #axs[i].set_xlim(0, 20)

    w, h = fig.get_size_inches()
    fig.set_size_inches(w*1.5, h*1.5)
    fig.tight_layout()
    plt.savefig(out)
    plt.show()

def main_only_trace(file_trace, out):
    latencytrace = read_trace(file_trace)
    if not latencytrace:
        print('error read file')
        sys.exit(1)

    tm_tsc, tm, lat = zip(*latencytrace)

    fig, ax = plt.subplots()

    x, y, z = windower( tm_tsc, lat, cycles_per_us, ms_window=20, percentile=0.999 )
    ax.plot(x, y, linestyle='--')
    ax.set_ylabel("Latency (us)")

    plt.xlabel("Time (s)")

    w, h = fig.get_size_inches()
    fig.set_size_inches(w*1.5, h*1.5)
    fig.tight_layout()
    #plt.show()
    plt.savefig(out)


if (len(sys.argv) == 3):
     main_only_trace( sys.argv[1], sys.argv[2] )
elif (len(sys.argv) == 5):
    main( sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4] )
else:
    print('Usage: {} file.trace file.mem file_swaptions out.png'.format(sys.argv[0]))
    sys.exit(1)

