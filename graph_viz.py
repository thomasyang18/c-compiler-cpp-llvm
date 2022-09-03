# A tool for debugging AST output.

import sys

cur_file = sys.stdin

node_name = {}  # tuple to string
node_adj = {}  # tupel to list
stack = {}  # int to int

depth = 0

par = []

with sys.stdin as f:
    flat_list=[word for line in f for word in line.split()]
    for i in range(0, len(flat_list), 3):
        l = flat_list[i:i+3]
        if l[0] == "Enter":
            if depth not in stack:
                stack[depth] = []
            cur = (depth, len(stack[depth]))
            stack[depth].append(cur)

            node_adj[cur] = []

            if par:
                node_adj[par[-1]].append(cur)
            node_name[cur] = l[2].replace('%', '')
            par.append(cur)

            depth += 1
        elif l[0] == "Exit":
            depth -= 1
            par.pop()


def print_graph():
    cur = (0, 0)
    nodes = set()
    edges = set()

    def dfs(node):
        nodes.add(node)
        for a in node_adj[node]:
            dfs(a)
            edges.add((node, a))

    dfs(cur)

    print("digraph main {".strip())

    def tuple_to_node(node):
        return node_name[node].strip() + "_" + str(node[0]) + "_" + str(node[1])

    for node in nodes:
        print(tuple_to_node(node), ";")
    for edge in edges:
        print(tuple_to_node(edge[0]), "->", tuple_to_node(edge[1]), ";")

    print("}")


def interact():
    cur = (0, 0)
    while True:
        print("Current node:", node_name[cur])
        print("Children:")

        i = 0
        for node in node_adj[cur]:
            print(i, node_name[node])
            i += 1

        x = input()
        y = -1
        try:
            y = int(x)
        except ValueError:
            y = -1
        if y in range(0, len(node_adj[cur])):
            par.append(cur)
            cur = node_adj[cur][y]
        elif x == ".." and par:
            cur = par.pop()
        else:
            print("Try again")
            continue


print_graph()