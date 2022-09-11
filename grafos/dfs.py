
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

def check_adajacent_from_vertex(v):

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

            v2['visited'] = 1
            queue.append(v2)

            print( '{} e {} são adjacentes pela aresta {}'.
            format(v['name'], v2['name'], edge + 1))

            return

    queue.pop() #remove element from stack

def dfs():
    GRAPH[0]['visited'] = 1
    queue.append(GRAPH[0])
    while len(queue):
        print(queue)
        v = queue[-1] # acess top element from stack
        check_adajacent_from_vertex(v)

dfs()
