
# matriz de incidencia
#    e1__e2__e3
# v1| 1, 0, 1
# v2| 1, 1, 0
# v3| 0, 1, 1

GRAPH = [
     { 'name': 'v1', 'visited' : 0, 'edges' : [1, 0, 1, 0, 0, 0] },
     { 'name': 'v2', 'visited' : 0, 'edges' : [1, 0, 0, 0, 0, 0] },
     { 'name': 'v3', 'visited' : 0, 'edges' : [0, 1, 1, 1, 0, 0] },
     { 'name': 'v4', 'visited' : 0, 'edges' : [0, 0, 0, 1, 1, 0] },
     { 'name': 'v5', 'visited' : 0, 'edges' : [0, 0, 0, 0, 1, 0] },
     { 'name': 'v6', 'visited' : 0, 'edges' : [0, 1, 0, 0, 0, 0] }
]
queue = []
distance = 0

def check_adajacent_from_vertex(v, search=None):
    # if current vertx reaches other vertx, this advanced on graph
    global distance
    advanced = 0
    # if vertex is not incident the edge, skip edge
    for edge in range(len(GRAPH)):
        print('checking {}'.format(v['name']))
        if v['edges'][edge] == 0:
            continue

        # check all vertex incident in edge
        for i in range(len(GRAPH)):
            v2 = GRAPH[i]

            # if v['name'] == v2['name'] or \ #unnecessary because 'visited'
            if v2['edges'][edge] == 0 or \
               v2['visited']:
                continue

            advanced = 1
            v2['visited'] = 1
            queue.append(v2)

            print( '{} e {} são adjacentes pela aresta {}'.
            format(v['name'], v2['name'], edge + 1))

            if search and search['name'] == v2['name']:
                print('found {} '.format(v2['name']) )
                distance += advanced
                return True

    distance += advanced
    return False


def get_distance(u, v):
    start = u
    u['visited'] = 1
    queue.append(u)
    while len(queue):
        u = queue.pop()
        if ( check_adajacent_from_vertex(u, v) ):
            break

    print('distance of {} and {} is {}'
    .format(start['name'], v['name'], distance) )

def bfs():
    GRAPH[0]['visited'] = 1
    queue.append(GRAPH[0])
    while len(queue):
        v = queue.pop(0) # remove element from fifo
        check_adajacent_from_vertex(v)

# get_distance(GRAPH[0], GRAPH[4])
bfs()
