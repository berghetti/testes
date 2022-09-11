
# matriz de incidencia
#    e1__e2__e3
# v1| 1, 0, 1
# v2| 1, 1, 0
# v3| 0, 1, 1

GRAPH = [
     { 'name': 'v1', 'visited' : 0, 'edges' : [1, 0, 1, 0, 0] },
     { 'name': 'v2', 'visited' : 0, 'edges' : [1, 1, 0, 0, 0] },
     { 'name': 'v3', 'visited' : 0, 'edges' : [0, 1, 1, 1, 0] },
     { 'name': 'v4', 'visited' : 0, 'edges' : [0, 0, 0, 1, 1] },
     { 'name': 'v5', 'visited' : 0, 'edges' : [0, 0, 0, 0, 1] },
     # { 'name': 'v5', 'visited' : 0, 'edges' : [0, 0, 0, 1] }
]
queue = []
total_vertices = len(GRAPH)
count = 1

def check_adajacent_from_vertex(v):
    # if current vertx reaches other vertx, this advanced on graph
    global count
    # if vertex is not incident the edge, skip edge
    for edge in range(len(GRAPH)):
        if v['edges'][edge] == 0:
            continue

        # check all vertex incident in edge
        for i in range(len(GRAPH)):
            v2 = GRAPH[i]

            # if v['name'] == v2['name'] or \ #unnecessary because 'visited'
            if v2['edges'][edge] == 0 or \
               v2['visited']:
                continue

            count += 1
            v2['visited'] = 1
            queue.append(v2)

            print( '{} e {} são adjacentes pela aresta {}'.
            format(v['name'], v2['name'], edge + 1))


def bfs():
    GRAPH[0]['visited'] = 1
    queue.append(GRAPH[0])
    while len(queue):
        v = queue.pop(0)
        check_adajacent_from_vertex(v)

    # print(count, total_vertices)
    if (count == total_vertices):
        print('Grafo conexo')
    else:
        print('Grafo não conexo')

bfs()
